"""URLs de admin2 """
from django.urls import path, include
from django.views.generic import TemplateView
from django.conf.urls.static import static
from django.conf import settings
#local views
from apps.admin2.views import (ListAthletesView,
        DetailAthleteView, CreateAthleteView,
        UpdateAthleteView,
        )

urlpatterns = [
        path(
            route = '-AthleteList/',
            view = ListAthletesView.as_view(),
            name = "AthleteList",
            ),
        path(
            route = '-AthleteDetail/<numero_id>',
            view = DetailAthleteView.as_view(),
            name = "DetailAthlete"
            ),
        path(
            route = '-CreateAthlete/',
            view = CreateAthleteView.as_view(),
            name = "CreateAthlete",
            ),
        path(
            route = '-UpdateAthlete/<numero_id>',
            view = UpdateAthleteView.as_view(),
            name = "UpdateAthlete",
            ),
        ]
