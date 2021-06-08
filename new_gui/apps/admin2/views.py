#django modules
from django.shortcuts import render, redirect
from django.views.generic import (ListView, UpdateView,
        DetailView
        )
from django.views.generic.edit import CreateView

#local modules
from apps.admin2.models import Athletes
from apps.start_block.models import Session

class ListAthletesView(ListView):
    model = Athletes
    template_name = 'athlete_list.html'

class DetailAthleteView(DetailView):
    template_name = 'athlete-detail.html'
    slug_field = "numero_id"
    slug_url_kwarg = "numero_id"
    queryset = Athletes.objects.all()

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        athlete_id = context['object'].id
        context['sessions'] = Session.objects.filter(athlete = athlete_id)
        return context

class CreateAthleteView(CreateView):
    model = Athletes
    fields = [
            'numero_id',
            'nom',
            'prenom',
            'date_naissance',
            'photo',
            ]
    template_name = 'create_athlete'

