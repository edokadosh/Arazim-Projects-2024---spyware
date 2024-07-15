import socket
import sys
from _thread import *
import threading
from connection import Connection
from encConnection import EncConnection
from icecream import ic


class Listener:

    def __init__(self, host, port, backlog=1000):
        self.host = host
        self.port = port
        self.backlog = backlog
        self.socket = socket.socket()
        self.socket.setsockopt(
            socket.SOL_SOCKET, socket.SO_REUSEADDR, 1
        )  # Optional: Enable socket reuse

    def __repr__(self):
        return f"Listener(port={self.port}, host={self.host}, backlog={self.backlog})"


        print(f"Listening on {self.host}:{self.port}")

    def stop(self):
        self.socket.close()

    def accept(self) -> EncConnection:
        connSocket, connAddr = self.socket.accept()
        conn = EncConnection(connSocket)
        print(f"accepted connection {conn}")
        return conn

    def __enter__(self):
        self._start()
        return self

    def __exit__(self, exc_type, exc_value, exc_traceback):
        if exc_type != None:
            print("\nExecution type:", exc_type)
            print("\nExecution value:", exc_value)
            print("\nTraceback:", exc_traceback)
        self.stop()
