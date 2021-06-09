from django.db import models
from apps.admin2.models import Athletes

class Session(models.Model):
    athlete =  models.ForeignKey(
            'admin2.Athletes',
            verbose_name='Athlete',
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
    created = models.DateTimeField(auto_now_add=True)
    modified = models.DateTimeField(auto_now=True)

    def __str__(self):
        return F"{self.athlete} => {self.created}"
