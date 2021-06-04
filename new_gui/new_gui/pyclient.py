import socket               
 
sock = socket.socket()
 
host = "10.20.1.56" #ESP32 IP in local network
port = 50             #ESP32 Server Port    
 
sock.connect((host, port))
try: 
    
    message = b"2"
    sock.send(message)
     
    data = b""       
    number = 0 
    llega = b""
    while (not data == b"!"):
        data = sock.recv(1)
        llega += data
        if (data == b"\n"):
            number += 1
            print(F"{number} = {str(llega)}")
            llega = b""
     
except Exception as E:
    print("Error: ")
    print(E)
 
sock.close()
