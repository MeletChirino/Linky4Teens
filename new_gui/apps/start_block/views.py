#python
import os
import socket
import time
import csv
from pathlib import Path

#django modules
from django.shortcuts import render, redirect
from django.views.generic.edit import CreateView
from django.views.generic import DetailView, FormView, ListView

#models
from apps.start_block.models import Session
from apps.admin2.models import Athletes, Coach

#local functions
from .math_functions import graph_data, save_csv_data

class DetailSessionView(DetailView):
    template_name = 'start_point/results.html'
    model = Session
    pk_url_kwarg = 'session_id'


    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        session_id = context['object'].id
        session_data = Session.objects.get(pk = session_id)
        file_name = session_data.data
        context["graph_gauche"] = F"data/{file_name}_left.png"
        context["graph_droit"] = F"data/{file_name}_right.png"
        context["max_force_gauche"] = session_data.max_force_left
        context['max_force_droit'] = session_data.max_force_right
        return context

def delete(request, session_id):
    data = Session.objects.filter(id = session_id)
    data.update(
            enabled = False,
            )
    return redirect("start_block:SessionList")

class CreateSessionView(CreateView):
    model = Session
    fields = [
            'athlete',
            'coach',
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
    name = F"{year}_{month}_{day}_{hour}_{mins}_{sec}"
    return name

def results(request):
    BASE_DIR = Path(__file__).resolve().parent.parent.parent
    template_name = 'start_point/results.html'

    coach_id = request.POST["coach"]
    coach_name = Coach.objects.get(
            id = coach_id
            ).nom
    file_name = request.POST["data"]
    file_name = F"{coach_name}_{file_name}"
    save_csv_data(file_name)

    print("graficando")
    max_force_gauche, max_force_droit = graph_data(file_name)

    #context data
    data = {
            "graph_gauche": F"data/{file_name}_left.png",
            #"graph_droit": F"data/{file_name}_right.png",
            "max_force_gauche": max_force_gauche,
            'max_force_droit': max_force_droit,
            }

    #saving model
    new_data = Session()
    new_data.data = file_name
    new_data.athlete = Athletes.objects.get(id=request.POST['athlete'])
    new_data.coach = Coach.objects.get(
            id = request.POST['coach']
            )
    new_data.max_force_left = max_force_gauche
    new_data.max_force_right = max_force_droit
    new_data.save()

    return render(
            request,
            template_name,
            context = data
            )

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

class ListSessionView(ListView):
    model = Session
    template_name = 'start_point/list_sessions.html'

    def get_queryset(self):
        return Session.objects.filter(
                athlete__enabled = True,
                ).exclude(
                enabled = False,
                )

    def get_ordering(self):
        ordering = self.request.GET.get('ordering', 'created')
        # validate ordering here
        return ordering
