from agent import Agent
from icecream import ic


class Operation:
    def __init__(self) -> None:
        print("Empty initiator for operation")
        self.spyAgent = None
        self.managerAgent = None
        self.netDriver = None

    def addAgent(self, agent: Agent, type: str) -> None:
        if type == "maneger":
            if self.managerAgent is None:
                self.managerAgent = agent
                return
            if not self.managerAgent.conn.is_open():
                self.managerAgent = agent
        elif type == "spy":
            if self.spyAgent is None:
                self.spyAgent = agent
                return
            if not self.spyAgent.conn.is_open():
                self.spyAgent = agent
        else:
            ic("wrong type")


"""

Sivan:
repr

"""
