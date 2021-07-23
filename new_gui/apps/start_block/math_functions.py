'''Math and plotting functions used in views file'''
#python modules
import csv
import numpy as np
from matplotlib import pyplot as plt
import os
from pathlib import Path
import socket

def save_csv_data(file_name):
    BASE_DIR = Path(__file__).resolve().parent.parent.parent
    file_path = os.path.join(
            BASE_DIR,
            F"static/data/{file_name}.csv"
            )
    print(file_path)
    file = open(file_path, "a")

    s_point = socket.socket()
    port = 50
    host = "10.20.1.56"
    print(F"Connecting to {host} in port {port}")
    s_point.settimeout(10)
    try:
        s_point.connect((host, port))
        s_point.settimeout(None)
        message = b"1"
        s_point.send(message)

        data = b""
        number = 0
        llega = b""
        print(F'Receiving data in {file_name}')
        while (not data == b"!"):
            data = s_point.recv(1)
            #print(data)
            llega += data
            if (data == b"\n"):
                number += 1
                print(F"{number} = {str(llega)}")
                file.write(F"{llega.decode('ascii')}")
                llega = b""
    except OSError:
        return redirect("home")
    except Exception as E:
        print("Error: ")
        print(E)
        return redirect("home")

    file.close()
    s_point.close()


def read_csv_data(file_name):
    file_handle = open(file_name, 'r', encoding='utf-8')
    csv_reader = csv.DictReader(file_handle)
    final_dict = list()
    for row in csv_reader:
        keys = row.keys()
        for key in keys:
            try:
                final_dict.append({key: float(row[key])})
            except ValueError:
                pass
            except Exception as E:
                print("Error")
                print(E)
                print(row[key])

    keys = list(keys)
    file_handle.close()
    return final_dict, keys

def raw2list(column_name, dict_):
    final = []
    for row in dict_:
        try:
            final.append(row[column_name])
        except KeyError:
            pass

    return final

def graph_data(file_name):

    global BASE_DIR
    BASE_DIR = Path(__file__).resolve().parent.parent.parent
    file_path = os.path.join(
            BASE_DIR,
            F"static/data/{file_name}.csv"
            )
    print(file_path)
    raw_data, keys_list = read_csv_data(file_path)
    time = raw2list('time', raw_data)
    #left feet data
    left_forces = ['l_force','r_force']
    forces = []
    for key in left_forces:
        force = raw2list(key, raw_data)
        forces.append(force)
    plot_feet_data(F"{file_name}_left", forces, time, 'Forces Vs Time')
    max_force_gauche = max(forces[0])
    max_force_droit = max(forces[1])

    return max_force_gauche, max_force_droit

def plot_feet_data(file_name, forces, time, title):
    global BASE_DIR
    file_path = os.path.join(
            BASE_DIR,
            F"static/data/{file_name}.png"
            )

    force_1 = np.array(forces[0])
    force_2 = np.array(forces[1])

    i = 0
    mean_force = []
    for value in force_1:
        mean_value = force_1[i] + force_2[i]
        mean_value /= 2
        mean_force.append(mean_value)
        i += 1

    mean_force = np.array(mean_force)

    plt.plot(time, force_1, ":", label="Capteur 1")
    plt.plot(time, force_2, ":", label="Capteur 2")
    plt.plot(time, mean_force, "b-", label="Force Moyenne")

    plt.xlabel('Temps (ms)')
    plt.ylabel('Force (Kg)')
    plt.title(title)

    plt.legend()
    plt.grid(True)
    plt.show()

    print(F"Saving picture to {file_path}")
    plt.savefig(file_path)
    plt.close()
