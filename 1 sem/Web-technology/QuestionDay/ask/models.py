from django.contrib.contenttypes.models import ContentType
from django.db import models
from django.contrib.auth.models import User
from django.contrib.contenttypes.fields import GenericForeignKey
from django.http import Http404
from django.db.models import Count
import requests
import json


class QuestionManager(models.Manager):
    def get_question_by_id(self, q_id):
        try:
            q = self.get(pk=q_id)
            q.tag_list = self.get_tags_for_question_by_id(q.id)
            q.answers_count = self.get_count_of_answers(q.id)
        except Question.DoesNotExist:
            q = None
            raise Http404("Question does not exist")
        return q

    def get_new_questions(self):
        q_list = self.prefetch_related('user').order_by('-pub_date')[:100]
        return Question.objects.fill_questions(q_list)

    def get_best_questions(self):
        q_list = self.order_by('-rating')[:100]
        return Question.objects.fill_questions(q_list)

    def get_count_of_answers(self, q_id):
        return AnswerManager.get_count_of_answers_for_question(Answer.objects, q_id)

    def get_answers_for_question_by_id(self, q_id):
        return AnswerManager.get_answers_for_question_by_id(Answer.objects, q_id)

    def get_tags_for_question_by_id(self, q_id):
        return TagManager.get_tags_for_question_by_id(Tag.objects, q_id)

    def get_questions_by_tagname(self, tag_name):
        q_list = self.filter(tag__name=tag_name)
        return Question.objects.fill_questions(q_list)

    def fill_questions(self, q_list):
        for q in q_list:
            q.tag_list = self.get_tags_for_question_by_id(q.id)
            q.answers_count = self.get_count_of_answers(q.id)
        return q_list


class AnswerManager(models.Manager):
    def str_to_bool(self, s):
        if s == 'true':
            return True
        elif s == 'false':
            return False
        else:
            raise ValueError

    def get_count_of_answers_for_question(self, q_id):
        return self.filter(question=q_id).count()

    def get_answers_for_question_by_id(self, q_id):
        return self.filter(question=q_id).order_by('-pub_date')[:100]

    def get_answer_by_id(self, id):
        return self.get(id=id)

    def set_correct(self, value, answer_id):
        a = Answer.objects.get(pk=answer_id)
        a.correct = self.str_to_bool(value)
        a.save()
   

class TagManager(models.Manager):
    def get_tags_for_question_by_id(self, q_id):
        return self.filter(question=q_id)

    def get_popular_tags(self):
        return self.annotate(Count('question')).order_by('-question__count')[:10]

    def get_tag_by_name(self, tag_name):
        return self.get(name=tag_name)


class ProfileManager(models.Manager):
    def get_popular_users(self):
        return self.annotate(Count('question')).order_by('-question__count')[:10]

    def check_user_unicue(self, login):
        return User.objects.filter(username=login).exists()

    def get_profile_by_user(self, user_id):
        return self.get(user=user_id)


class LikeManager(models.Manager):
    def str_to_bool(self, s):
        if s == 'True':
            return True
        elif s == 'False':
            return False
        else:
            raise ValueError

    def check_a_like(self, user_id, answer_id):
        try:
            ct = ContentType.objects.get(app_label="ask", model="answer")
            check = self.get(user=user_id, content_type=ct, object_id=answer_id).like
        except Like.DoesNotExist:
            check = None
        return check

    def check_q_like(self, user_id, question_id):
        try:
            ct = ContentType.objects.get(app_label="ask", model="question")
            check = self.get(user=user_id, content_type=ct, object_id=question_id).like
        except Like.DoesNotExist:
            check = None
        return check

    def a_like(self, user_id, answer_id, like0):
        like = self.str_to_bool(like0)
        answer = Answer.objects.get_answer_by_id(answer_id)
        rating = answer.rating

        check = self.check_a_like(user_id, answer_id)
        if check is None:
            ct = ContentType.objects.get(app_label="ask", model="answer")
            l = Like.objects.create(user=user_id, like=like, content_type=ct, object_id=answer_id)
            l.save()
            if like:
                rating += 1
            else:
                rating -= 1
            answer.rating = rating
            answer.save()
            return 'Success', rating
        else:
            if check == like:
                return 'Already', rating
            else:
                ct = ContentType.objects.get(app_label="ask", model="answer")
                l = self.get(user=user_id, content_type=ct, object_id=answer_id)
                l.like = like
                l.save()

                if like == True:
                    rating += 2
                else:
                    rating -= 2
                answer.rating = rating
                answer.save()

                return 'Success', rating

    def q_like(self, user_id, question_id, like0):
        like = self.str_to_bool(like0)
        question = Question.objects.get_question_by_id(question_id)
        rating = question.rating

        check = self.check_q_like(user_id, question_id)
        if check is None:
            ct = ContentType.objects.get(app_label="ask", model="question")
            l = Like.objects.create(user=user_id, like=like, content_type=ct, object_id=question_id)
            l.save()
            if like:
                rating += 1
            else:
                rating -= 1
            question.rating = rating
            question.save()
            return 'Success', rating
        else:
            if check == like:
                return 'Already', rating
            else:
                ct = ContentType.objects.get(app_label="ask", model="question")
                l = self.get(user=user_id, content_type=ct, object_id=question_id)
                l.like = like
                l.save()

                if like:
                    rating += 2
                else:
                    rating -= 2
                question.rating = rating
                question.save()

                return 'Success', rating


# ______________MODELS______________#


class Question(models.Model):
    user = models.ForeignKey('Profile')
    title = models.CharField(max_length=255)
    text = models.TextField()
    rating = models.IntegerField(default=0)
    pub_date = models.DateTimeField(auto_now_add=True)
    tag = models.ManyToManyField('Tag')
    objects = QuestionManager()

    def __str__(self):
        return "{} {} {} {}".format(self.id, self.title, self.rating, self.pub_date)


class Answer(models.Model):
    user = models.ForeignKey('Profile')
    question = models.ForeignKey('Question')
    text = models.TextField()
    rating = models.IntegerField(default=0)
    correct = models.BooleanField(default=False)
    pub_date = models.DateTimeField(auto_now_add=True)
    objects = AnswerManager()

    def notify(self):
        data = {'id': self.id, 'user': self.user.id, 'text': self.text, 'rating': self.rating, 'correct': self.correct, }
        requests.post('http://question-day.io/pub?id=1', data=json.dumps(data))

    def __str__(self):
        return "{} {}".format(self.question.title, self.pub_date)


class Tag(models.Model):
    name = models.CharField(max_length=255)
    objects = TagManager()

    def __str__(self):
        return "{}".format(self.name)


class Profile(models.Model):
    user = models.OneToOneField(User)
    nickname = models.CharField(max_length=255)
    avatar = models.ImageField(upload_to='uploads', null=True, blank=True)
    objects = ProfileManager()

    def __str__(self):
        return "{}".format(self.nickname)


class Like(models.Model):
    content_type = models.ForeignKey(ContentType, related_name="content_type_likes")
    object_id = models.PositiveIntegerField()
    content_object = GenericForeignKey('content_type', 'object_id')

    user = models.ForeignKey('Profile')
    like = models.BooleanField()
    objects = LikeManager()

    def __str__(self):
        return "{} {} {} {} {}".format(self.content_type, self.user, self.object_id, self.content_object, self.like)

