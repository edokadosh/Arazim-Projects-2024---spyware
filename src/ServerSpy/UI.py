"""
from connection import Connection
from listener import Listener
from command import Command
from responce import Responce
from funCode import FunCode
from status import Status
from contParams import *
from icecream import ic
"""
#from agent import Agent
#from operation import Operation
import click


class Agent():
    def __init__(self, x):
        self.x = x

    def f(self, s):
        print(f'f {self.x}: {s}')

class Operation():
    def __init__(self, s):
        self.spyAgent = Agent(1 + s)
        self.managerAgent = Agent(2 + s)

class Context:
    def __init__(self, operations):
        self.selected_operation = None
        self.oper_dict = operations


def call_method(agent: Agent, method: str, *args):
    if hasattr(agent, method):
        try:
            print("DB: Received args:")
            getattr(agent, method)(*args)
        except:
            print("OOPSIE: probably wrong arguments")
    else:
        print(f"OOPSIE: Method {method} not found")


class UI:
    def __init__(self, operations: dict[str, Operation]) -> None:
        print("Initing")
        self.ctx = Context(operations)
        self.cli = click.Group()
        self.add_commands()

    def add_commands(self):
        self.cli.add_command(self.select_contraption)
        self.cli.add_command(self.list_operation_names)
        self.cli.add_command(self.list_operations)
        self.cli.add_command(self.rename_operation)
        self.cli.add_command(self.spyware)
        self.cli.add_command(self.manager)

    @click.command
    @click.argument('name')
    @click.pass_context
    def select_contraption(ctx: Context, name: str):
        if name not in ctx.oper_dict.keys():
            print("OOPSIE: Contraption not found")
        ctx.selected_operation = name
        print(f"Activated {name}")

    @click.command
    @click.argument('method')
    @click.argument('args', nargs=-1)
    @click.pass_context
    def spyware(ctx, method, *args):
        call_method(ctx.selected_operation.spyAgent, method, *args)

    @click.command
    @click.argument('method')
    @click.argument('args', nargs=-1)
    @click.pass_context
    def manager(ctx, method, *args):
        call_method(ctx.selected_operation.managerAgent, method, *args)


    @click.command
    @click.pass_context
    def list_operations(ctx):
        print("Available operations:")
        for op_name, oper in ctx.oper_dict.items():
            print(f"{op_name}\t\t{oper}")


    @click.command
    @click.pass_context
    def list_operation_names(ctx):
        print(ctx)
        print("Available operations:")
        for op_name in ctx.oper_dict.keys():
            print(f"{op_name}")


    @click.command
    @click.argument('old_name')
    @click.argument('new_name')
    @click.pass_context
    def rename_operation(ctx, old_name, new_name):
        #self.list_operation_names()
        #old_name = input("Old name: ")
        if old_name not in ctx.oper_dict.keys():
            print("OOPSIE: Operation not found")
            return
        
        #new_name = input("New name: ")
        if new_name in ctx.oper_dict.keys():
            print("OOPSIE: new name exists")
            return
        
        ctx.oper_dict[new_name] = ctx.oper_dict.pop(old_name)
    
    def run(self):
        self.cli(obj=self.ctx)
        
if __name__ == "__main__":
    operations = {'mat': Operation(0), 'buja': Operation(1)}
    ui = UI(operations)
    #ui.run()

    #value = click.prompt('$', type=click.Choice(list(ui.cli.commands.keys()) + ['exit']))
    value = input('$ ')
    while value != 'exit':
        args = value.split()
        ui.cli.commands[args[0]]('e')
