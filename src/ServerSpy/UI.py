"""
from connection import Connection
from listener import Listener
from command import Command
from responce import Responce
from funCode import FunCode
from status import Status
from icecream import ic
"""
#from agent import Agent
#from operation import Operation
from contParams import *
import click


class Agent():
    def __init__(self, x):
        self.x = x

    def f(self, s):
        print(f'f {self.x}: {s}')

class Operation():
    def __init__(self, s):
        self.spyAgent = Agent(s)
        self.managerAgent = Agent(s)

class Context:
    def __init__(self, operations):
        self.selected_operation: Operation = None
        self.oper_dict = operations
        self.cont_ident = 0


def call_method(obj, method: str, *args):
    if hasattr(obj, method):
        try:
            getattr(obj, method)(*args)
        except:
            print("OOPSIE: probably wrong arguments")
    else:
        print(f"OOPSIE: Method {method} not found")

def print_methods(obj):
        attributes = dir(obj)
        methods = [attr for attr in attributes if callable(getattr(obj, attr)) and not attr.startswith("__")]
        print("YAY: Available Methods: (q for exit)")
        print(methods)

class UI:
    def __init__(self, operations: dict[str, Operation]) -> None:
        self.ctx = Context(operations)

    def is_op_active(self) -> bool:
        if not self.ctx.selected_operation:
            print("OOPSIE: Unselected operation")
            return False
        return True

    def select(self, name: str):
        if name not in self.ctx.oper_dict.keys():
            print("OOPSIE: Contraption not found")
        self.ctx.selected_operation = self.ctx.oper_dict[name]
        print(f"YAY: Selected {name}")

    def spyware(self, method, *args):
        if self.is_op_active():
            call_method(self.ctx.selected_operation.spyAgent, method, *args)

    def manager(self, method, *args):
        if self.is_op_active():
            call_method(self.ctx.selected_operation.managerAgent, method, *args)

    # list operations
    def ops(self):
        print("YAY: Available operations:")
        for op_name in self.ctx.oper_dict.keys():
            if (self.ctx.selected_operation == self.ctx.oper_dict[op_name]):
                print("-> ", end='')
            else:
                print('   ', end='')
            print(f"{op_name}")

    def rename(self, old_name, new_name):
        if old_name not in self.ctx.oper_dict.keys():
            print("OOPSIE: Operation not found")
            return
        if new_name in self.ctx.oper_dict.keys():
            print("OOPSIE: new name exists")
            return
        self.ctx.oper_dict[new_name] = self.ctx.oper_dict.pop(old_name)

    # list agent operations
    def agentops(self):
        print_methods(Agent(0))

    def help(self):
        print_methods(self)

    def bash(self, cmd: str):
        if not self.is_op_active():
            return
        res = self.manager('run_bash', cmd)
        print(f"DB: {res=}")
        return res[1]

    def findNet(self):
        nets = self.bash('ls /sys/class/net')
        for net in nets:
            if net != 'lo':
                self.ctx.selected_operation.netDriver = net
                return True
        print("OOPSIE: Could not find non localhost network device")
        return False

    def sniff(self, time: int):
        if not self.is_op_active():
            return
        net_driver = self.ctx.selected_operation.netDriver
        if not net_driver:
            print("MEH: Trying to find out network driver")
            if not self.findNet():
                return
        params = ContParams(SnifferType, Params(SniffParams(time, net_driver.encode())))
        print(self.ctx.selected_operation.spyAgent.runContraption(params, self.ctx.cont_ident))
        self.ctx.cont_ident += 1

        
if __name__ == "__main__":
    operations = {'mat': Operation('m'), 'buja': Operation('b')}
    ui = UI(operations)
    ui.help()

    value = input('$ ')
    while value != 'q':
        args = value.split()
        call_method(ui, args[0], *args[1:])
        value = input('$ ')
