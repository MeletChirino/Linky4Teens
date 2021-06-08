
"""URLs de starting block """
from django.urls import path, include
from django.views.generic import TemplateView
from django.conf.urls.static import static
from django.conf import settings
#local views
from apps.start_block.views import (
        get_data, CreateSessionView, results,
        )
from apps.admin2.views import (ListAthletesView,
        DetailAthleteView,
        )

urlpatterns = [
        path(
            route = '-home/',
            view = CreateSessionView.as_view(),
            name = "home",
            ),
        path(
            route = '-get/',
            view = get_data,
            name = "get_data",
            ),
        path(
            route = '-results/',
            view = results,
            name = "results",
            ),
        ]
