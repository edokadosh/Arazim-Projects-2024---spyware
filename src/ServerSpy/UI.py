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
    def __init__(self, oper_dict: dict[str, Operation]) -> None:
        print("Initiating UI")
        self.oper_dict = oper_dict
        self.active_contraption = None


    def activate_contraption(self, name):
        if name not in self.oper_dict.keys():
            print("OOPSIE: Contraption not found")
        


    def list_operations(self):
        print("Available operations:")
        for op_name, oper in self.oper_dict.items():
            print(f"{op_name}\t\t{oper}")

    def list_operation_names(self):
        print("Available operations:")
        for op_name in self.oper_dict.keys():
            print(f"{op_name}")


    def rename_operation(self, old_name, new_name):
        #self.list_operation_names()
        #old_name = input("Old name: ")
        if old_name not in self.oper_dict.keys():
            print("OOPSIE: Operation not found")
            return
        
        #new_name = input("New name: ")
        if new_name in self.oper_dict.keys():
            print("OOPSIE: new name exists")
            return
        
        self.oper_dict[new_name] = self.oper_dict.pop(old_name)
        
