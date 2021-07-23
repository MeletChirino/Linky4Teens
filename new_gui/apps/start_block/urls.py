
"""URLs de starting block """
from django.urls import path, include
from django.views.generic import TemplateView
from django.conf.urls.static import static
from django.conf import settings
#local views
from apps.start_block.views import (
        get_data, CreateSessionView, results,
        DetailSessionView, ListSessionView,
        delete
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
            route = '-DeleteSession/<session_id>',
            view = delete,
            name = "DeleteSession",
            ),
        path(
            route = '-results/',
            view = results,
            name = "results",
            ),
        path(
            route = '-session/<session_id>',
            view = DetailSessionView.as_view(),
            name = "DetailSession",
            ),
        path(
            route = '-session-list',
            view = ListSessionView.as_view(),
            name = "SessionList"
            ),
        ]
