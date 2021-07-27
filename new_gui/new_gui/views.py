"""Views file for django"""
# python modules
import time
import os
from pathlib import Path
import socket
from git import Repo
from pathlib import Path

# django modules
from django.shortcuts import render, redirect

# serial functions
from apps.relays.serial_module_ import serial_ports

def update_view(request):
    print("updating GUI")
    BASE_DIR = Path(__file__).resolve().parent.parent.parent
    print(BASE_DIR)
    repo = Repo(BASE_DIR)
    origin = repo.remotes.origin
    origin.pull("master")
    return redirect("home")


def home(request):
    template_name = 'home.html'
    info = {
            'start_block': 'Disconnected',
            'title': 'Bienvenu',
            }
    #serial ports
    serial_ports_ = serial_ports()
    info['serial_ports'] = serial_ports_

    starting_block = StartingBlock(
            host = "10.20.1.56",
            port = 50,
            )
    starting_block.get_charge()
    info['starting_block'] = starting_block


    return render(
            request,
            template_name,
            context=info
            )

class StartingBlock:
    def __init__(self, host, port, **kwargs):
        self.port = port
        self.host = host
        self.connected = False
        self.charge = 0
        self.icon = "fas fa-bug"
        self.warning = ''

    def get_charge(self):
        s_point = socket.socket()
        s_point.settimeout(2)
        port = 50
        print(F"Connecting to {self.host} in port {self.port}")
        try:
            s_point.connect(
                    (self.host, self.port)
                    )
        except Exception as E:
            return None

        s_point.settimeout(None)
        print(F"Connected!")
        try:
            message = b"2"
            s_point.send(message)

            data = b""
            number = 0
            llega = b""
            print(F'Receiving data ')
            while (not data == b"!"):
                data = s_point.recv(1)
                print(F"data = {data}")
                llega += data
                if (data == b"\n"):
                    number += 1
                    print(F"{number} = {str(llega)}")
                    msg = llega.decode('ascii')
                    print(F"{number} = {str(msg)}")
                    llega = b""
                    self.connected = True
                    self.charge = msg
                    print(F'Message {msg}')
            if int(msg) > 80:
                self.icon = "fas fa-battery-full"
            elif int(msg) <= 80 and int(msg) > 75:
                self.icon = "fas fa-battery-three-quarters"
            elif int(msg) <= 75 and int(msg) > 50:
                self.icon = "fas fa-battery-half"
            elif int(msg) <= 50 and int(msg) > 25:
                print("menor que 50 y mayor que 25")
                self.icon = "fas fa-battery-quarter"
            elif int(msg) <= 25 and int(msg) > 0:
                self.icon ='fas fa-battery-empty'
        except Exception as E:
            self.connected = False
            print("Error: ")
            print(E)
        s_point.close()

class CheckStartingBlock:
    def __init__(self, get_response):
        self.get_response = get_response
        pass

    def __call__(self, request):
        starting_block = StartingBlock(
                host = "10.20.1.56",
                port = 50,
                )
        starting_block.get_charge()

        request.starting_block = starting_block

        response = self.get_response(request)
        return response

def serial_list(request):
    serial_ports_list = serial_ports()
    print(serial_ports_list)
    list_ = {'serial_ports_list': serial_ports_list}

    return render(request, 'serial_list.html', context=list_)

