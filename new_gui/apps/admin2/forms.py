from django import forms
from .models import Athletes

class DateInput(forms.DateInput):
    input_type = 'date'

class NumberInput(forms.NumberInput):
    input_type = 'number'

class RegisterForm(forms.ModelForm):
    class Meta:
        model = Athletes
        fields = {
                'numero_id',
                'nom',
                'prenom',
                'date_naissance',
                'photo',
                }
        widgets = {
                'numero_id': NumberInput(
                    attrs={
                        'class': 'form-control form-control-user',
                        'placeholder': 'Numero ID'
                        }
                    ),
                'nom': forms.TextInput(
                    attrs={
                        'class': 'form-control form-control-user',
                        'placeholder': 'Nom'
                        }
                    ),
                'prenom': forms.TextInput(
                    attrs={
                        'class': 'form-control form-control-user',
                        'placeholder': 'Prenom'
                        }
                    ),
                'date_naissance': DateInput(
                    attrs={
                        'class': 'form-control form-control-user',
                        'placeholder': 'Numero ID'
                        }
                    ),
                'photo': forms.FileInput(
                    attrs={
                        'class': 'form-control form-control-user',
                        'placeholder': 'Numero ID'
                        }
                    ),
                }
        def save(self):
            data = self.cleaned_data
            new_data = Athletes(**data)
            new_data.save()
