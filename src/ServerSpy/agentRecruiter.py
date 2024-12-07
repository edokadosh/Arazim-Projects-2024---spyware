import threading
from agent import Agent
from listener import Listener


class AgentValidator(threading.Thread):

    def __init__(self):
        super().__init__()

    def run(self, agent: Agent) -> tuple[str, Agent]:
        agent


class AgentRecruiter(threading.Thread):

    def __init__(
        self,
        host: str,
        port: int,
        recruited_list: list[Agent],
        type: str,  # "maneger" or "spyware"
    ):
        super().__init__()
        self._stop_event = threading.Event()
        self.recruitedEvent = threading.Event()
        self.recruited_list = recruited_list
        self.listener = Listener(host, port)
        self.type = type

    def run(self):
        with self.listener as listener:
            while not self._stop_event.is_set():
                conn = listener.accept()
                agent = Agent(conn)

                self.recruited_list.append(agent)
                self.recruitedEvent.set()
