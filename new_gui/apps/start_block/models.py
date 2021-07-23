from django.db import models
from apps.admin2.models import Athletes,Coach

def get_coach():
    return Coach.objects.get_or_create(id=1)


class Session(models.Model):
    athlete =  models.ForeignKey(
            'admin2.Athletes',
            verbose_name='Athlete',
            on_delete = models.CASCADE,
            )
    coach =  models.ForeignKey(
            'admin2.Coach',
            verbose_name='Coach',
            on_delete = models.CASCADE,
            )
    data = models.FileField(
            upload_to='static/data/',
            )
    max_force_left = models.IntegerField(
            default=10,
            blank=True,
            )
    max_force_right = models.IntegerField(
            default=10,
            blank=True,
            )
    enabled = models.BooleanField(
            default = True
            )
    created = models.DateTimeField(auto_now_add=True)
    modified = models.DateTimeField(auto_now=True)

    def __str__(self):
        return F"{self.athlete} => {self.created} par {self.coach}"
