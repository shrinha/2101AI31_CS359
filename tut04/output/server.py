import socket

# Create a socket object
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to a specific address and port
server_address = ("127.0.0.1", 12345)
server_socket.bind(server_address)

# Listen for incoming connections
server_socket.listen(1)
print("Server is Listening at socket 12345")

while True:
    # Wait for a connection
    print("Waiting for a connection...")
    client_socket, client_address = server_socket.accept()
    print(f"Connected to {client_address}")

    # Receive data from the client
    data = client_socket.recv(1024).decode('utf-8')
    data_1 = data[7:] 
    print(f"Received: {data_1}")

    if data.startswith("Task 1"):
        # Task 1: Echo back a message
            response = "Have a good day"
            
    elif data.startswith("Task 2"):
        # Task 2: Count vowels in a string
        text = data.split(":")[1]
        vowels = sum(1 for char in text if char.lower() in "aeiou")
        response = f"Number of vowels: {vowels}"
        
    elif data.startswith("Task 3"):
        # Task 3: Check if a string is a palindrome
        text = data.split(":")[1]
        is_palindrome = (text.lower() == text[::-1].lower())
        if is_palindrome:
            response = "String is palindrome"
        else:
            response = "String is not palindrome"
    elif data.startswith("Task 4"):
        # Task 4: Sum of elements in an integer array
        numbers = [int(x) for x in data.split(":")[1].split(",")]
        array_sum = sum(numbers)
        response = f"Sum of elements: {array_sum}"
    else:
        response = "Invalid task"

    # Send the response back to the client
    client_socket.send(response.encode('utf-8'))

    # Close the connection
    client_socket.close()
