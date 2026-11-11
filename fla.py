import socket
import time

max_pwm = 3200
start_pwm = 1500
# 1. Create a TCP/IP socket
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
    # 2. Connect to the server
    client_socket.connect(('192.168.2.16', 1234))
    
    # 3. Send data
    client_socket.sendall(b"Hello, Server")
    null = input("Press Enter to begin FLA Test")
    client_socket.sendall(b"")
    # 4. Receive response
    data = client_socket.recv(1024)
    print(f"Received: {data.decode()}")
