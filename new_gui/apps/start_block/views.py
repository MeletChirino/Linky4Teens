#python
import socket
import time
import csv

#django modules
from django.shortcuts import render, redirect
from django.views.generic.edit import CreateView

#models
from apps.start_block.models import Session
from apps.admin2.models import Athletes

class CreateSessionView(CreateView):
    model = Session
    fields = [
            'athlete',
            ]
    template_name = "start_point_home.html"

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        context['file_name'] = generate_file_name()
        return context

def generate_file_name():
    year = time.localtime()[0]
    month =  time.localtime()[1]
    day =  time.localtime()[2]
    hour =  time.localtime()[3]
    mins =  time.localtime()[4]
    sec =  time.localtime()[5]
    name = F"static/data/{year}_{month}_{day}_{hour}_{mins}_{sec}.csv"
    return name

def results(request):
    '''
    if request.method == "GET":
        return redirect("start_point:home")
    '''
    #athlete = request.POST['athlete']
    #file_name = request.POST['data']
    template_name = 'start_point/results.html'

    athlete = '1'
    file_name = "static/data/2021_6_8_10_14_38.csv"
    data = {
            "holi": "holi",
            }

    return render(
            request,
            template_name,
            context = data
            )

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

def get_dict_row(column_name, dict_):
    final = []
    for row in dict_:
        #import pdb; pdb.set_trace()
        try:
            final.append(row[column_name])
        except KeyError:
            pass

    #print(final)
    return final



def get_data(request):
    name = request.POST["data"]
    file = open(name, "a")

    #if(request.method == "GET"):
    #    return redirect("start_block:home")

    s_point = socket.socket()
    port = 50
    host = "10.20.1.56"
    print(F"Connecting to {host} in port {port}")
    s_point.connect((host, port))
    try:
        message = b"1"
        s_point.send(message)

        data = b""
        number = 0
        llega = b""
        print(F'Receiving data in {name}')
        while (not data == b"!"):
            data = s_point.recv(1)
            #print(data)
            llega += data
            if (data == b"\n"):
                number += 1
                #print(F"{number} = {str(llega)}")
                file.write(F"{llega.decode('ascii')}")
                llega = b""

    except Exception as E:
        print("Error: ")
        print(E)

    file.close()
    s_point.close()
    new_data = Session()
    new_data.athlete = Athletes.objects.get(id=request.POST['athlete'])
    new_data.data = request.POST['data']
    new_data.save()

    return redirect("start_block:home")
