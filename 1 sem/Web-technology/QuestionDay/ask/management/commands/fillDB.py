from django.core.management.base import BaseCommand, CommandError
from ask.fill_db import FillDB


class Command(BaseCommand):
    def handle(self, *args, **options):
        fillDB = FillDB()
        fillDB.ask_with_answers()

