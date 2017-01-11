from django.contrib import admin

from ask.models import Answer, Tag, Question, Profile, Like


class AnswerInline(admin.TabularInline):
    model = Answer
    extra = 1


class QuestionAdmin(admin.ModelAdmin):
    inlines = [AnswerInline]
    list_display = ('user', 'title', 'text', 'rating', 'pub_date')


class ProfileAdmin(admin.ModelAdmin):
    list_display = ('user', 'nickname')


class AnswerAdmin(admin.ModelAdmin):
    list_display = ('user', 'question', 'text', 'rating', 'correct', 'pub_date')


class TagAdmin(admin.ModelAdmin):
    list_display = ('name',)


class LikeAdmin(admin.ModelAdmin):
    list_display = ('content_type', 'user', 'object_id', 'content_object', 'like')


admin.site.register(Profile, ProfileAdmin)
admin.site.register(Question, QuestionAdmin)
admin.site.register(Answer, AnswerAdmin)
admin.site.register(Tag, TagAdmin)
admin.site.register(Like, LikeAdmin)

