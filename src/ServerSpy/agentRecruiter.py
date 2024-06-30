import threading
from agent import Agent
from listener import Listener


class AgentRecruiter(threading.Thread):

    def __init__(
        self,
        host: str,
        port: int,
        agentList: list[Agent],
        recruitedEvent: threading.Event,
    ):
        super().__init__()
        self._stop_event = threading.Event()
        self.recruitedEvent = recruitedEvent
        self.recruited = agentList
        self.listener = Listener(host, port)

    def run(self):
        with self.listener as listener:
            while not self._stop_event.is_set():
                conn = listener.accept()
                self.recruited.append(Agent(conn))
                self.recruitedEvent.set()
