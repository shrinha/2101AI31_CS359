import socket

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ("127.0.0.1", 12345)
client_socket.connect(server_address)

message = "Task 1:Hello IITP"
client_socket.send(message.encode('utf-8'))

response = client_socket.recv(1024).decode('utf-8')
print(f"Server Response: {response}")
        
client_socket.close()
