"""new_gui URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/3.1/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path, include
from django.conf.urls.static import static
from django.conf import settings
#local views
from .views import serial_list, report_page, i2c_page

urlpatterns = [
    path('admin/', admin.site.urls),
    path('i2c/<int:number>', i2c_page),
    path('home/', serial_list, name='home'),
    path('graph/', report_page, name='report_page'),
    path('admin2', include(('apps.admin2.urls','admin2'), namespace='admin2')),
    path(
        'start_block',
        include(
            ('apps.start_block.urls','start_block'),
            namespace='start_block')
        ),
] + static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)
