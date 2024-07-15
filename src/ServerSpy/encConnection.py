from connection import Connection
import socket
import ssl
from icecream import ic

CERT_FILE = "server.crt"
KEY_FILE = "server.key"


class EncConnection(Connection):

    def __init__(self, conn_socket, cert_file=CERT_FILE, key_file=KEY_FILE):
        context = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH)
        context.load_cert_chain(certfile=cert_file, keyfile=key_file)
        ssl_socket = context.wrap_socket(conn_socket, server_side=True)
        super().__init__(ssl_socket)

    @classmethod
    def connect(
        cls, host, port, cert_file=CERT_FILE, key_file=KEY_FILE
    ) -> "EncConnection":
        # create socket
        conn_socket = socket.socket()
        conn_socket.connect((host, port))

        # create encryption contex
        context = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH)
        print("CY")
        context.load_cert_chain(certfile=cert_file, keyfile=key_file)
        print("BER")
        ssl_socket = context.wrap_socket(conn_socket, server_side=True)
        return cls(ssl_socket)

    def recv_bytes(self, length: int) -> bytes:
        # print("try enc recv_bytes")
        return self.socket.read(length)

    def send_data(self, data: bytes):
        self.socket.write(data)

    def is_open(self) -> bool:
        try:
            if self.socket.getpeercert():
                return True
            return False
        except:
            return False
