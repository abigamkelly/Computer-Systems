# Abigail Kelly
# Dr. Sainju
# Final Project: server.py
# This is the server program that receives a string from the client that contains how
# many equations are in the system, and the equations.  This program will turn the
# string into a sympy object and solve the equations.  The solutions are then turned
# into a string and sent back to the client.  An empty string will be returned to the
# client if there are no solutions to the equation.

import socket
from sympy import *
from sympy.abc import x, y, z
from sympy.parsing.sympy_parser import parse_expr
import threading

# set IP, PORT, ADDR, SIZE, FORMAT
IP = socket.gethostbyname(socket.gethostname())
PORT = 58927
ADDR = (IP, PORT)
SIZE = 1024
FORMAT = "utf-8"

# this function handles the client connections
def handle_client(conn, addr):
    # print message that client has connected
    print(f"[NEW CONNECTION] {addr} connected.")
    equations = []
    connected = True
    while connected:
        msg = conn.recv(SIZE).decode(FORMAT)    # get message from client

        # if the msg from the client is 0, end the connection
        if msg == "0":
            connected = False
            continue

        # split string received from client where there is a comma and save in a list
        msg = msg.split(',')
        msg.pop()       # get rid of last element of list

        #iterates over list and turns each element into a sympy object
        for i in range(0, len(msg)):
            equations.append(parse_expr(msg[i]))

        # solve the system of equations
        solutions = solve(equations, x, y, dict=True)
        
        sol = str(solutions)            # change sympy object to string
        conn.send(sol.encode(FORMAT))   # send message to client
        equations.clear()               # clear the list
        
    # print the disconnected message
    print(f"[DISCONNECTED] {addr} disconnected.")
    conn.close()               # close the connection

def main():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # start the server
    server.bind(ADDR)   # binds IP address and Port
    server.listen()     # listen for a connection

    while True:
        conn, addr = server.accept()    # accept connection from client
        thread = threading.Thread(target=handle_client, args=(conn, addr))  # create a new thread
        thread.start()      #s tart the thread


if __name__ == "__main__":
    main()