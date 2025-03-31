import socket

ESP32_IP = "192.168.4.1"
PORT = 12345  

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((ESP32_IP, PORT))

with open("pm2.5.txt", "w") as pmfile, open("co2.txt", "w") as co2file, open("temp.txt", "w") as tempfile:
    while True:
        response = client.recv(1024).decode().strip()
        if not response:
            print("Connection closed by the server.")
            break
        
        lines = response.strip().split('\n')
        try:
            lines = [line.split(':')[1] for line in lines]
            pm1 = lines[0].strip()
            pm2_5 = lines[1].strip()
            co2 = lines[3].strip()
            temp = lines[4].strip()
            
            print(f"PM1: {pm1} PM2.5: {pm2_5}, CO2: {co2}, Temp: {temp}") 
            
            pmfile.write(pm2_5 + "\n")
            pmfile.flush()  
            
            co2file.write(co2 + "\n")
            co2file.flush()  
            
            tempfile.write(temp + "\n")
            tempfile.flush()  
        except IndexError as e:
            print(f"Error parsing response: {e}")
