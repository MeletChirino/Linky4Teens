from django.db import models
from apps.admin2.models import Athletes

class Course(models.Model):
    athlete =  models.ForeignKey(
            'admin2.Athletes',
            verbose_name='Athlete',
            on_delete = models.CASCADE,
            )
    data = models.FileField(
            upload_to='static/data/',
            )
    created = models.DateTimeField(auto_now_add=True)
    modified = models.DateTimeField(auto_now=True)

    def __str__(self):
        return F"{self.athlete} => {self.created}"
