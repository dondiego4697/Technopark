"""question_day URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/1.10/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  url(r'^$', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  url(r'^$', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.conf.urls import url, include
    2. Add a URL to urlpatterns:  url(r'^blog/', include('blog.urls'))
"""
from django.conf.urls import url
from django.contrib import admin
from ask import views

app_name = 'ask'
urlpatterns = [
    url(r'^admin/', admin.site.urls),
    url(r'^signup/$', views.signup, name='signup'),
    url(r'^vote/$', views.vote, name='vote'),
    url(r'^correct/$', views.correct, name='correct'),
    url(r'^tag/(?P<tag_name>[\w#.+-]+)/', views.bytag, name='bytag'),
    url(r'^question/(?P<question_id>[0-9]+)/', views.question, name='question'),
    url(r'^login/', views.login_view, name='login'),
    url(r'^settings/', views.settings, name='settings'),
    url(r'^ask/', views.ask, name='ask'),
    url(r'^hot/', views.hot, name='hot'),
    url(r'^logout/', views.logout_view, name='logout'),
    url(r'^signup_success/', views.signup_success, name='signup_success'),
    url(r'^/?', views.index, name='index'),
]
