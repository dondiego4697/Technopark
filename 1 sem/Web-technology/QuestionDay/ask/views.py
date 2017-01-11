from django.contrib.auth.decorators import login_required
from django.core.paginator import Paginator, PageNotAnInteger, EmptyPage
from django.http import HttpResponseRedirect
from django.shortcuts import render
from django.views.decorators.csrf import csrf_exempt

from ask.models import Question, Answer, Tag, Profile, Like
from ask.forms import SignUpForm, AskForm, AnswerForm, EditProfileForm
from django.contrib.auth import authenticate, login
from django.contrib.auth import logout
from django.core.urlresolvers import reverse
import json
from django.http import HttpResponse


def get_popular():
    return Tag.objects.get_popular_tags(), Profile.objects.get_popular_users()


def logout_view(request):
    logout(request)
    return HttpResponseRedirect(reverse('login'))


def signup_success(request):
    popular_tags, popular_users = get_popular()
    return render(request, 'signup_success.html', {
        'block_title': 'Success',
        'popular_tags': popular_tags,
        'popular_users': popular_users,
    })


def signup(request):
    popular_tags, popular_users = get_popular()

    try:
        url_continue = request.GET['continue']
    except:
        url_continue = '/'

    if request.method == "POST":
        form = SignUpForm(request.POST, request.FILES)
        if form.is_valid():
            form.save()
            return HttpResponseRedirect(url_continue)
    else:
        form = SignUpForm()
    return render(request, 'signup.html', {
        'form': form,
        'get': url_continue,
        'block_title': 'Sign Up',
        'popular_tags': popular_tags,
        'popular_users': popular_users,
    })


def login_view(request):
    popular_tags, popular_users = get_popular()
    error = ''

    try:
        url_continue = request.GET['continue']
    except:
        url_continue = '/'

    if request.method == "POST":
        username = request.POST['login']
        password = request.POST['password']

        user = authenticate(username=username, password=password)

        if user is not None:
            login(request, user)
            return HttpResponseRedirect(url_continue)
        else:
            error = 'Wrong login or password'

    return render(request, 'login.html', {
        'error': error,
        'get': url_continue,
        'popular_tags': popular_tags,
        'popular_users': popular_users,
    })


def index(request):
    questions = Question.objects.get_new_questions()
    page = paginate(questions, request)
    questions = page
    popular_tags, popular_users = get_popular()

    return render(request, 'index.html', {
        'questions': questions,
        'page': page,
        'block_title': 'Question Day',
        'popular_tags': popular_tags,
        'popular_users': popular_users,
    })


def bytag(request, tag_name):
    popular_tags, popular_users = get_popular()
    questions = Question.objects.get_questions_by_tagname(tag_name)
    page = paginate(questions, request)
    questions = page

    return render(request, 'tag.html', {
        'questions': questions,
        'page': page,
        'tag_name': tag_name,
        'block_title': tag_name,
        'popular_tags': popular_tags,
        'popular_users': popular_users,
    })


def question(request, question_id):
    popular_tags, popular_users = get_popular()

    question = Question.objects.get_question_by_id(question_id)
    answers = Answer.objects.get_answers_for_question_by_id(question_id)
    page = paginate(answers, request)
    answers = page

    if request.method == "POST":
        form = AnswerForm(request.POST)
        if form.is_valid():
            a = form.save(request.user, question)
	    a.notify()
            return HttpResponseRedirect(reverse('question', kwargs={'question_id': question_id}))
    else:
        form = AnswerForm()

    return render(request, 'question.html', {
        'form': form,
        'answers': answers,
        'page': page,
        'question': question,
        'block_title': question.title,
        'popular_tags': popular_tags,
        'popular_users': popular_users,
    })


@login_required(login_url='/login/')
def ask(request):
    popular_tags, popular_users = get_popular()

    if request.method == "POST":
        form = AskForm(request.POST)
        if form.is_valid():
            id = form.save(request.user)
            return HttpResponseRedirect(reverse('question', kwargs={'question_id': id}))
    else:
        form = AskForm()
    return render(request, 'ask.html', {
        'block_title': 'New question',
        'popular_tags': popular_tags,
        'popular_users': popular_users,
        'form': form,
    })


@login_required(login_url='/login/')
def settings(request):
    popular_tags, popular_users = get_popular()

    if request.method == "POST":
        form = EditProfileForm(request.POST, request.FILES)
        if form.is_valid():
            form.save(request.user)
            return HttpResponseRedirect(reverse('index'))
    else:
        form = EditProfileForm(initial={'email': request.user.email, 'nickname': request.user.profile.nickname})

    return render(request, 'settings.html', {
        'block_title': 'Settings',
        'popular_tags': popular_tags,
        'popular_users': popular_users,
        'form': form,
    })


def vote(request):
    if request.method == "POST":
        # if request.is_ajax():
        vote = request.POST['vote']
        type = request.POST['type']
        id = request.POST['id']
        profile = request.user.profile
        if type == "a":
            result, rating = Like.objects.a_like(profile, id, vote)
            data = {"result": result, "rating": rating, "type": type, "id": id}
            return HttpResponse(json.dumps(data), content_type='application/json')
        elif type == "q":
            result, rating = Like.objects.q_like(profile, id, vote)
            data = {"result": result, "rating": rating, "type": type, "id": id}
            return HttpResponse(json.dumps(data), content_type='application/json')
    else:
        data = {'result': 'Error'}
        return HttpResponse(json.dumps(data), content_type='application/json')


def correct(request):
    if request.method == "POST":
        id = request.POST['id']
        value = request.POST['value']
        Answer.objects.set_correct(value, id)
        data = {'result': 'Success'}
        return HttpResponse(json.dumps(data), content_type='application/json')
    else:
        data = {'result': 'Error'}
        return HttpResponse(json.dumps(data), content_type='application/json')


def hot(request):
    questions = Question.objects.get_best_questions()
    page = paginate(questions, request)
    questions = page

    return render(request, 'hot.html', {
        'questions': questions,
        'page': page,
        'block_title': 'Hot',
    })


def paginate(list, request):
    paginator = Paginator(list, 5)
    pageNum = request.GET.get('page')
    try:
        page = paginator.page(pageNum)
        if pageNum != None:
            pageNum = int(pageNum)
    except PageNotAnInteger:
        page = paginator.page(1)
        pageNum = 1
    except EmptyPage:
        page = paginator.page(paginator.num_pages)
        pageNum = paginator.num_pages
    page.page_range_current = get_page_range(pageNum, paginator)
    return page


def get_page_range(current, paginator):
    count = 5
    wing = 2
    if current - wing > 0:
        if current + wing <= paginator.num_pages:
            page_range = range(current - wing, current + wing + 1)
        else:
            page_range = range(current - (count - (paginator.num_pages + 1 - current)), paginator.num_pages + 1)
    else:
        if current + count - 1 <= paginator.num_pages:
            page_range = range(1, count + 1)
        else:
            page_range = range(1, paginator.num_pages + 1)
    return page_range

