from connection import Connection
from command import Command
from responce import Responce

HOST = "192.168.83.130"
PORT = 65432


def main():
    with Connection.connect(HOST, PORT) as conn:
        conn.send_command(Command(45, 3, "mooooo"))
        res = conn.recv_responce()
        print(f"{res = }")


if __name__ == "__main__":
    main()
