
"""URLs de relays """
from django.urls import path, include
from django.views.generic import TemplateView
from django.conf.urls.static import static
from django.conf import settings

#local views
from .views import (CreateCourseView, results)

urlpatterns = [
        path(
            route = '-home/',
            view = CreateCourseView.as_view(),
            name = "home",
            ),
        path(
            route = '-results/',
            view = results,
            name = "results",
            ),
        ]
