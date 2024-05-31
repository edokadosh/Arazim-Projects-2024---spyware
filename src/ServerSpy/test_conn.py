from Connection import Connection
from Command import Command
from Responce import Responce

HOST = "192.168.83.130"
PORT = 65432


def main():
    with Connection.connect(HOST, PORT) as conn:
        conn.send_command(Command(45, 3, "mooooo"))


if __name__ == "__main__":
    main()
