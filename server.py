import socket

# Define host and port
HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
PORT = 65432        # Port to listen on (non-privileged ports are > 1023)

# Create a TCP/IP socket
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
    # Bind the socket to the address and port
    server_socket.bind((HOST, PORT))
    
    # Listen for incoming connections
    server_socket.listen()

    print(f"Server is listening on {HOST}:{PORT}")

    # Accept connections indefinitely
    while True:
        # Accept a connection
        client_socket, client_address = server_socket.accept()
        print(f"Connection from {client_address}")

        # Send a response back to the client
        response = input("msg: ")
        client_socket.sendall(response.encode())

        # Receive data from the client
        data = client_socket.recv(1024)
        if not data:
            break  # If no data received, break the loop

        # Print received data
        print(f"Received data: {data.decode()}")

    # Close the connection
    client_socket.close()
