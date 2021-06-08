'''Math and plotting functions used in views file'''
import csv
import numpy as np
from matplotlib import pyplot as plt

def read_csv_data(file_name):
    file_handle = open(file_name, 'r', encoding='utf-8')
    csv_reader = csv.DictReader(file_handle)
    final_dict = list()
    for row in csv_reader:
        keys = row.keys()
        for key in keys:
            final_dict.append({key: float(row[key])})

    keys = list(keys)
    file_handle.close()
    return final_dict, keys

def raw2list(column_name, dict_):
    final = []
    for row in dict_:
        #import pdb; pdb.set_trace()
        try:
            final.append(row[column_name])
        except KeyError:
            pass

    #print(final)
    return final

def graph_data(file_name):
    raw_data, keys_list = read_csv_data(file_name)
    time = raw2list('time', raw_data)
    #left feet data
    left_forces = ['l_force_1', 'l_force_2', 'l_force_3', 'l_force_4']
    forces = []
    for key in left_forces:
        force = raw2list(key, raw_data)
        forces.append(force)
    import pdb; pdb.set_trace()
    plot_feet_data(file_name, forces, time)
    #right feet data
    left_forces = ['r_force_1', 'r_force_2', 'r_force_3', 'r_force_4']
    forces = []
    for key in left_forces:
        force = raw2list(key, raw_data)
        forces.append(force)
    #plot_feet_data(file_name, forces, time)


def plot_feet_data(file_name, forces, time):

    force_4 = np.array(forces[0])
    force_1 = np.array(forces[1])
    force_2 = np.array(forces[2])
    force_3 = np.array(forces[3])

    i = 0
    mean_force = []
    for value in force_1:
        mean_value = force_1[i] + force_2[i] + force_3[i] + force_4[i]
        mean_value /= 4
        mean_force.append(mean_value)
        i += 1
    mean_force = np.array(mean_force)

    plt.plot(force_1, time, "r:", label="Capteur 1")
    plt.plot(force_2, time, "r:", label="Capteur 2")
    plt.plot(force_3, time, "r:", label="Capteur 3")
    plt.plot(force_4, time, "r:", label="Capteur 4")
    plt.plot(mean_force, time, "b-", label="Force Moyenne")

    plt.xlabel('Temps (ms)')
    plt.ylabel('Force (Kg)')
    plt.title('Title')

    plt.legend()
    plt.grid(True)

    plt.savefig(F"{file_name}.png")

