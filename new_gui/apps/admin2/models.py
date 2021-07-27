from django.db import models

class Athletes(models.Model):
    numero_id = models.IntegerField()
    nom = models.CharField(max_length=20)
    prenom = models.CharField(max_length=30)
    date_naissance = models.DateField()
    photo = models.ImageField(
            upload_to = 'img/',
            verbose_name="Photo de Profil (pas necessaire)",
            blank=True,
            )
    categorys = [
            ("VE", "Masters"),
            ("SE", "Seniors"),
            ("ES", "Espoirs"),
            ("JU", "Junior"),
            ("CA", "Cadets"),
            ("MI", "Minimes"),
            ("BE", "Benjamines"),
            ]
    categorie = models.CharField(
            default = "MI",
            max_length = 2,
            choices = categorys,
            )
    enabled = models.BooleanField(
            default = True
            )
    created = models.DateTimeField(auto_now_add=True)
    modified = models.DateTimeField(auto_now=True)



    def __str__(self):
        return F"{self.nom}"

class Coach(models.Model):
    numero_id = models.IntegerField()
    nom = models.CharField(max_length=20)
    prenom = models.CharField(max_length=30)
    date_naissance = models.DateField()
    photo = models.ImageField(
            upload_to = 'img/',
            verbose_name="Photo de Profil (pas necessaire)",
            blank=True,
            )
    categorys = [
            ("VE", "Masters"),
            ("SE", "Seniors"),
            ("ES", "Espoirs"),
            ("JU", "Junior"),
            ("CA", "Cadets"),
            ("MI", "Minimes"),
            ("BE", "Benjamines"),
            ]
    categorie = models.CharField(
            verbose_name = "Categroie Entraine",
            default = "MI",
            max_length = 2,
            choices = categorys,
            )
    enabled = models.BooleanField(
            default = True
            )
    created = models.DateTimeField(auto_now_add=True)
    modified = models.DateTimeField(auto_now=True)

    def __str__(self):
        return F"{self.nom}"

