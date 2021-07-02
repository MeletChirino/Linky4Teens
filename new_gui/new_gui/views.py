"""Views file for django"""
# python modules
import time
import os
from pathlib import Path
import socket

# django modules
from django.shortcuts import render, redirect

# serial functions
from apps.relays.serial_module_ import serial_ports

#i2c and electronic modules
from smbus import SMBus

def home(request):
    template_name = 'home.html'
    info = {
            'start_block': 'Disconnected',
            'title': 'Bienvenu',
            }

    #serial ports
    serial_ports_ = serial_ports()
    info['serial_ports'] = serial_ports_

    s_point = socket.socket()
    s_point.settimeout(10)
    port = 50
    host = "10.20.1.56"
    start_time = time.time()
    print(F"Connecting to {host} in port {port}")
    try:
        s_point.connect((host, port))
    except Exception as E:
        print("Error: Timeout!")
        print(E)
        return render(
                request,
                template_name,
                context=info
                )
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
            #print(data)
            llega += data
            if (data == b"\n"):
                number += 1
                print(F"{number} = {str(llega)}")
                msg = llega.decode('ascii')
                print(F"{number} = {str(msg)}")
                #file.write(F"{llega.decode('ascii')}")
                llega = b""
                info['start_block'] = "Connected!"
                print(F'Message {msg}')

    except Exception as E:
        print("Error: ")
        print(E)

    s_point.close()

    return render(
            request,
            template_name,
            context=info
            )



def i2c_page(request, number):
    bus = SMBus(1) # indicates /dev/i2c-1
    if number == 1:
        bus.write_byte(0x8, 0x1)
        return redirect("home")
    elif number == 2:
        bus.write_byte(0x9, 0x1)
        return redirect("home")
    else:
        bus.write_byte(0x8, 0x0)
        bus.write_byte(0x9, 0x0)
        return redirect("home")

def serial_list(request):
    serial_ports_list = serial_ports()
    print(serial_ports_list)
    list_ = {'serial_ports_list': serial_ports_list}

    return render(request, 'serial_list.html', context=list_)

def report_page(request):
    BASE_DIR = Path(__file__).resolve().parent.parent
    if request.method!="POST":
        return redirect('home')

    port = request.POST['port']
    try:
        arduino = Temoin(port, 115200)
        arduino.connect()
    except:
        return serial_list()
    if(arduino.connected):
        data = arduino.get_data()
        print("data =  ")
        lap = 0
        t1 = []
        t2 = []
        t3 = []
        times = []
        relay_state = []
        for data_raw in data:
            try:
                splitted_data = str(data_raw).split("/")
                print(splitted_data)
                #import pdb; pdb.set_trace()
                t1_ = float(splitted_data[1])
                t3_ = float(splitted_data[3])

                t1.append(t1_)
                t2.append(float(splitted_data[2]))
                t3.append(t3_)
                times.append(t1_)
                times.append(t3_)
                relay_state.append(int(float(splitted_data[4])))
                print(F"Lap {lap}\nt1 = {t1[lap]}, t2 = {t2[lap]}, t3 = {t3[lap]}, relay_ok = {relay_state[lap]}")
                lap += 1
            except Exception as err:
                print(F"error aleatorio guinho ginho: {err}")
        distance = []
        for i in range(1,6):
            d1 = int(request.POST[F'distance{i}'])
            d2 = d1 + int(request.POST[F'distancet'])
            distance.append(d1)
            distance.append(d2)
        print(F'distance\nLen = {len(distance)}')
        distance = np.array(distance)
        print(distance)
        times_lapse = []
        times = np.array(times)/1000
        for i, time in enumerate(times):
            if i == 0:
                times_lapse.append(time)
            else:
                new_time = time - times[i-1]
                times_lapse.append(new_time)

        print(F'times\nLen = {len(times_lapse)}')
        print(times)

        speed = distance / times_lapse
        print(speed)

        #plotting script
        fig, ax = plt.subplots()
        ax.plot(times, speed)
        ax.grid()
        img_path = os.path.join(BASE_DIR, 'static/img/graph.png')
        print(img_path)
        fig.savefig(img_path)

        relay = range(1,6)

        print(F'relay\nLen = {len(relay)}')
        relay = np.array(relay)
        print(relay)

        print(F'state\nLen = {len(relay_state)}')
        print(relay_state)

        fig, ax = plt.subplots()
        ax.stem(relay, relay_state)
        img_path = os.path.join(BASE_DIR, 'static/img/graph2.png')
        fig.savefig(img_path)
        #plt.show()


        data = {
                'img': img_path,
                'speed': speed.tolist(),
                'mean_speed': distance[-1]/times[-1],
                'state': relay_state,

                }
        #return render(request, 'index.html', context={'img': img_path})
        return render(request, 'index.html', context=data)
    else:
        pass
