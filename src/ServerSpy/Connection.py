import socket
import struct
import sys
from Command import Command


class Connection:

    def __init__(self, conn_socket):
        self.socket: socket.socket = conn_socket
        self.local_name = conn_socket.getsockname()
        self.local_ip, self.local_port = self.local_name
        self.peer_name = conn_socket.getpeername()
        self.peer_ip, self.peer_port = self.peer_name

    def __repr__(self):
        return f"<Connection from {self.local_ip}:{self.local_port} to {self.peer_ip}:{self.peer_port}>"

    @classmethod
    def connect(cls, host, port) -> "Connection":
        conn_socket = socket.socket()
        conn_socket.connect((host, port))
        return Connection(conn_socket)

    def close(self) -> None:
        self.socket.close()

    def send_command(self, cmd: Command):
        self.socket.send(cmd.pack())

    def send_data(self, data: bytes):
        self.socket.send(data)

    def recv_data():
        pass

    def recv_string():
        pass

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, exc_traceback):
        if exc_type != None:
            print("\nExecution type:", exc_type)
            print("\nExecution value:", exc_value)
            print("\nTraceback:", exc_traceback)
        self.close()
