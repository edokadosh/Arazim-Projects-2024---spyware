import socket
import struct
import sys
from command import Command
from responce import Responce
from icecream import ic


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
        return cls(conn_socket)

    def close(self) -> None:
        self.socket.close()

    def send_command(self, cmd: Command):
        ic(cmd.funCode)
        self.socket.send(cmd.pack())

    def send_data(self, data: bytes):
        self.socket.send(data)

    def send_string(self, string: str):
        self.send_data(string.encode("utf-8"))

    # TODO add error handeling
    def recv_bytes(self, length: int) -> bytes:
        return self.socket.recv(length)

    def recv_responce_struct(self):
        bytes_recived = self.recv_bytes(Responce.sizeof)
        return Responce.unpack(bytes_recived)

    def recv_string(self, length: int) -> str:
        return self.recv_bytes(length).decode("utf-8")

    def recv_full_responce(self) -> tuple[Responce, str]:
        res = self.recv_responce_struct()
        msg = self.recv_string(res.dataLen)
        return res, msg

    def recv_file(self) -> bytes:
        (length,) = struct.unpack("=I", self.recv_bytes(struct.calcsize("=I")))
        return self.recv_bytes(length)

    def is_open(self) -> bool:
        try:
            # this will try to read bytes without blocking and also without removing them from buffer (peek only)
            data = self.socket.recv(16, socket.MSG_DONTWAIT | socket.MSG_PEEK)
            if len(data) == 0:
                return False
        except BlockingIOError:
            return True  # socket is open and reading from it would block
        except ConnectionResetError:
            return False  # socket was closed for some other reason
        except Exception as e:
            # logger.exception("unexpected exception when checking if a socket is closed")
            return True
        return True

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, exc_traceback):
        if exc_type != None:
            print("\nExecution type:", exc_type)
            print("\nExecution value:", exc_value)
            print("\nTraceback:", exc_traceback)
        self.close()
