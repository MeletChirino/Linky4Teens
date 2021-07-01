#django modules
from django.shortcuts import render, redirect
from django.views.generic import (ListView, UpdateView,
        DetailView, FormView
        )
from django.views.generic.edit import CreateView, UpdateView
from django.urls import reverse_lazy

#local modules
from apps.admin2.models import Athletes
from apps.start_block.models import Session
from apps.admin2.forms import RegisterForm

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

class CreateAthleteView(FormView):
    success_url = reverse_lazy('admin2:AthleteList')
    form_class = RegisterForm
    template_name = 'admin2/create_athlete.html'

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        context['title'] = "Creer un compte"
        return context

    def form_valid(self, form):
        form.save()
        return super(CreateAthleteView, self).form_valid(form)

class UpdateAthleteView(UpdateView):
    template_name = 'admin2/create_athlete.html'
    model = Athletes
    slug_field = 'numero_id'
    slug_url_kwarg = 'numero_id'

    fields = [
            'numero_id',
            'nom',
            'prenom',
            'date_naissance',
            'photo',
            ]
    success_url = reverse_lazy('admin2:AthleteList')

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        context['title'] = "Modifier les donnes"
        return context

    def form_valid(self, form):
        form.save()
        return super().form_valid(form)
