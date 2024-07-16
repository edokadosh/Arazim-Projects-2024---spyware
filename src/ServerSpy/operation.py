from agent import Agent
from icecream import ic


class Operation:
    def __init__(self) -> None:
        self.spyAgent = None
        self.managerAgent = None
        self.netDriver = None
        self.name = None
        self.ip = None

    def addAgent(self, agent: Agent, type: str) -> None:
        if type == "maneger":
            self.ip = agent.conn.peer_ip
            self.name = self.ip
            if self.managerAgent is None:
                self.managerAgent = agent
                return
            if not self.managerAgent.conn.is_open():
                self.managerAgent = agent
        elif type == "spy":
            self.ip = agent.conn.peer_ip
            self.name = self.ip
            if self.spyAgent is None:
                self.spyAgent = agent
                return
            if not self.spyAgent.conn.is_open():
                self.spyAgent = agent
        else:
            ic("wrong type")

    def __str__(self) -> str:
        if self.name:
            return f'{self.name}'
        return f'{self.ip}'

"""

Sivan:
repr

"""
