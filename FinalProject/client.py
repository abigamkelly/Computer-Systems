# Abigail Kelly
# Dr. Sainju
# Final Project: client.py
# This is the client program that will act as the user interface for a
# systems of equations solver.  This program will prompt the user to enter
# the number of equations in the system and the equations.  The system will
# be sent to the server, which will solve them and send back the solutions
# if there are any.

import socket

# set IP, PORT, ADDR, SIZE, FORMAT
IP = socket.gethostbyname(socket.gethostname())
PORT = 58927
ADDR = (IP, PORT)
SIZE = 1024
FORMAT = "utf-8"

def main():
    # establish client socket
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(ADDR)    #connect to the server

    # prompt the user for the number of equations they would like to enter
    num_equations = int(input("How many equations would you like to enter? "))
    equations = ""

    # loop until the user enters 0
    while num_equations != 0:
        if num_equations > 3:
            num_equations = int(input("Please enter a number <= 3: "))
            continue
        elif num_equations < 0:
            num_equations = int(input("Please enter a positive number <= 3: "))
            continue
        else:        
            # read in the equations
            for x in range(0, num_equations):
                equ = input("Enter an equation: ")
                equations += equ
                equations += ','
            # send the string of equations to the sever
            client.send(equations.encode(FORMAT))
            data = client.recv(SIZE).decode(FORMAT) # receive message from server
            
            # split the string received from the server and put in array
            data = data.split(',')     
            print("Solutions:")    # display the solutions
            for x in range(0, len(data)):
                if x == 0 or x % 2 == 0:
                    print(data[x])
                else:
                    print(data[x])
                
            num_equations = int(input("How many equations would you like to enter? "))
            equations = ""
    client.send(str(num_equations).encode(FORMAT))

if __name__ == "__main__":
    main()