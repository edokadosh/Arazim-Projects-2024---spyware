import socket

# Define host and port
HOST = '192.168.154.1'  # Standard loopback interface address (localhost)
PORT = 65432        # Port to listen on (non-privileged ports are > 1023)
BUFFER_SIZE = 4096

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


    def handleConnection(self) -> None:
        # Send a response back to the client
        response = input("msg: ")
        self.client_socket.sendall(response.encode())

        # Receive data from the client
        data = self.client_socket.recv(BUFFER_SIZE)
        if not data:
            return

        # Print received data
        print(f"Received data: {data.decode()}")

        print("Trying to receive a matbuja:")
        self.recvFile("matbuj.jpg")
        print("Done (Hopefully)")


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


def main() -> None:
    serv = Server()
    serv.beServer()


if __name__ == "__main__":
    main()
