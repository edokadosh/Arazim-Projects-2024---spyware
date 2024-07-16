from listener import Listener
from connection import Connection
from encConnection import EncConnection


HOST = "localhost"
PORT = 4545


def main():
    with Listener(HOST, PORT) as listener:
        conn = listener.accept()
        print(conn.is_open())
        conn.send_data(b"encrypted message from server")
        print("sent data")
        print(conn.recv_string(5))
        print(conn.recv_string(12))

        conn.socket.close()
        print(conn.is_open())


if __name__ == "__main__":
    main()
