#django modules
from django.shortcuts import render, redirect
from django.views.generic import (ListView, UpdateView,
        DetailView, FormView
        )
from django.views.generic.edit import CreateView, UpdateView
from django.urls import reverse_lazy
from django.db.models import Q

#local modules
from apps.admin2.models import Athletes, Coach
from apps.start_block.models import Session
from apps.admin2.forms import RegisterForm, CoachRegisterForm

# ------ Athlete Views ------
class ListAthletesView(ListView):
    model = Athletes
    template_name = 'athlete_list.html'

    def get_queryset(self):
        #this function excludes disabled data
        return Athletes.objects.exclude(enabled=False)

class DetailAthleteView(DetailView):
    template_name = 'athlete-detail.html'
    slug_field = "numero_id"
    slug_url_kwarg = "numero_id"
    queryset = Athletes.objects.all().exclude(
            enabled = False
            )

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        athlete_id = context['object'].id
        context['sessions'] = Session.objects.filter(
                athlete = athlete_id
                ).exclude(
                enabled = False
                )
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
            'categorie',
            ]
    success_url = reverse_lazy('admin2:AthleteList')

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        context['title'] = "Modifier les donnes"
        return context

    def form_valid(self, form):
        form.save()
        return super().form_valid(form)

def delete(request, athlete_id):
    data = Athletes.objects.filter(id = athlete_id)
    data.update(
            enabled = False,
            )
    return redirect("admin2:AthleteList")

# ------ Coach Views ------

class CreateCoachView(FormView):
    success_url = reverse_lazy('admin2:CoachList')
    form_class = CoachRegisterForm
    template_name = 'admin2/create_athlete.html'

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        context['title'] = "Creer un compte de Coach"
        return context

    def form_valid(self, form):
        form.save()
        return super(CreateCoachView, self).form_valid(form)

class UpdateCoachView(UpdateView):
    template_name = 'admin2/create_coach.html'
    model = Coach
    slug_field = 'numero_id'
    slug_url_kwarg = 'numero_id'

    fields = [
            'numero_id',
            'nom',
            'prenom',
            'date_naissance',
            'photo',
            'categorie',
            ]
    success_url = reverse_lazy('admin2:CoachList')

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        context['title'] = "Modifier les donnes du Coach"
        return context

    def form_valid(self, form):
        form.save()
        return super().form_valid(form)

class ListCoachView(ListView):
    #print("coach list")
    model = Coach
    template_name = 'coach_list.html'

class DetailCoachView(DetailView):
    template_name = 'admin2/coach_detail.html'
    slug_field = "numero_id"
    slug_url_kwarg = "numero_id"

    queryset = Coach.objects.all()

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        coach_id = context['object'].id
        context['sessions'] = Session.objects.filter(
                coach = coach_id,
                ).exclude(
                Q(enabled = False)|
                Q(athlete__enabled = False)
                )
        return context
