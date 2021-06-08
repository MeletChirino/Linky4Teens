from django.db import models

class Athletes(models.Model):
    numero_id = models.IntegerField()
    nom = models.CharField(max_length=20)
    prenom = models.CharField(max_length=30)
    date_naissance = models.DateField()
    photo = models.ImageField(
            upload_to = 'static/img/',
            verbose_name="Photo de Profil",
            blank=True,
            )
    created = models.DateTimeField(auto_now_add=True)
    modified = models.DateTimeField(auto_now=True)

    def __str__(self):
        return F"{self.nom}"



