import socket
import ssl

# Generate or load your server certificate and key
certfile = 'server.crt'  # Replace with your server's certificate file
keyfile = 'server.key'   # Replace with your server's private key file

context = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH)
context.load_cert_chain(certfile, keyfile)

# Create a TCP/IP socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(('localhost', 8888))
server_socket.listen(5)

print("Server started. Waiting for connections...")

while True:
    # Accept incoming connections
    connection, client_address = server_socket.accept()
    ssl_conn = context.wrap_socket(connection, server_side=True)

    try:
        print(f"Connection from {client_address}")

        # Receive data
        data = ssl_conn.recv(1024)
        if data:
            print(f"Received: {data.decode()}")

            # Echo back to client
            ssl_conn.sendall(data)
        else:
            print("No more data from client")
            break

    finally:
        # Clean up the connection
        ssl_conn.close()
