from connection import Connection
from listener import Listener
from command import Command
from responce import Responce
from funCode import FunCode
from status import Status
from contParams import *
from icecream import ic
from agent import Agent
from operation import Operation


class UI:
    def __init__(self, oper_dict: dict[str, ]) -> None:
        print("Initiating UI")
        self.oper_dict = oper_dict

    def list_operartions(self):
        print("Listing operations:")
        for op_name, oper in self.oper_dict.items():
            print  
