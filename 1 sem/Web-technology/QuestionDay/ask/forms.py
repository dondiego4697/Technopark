from django import forms
from django.contrib.auth.models import User
from ask.models import Profile, Question, Tag, Answer
from django.contrib.auth import login
import string
from django.contrib.auth import authenticate


class SignUpForm(forms.Form):
    login = forms.CharField(required=True, max_length=100, error_messages={'blank': 'Enter login'},
                            widget=forms.TextInput(attrs={
                                'class': 'form-control custom-input-text',
                                'placeholder': 'Enter login'
                            }))
    email = forms.EmailField(max_length=100, required=True, widget=forms.EmailInput(attrs={
        'class': 'form-control custom-input-text',
        'placeholder': 'Enter email'
    }), error_messages={'required': 'Enter email'})
    nickname = forms.CharField(required=True, max_length=100, widget=forms.TextInput(attrs={
        'class': 'form-control custom-input-text',
        'placeholder': 'Enter nickname'
    }), error_messages={'required': 'Enter nickname'})
    password = forms.CharField(required=True, widget=forms.PasswordInput(attrs={
        'class': 'form-control custom-input-text',
        'placeholder': 'Enter password'
    }), error_messages={'required': 'Enter password'})

    repeat_password = forms.CharField(required=True, widget=forms.PasswordInput(attrs={
        'class': 'form-control custom-input-text',
        'placeholder': 'Repeat password'
    }), error_messages={'required': 'Repeat password'})

    avatar = forms.FileField(required=False, label='Upload avatar', widget=forms.FileInput(attrs={
        'class': 'filestyle',
        'data-icon': 'false',
        'data-placeholder': 'No file',
        'data-buttonBefore': 'true',
    }))

    def clean(self):
        data = self.cleaned_data
        errors = []
        if Profile.objects.check_user_unicue(data['login']):
            msg = 'This login already used'
            self.add_error('login', msg)
            errors.append({'login': msg})
        if len(data['password']) < 8:
            msg = 'Password is too short. 8 is min'
            self.add_error('password', msg)
            errors.append({'password': msg})
        elif data['password'] != data['repeat_password']:
            msg = 'Passwords not equals'
            self.add_error('password', msg)
            self.add_error('repeat_password', msg)
            errors.append({'password': msg})
            # errors.append({'repeat_password': msg})
        if len(errors) != 0:
            raise forms.ValidationError(errors)
        return data

    def create_user(self):
        user = User.objects.create_user(self.cleaned_data['login'], self.cleaned_data['email'],
                                        self.cleaned_data['password'])
        user.save()
        profile = Profile.objects.create(user=user, nickname=self.cleaned_data['nickname'],
                                         avatar=self.cleaned_data['avatar'])
        profile.save()

    def save(self):
        self.create_user()


class AskForm(forms.Form):
    title = forms.CharField(max_length=100, error_messages={'required': 'Fill title'},
                            widget=forms.TextInput(attrs={
                                'class': 'form-control custom-input-text',
                                'placeholder': 'Enter title'
                            }))
    text = forms.CharField(max_length=1000, error_messages={'required': 'Fill text'},
                           widget=forms.Textarea(attrs={
                               'class': 'form-control custom-textarea-text',
                               'placeholder': 'Enter text',
                               'rows': '10',
                           }))

    tags = forms.CharField(max_length=40, error_messages={'required': 'Fill text'},
                           widget=forms.TextInput(attrs={
                               'class': 'form-control custom-input-text',
                               'placeholder': 'Enter tags with spaces'
                           }))

    def save(self, user):
        data = self.cleaned_data
        tags_names = data['tags'].split()
        tags_objects = []
        for tag_name in tags_names:
            lower_tag_name = tag_name.lower()
            try:
                tag = Tag.objects.get_tag_by_name(lower_tag_name)
            except Tag.DoesNotExist:
                tag = None

            if tag is not None:
                tags_objects.append(tag)
            else:
                new_tag = Tag.objects.create(name=tag_name)
                new_tag.save()
                new_tag = Tag.objects.get_tag_by_name(new_tag.name)
                tags_objects.append(new_tag)

        profile = Profile.objects.get(user=user)
        q = Question.objects.create(user=profile, title=data['title'], text=data['text'])
        for tag_object in tags_objects:
            q.tag.add(tag_object)
        q.save()

        return q.id


class AnswerForm(forms.Form):
    answer = forms.CharField(max_length=1000, error_messages={'required': 'Write your answer'},
                             widget=forms.Textarea(attrs={
                                 'class': 'form-control custom-textarea-text',
                                 'placeholder': 'Enter your answer here',
                                 'rows': '10',
                             }))

    def save(self, user, question):
        data = self.cleaned_data
        profile = Profile.objects.get(user=user)
        a = Answer.objects.create(user=profile, question=question, text=data['answer'])
        a.save()
        return a


class EditProfileForm(forms.Form):
    email = forms.EmailField(max_length=100, required=True, widget=forms.EmailInput(attrs={
        'class': 'form-control custom-input-text',
        'placeholder': 'Enter email'
    }), error_messages={'required': 'Enter email'})
    nickname = forms.CharField(required=True, max_length=100, widget=forms.TextInput(attrs={
        'class': 'form-control custom-input-text',
        'placeholder': 'Enter nickname'
    }), error_messages={'required': 'Enter nickname'})
    avatar = forms.FileField(required=False, label='Upload avatar', widget=forms.FileInput(attrs={
        'class': 'filestyle',
        'data-icon': 'false',
        'data-placeholder': 'No file',
        'data-buttonBefore': 'true',
    }))

    def save(self, user):
        data = self.cleaned_data
        profile = Profile.objects.get(user=user)
        user = User.objects.get(pk=user.id)

        profile.nickname = data['nickname']
        profile.avatar = data['avatar']
        profile.save()

        user.email = data['email']
        user.save()

