#python
import os

#django modules
from django.shortcuts import render, redirect
from django.views.generic.edit import CreateView
from django.views.generic import DetailView, FormView

#models
from apps.relays.models import Course
from apps.admin2.models import Athletes

#funtions
from apps.start_block.views import generate_file_name
from .serial_module_ import serial_ports, get_data

class CreateCourseView(CreateView):
    model = Course
    fields = [
            'athlete',
            ]
    template_name = "relays/create.html"

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        context['file_name'] = generate_file_name()
        return context


def results(request):
    template_name = 'start_point/results.html'

    file_name = request.POST["data"]

    #creacion del objeto y timeout
    devices = serial_ports()
    #check devices
    i = 0
    for device in devices:
        #para cada uno haces el get data
        print(F"Get data para {i} => {device}")
        get_data(
                port_name = device,
                file_name = F"{file_name}_t{i}"
                )
        i += 1

    #hacer graficos

    #context data
    data = {
            "graph": F"data/{file_name}.png",
            }

    #saving model
    athlete_ = Athletes.objects.get(
            id = request.POST['athlete']
            )
    new_data = Course(
            athlete = athlete_,
            data = request.POST['data'],
            ).save()

    return render(
            request,
            template_name,
            context = data
            )
    template_name = 'start_point/results.html'


