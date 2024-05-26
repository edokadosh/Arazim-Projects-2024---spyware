import socket
import Message_pb2

# Define host and port
HOST = '192.168.154.1'  # Standard loopback interface address (localhost)
PORT = 65432        # Port to listen on (non-privileged ports are > 1023)
BUFFER_SIZE = 1024

class Server:
    def sendFile(self, path: str) -> None:
        with open(path, 'rb') as file:
            data = file.read(BUFFER_SIZE)
            while data:
                self.client_socket.send(data)
                data = file.read(BUFFER_SIZE)


    def recvFile(self, path: str) -> None:
        with open(path, 'wb') as file:
            while True:
                data = self.client_socket.recv(BUFFER_SIZE)
                if not data:
                    break
                file.write(data)

    def recvData(self) -> str:
        # Receive data from the client
        data = self.client_socket.recv(BUFFER_SIZE)
        if not data:
            return
        return data


    def handleConnection(self) -> None:
        cmd = "cowsay cyber"
        print(f"Sending command '{cmd}'")
        self.sendCommand(3, cmd)
        msg = self.recvCommand()

        # Print received data
        print(f"Received data: {msg.fnccode},{msg.param1},{msg.param2},{msg.param3}")
        

    def beServer(self):
        # Create a TCP/IP socket
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
            server_socket.bind((HOST, PORT))        
            server_socket.listen()
            print(f"Server is listening on {HOST}:{PORT}")
            
            # Accept connections indefinitely
            while True:
                # Accept a connection
                self.client_socket, self.client_address = server_socket.accept()
                print(f"Connection from {self.client_address}")
                self.handleConnection()


    def sendCommand(self, code: int, param1: str, param2="", param3="") -> None:
        msgObj = Message_pb2.Message()
        msgObj.fnccode = code
        msgObj.param1 = param1
        if (param2):
            msgObj.param2 = param2
        if (param3):
            msgObj.param3 = param3

        print(msgObj.SerializeToString())
        self.client_socket.sendall(msgObj.SerializeToString())
        print("Done")

    def recvCommand(self) -> Message_pb2.Message:
        msg = Message_pb2.Message()
        msg.ParseFromString(self.recvData())
        return msg


def main() -> None:
    serv = Server()
    serv.beServer()


if __name__ == "__main__":
    main()