import socket

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ('localhost', 12345)
client_socket.connect(server_address)

# Prompt the user to enter an integer array
user_input = input("Enter an integer array (comma-separated): ")
message = f"Task 4:{user_input}"
client_socket.send(message.encode('utf-8'))

response = client_socket.recv(1024).decode('utf-8')
print(f"Server Response: {response}")

client_socket.close()
