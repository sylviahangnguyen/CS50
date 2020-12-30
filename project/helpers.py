import os
import requests
import urllib.parse

from flask import redirect, render_template, request, session, flash
from functools import wraps


def apology(message, code=400):
    """Render message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    http://flask.pocoo.org/docs/1.0/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            flash("You must log in to use Habit Tracker.", category="error")
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function

def decipher_date_to_int( day, month, year):
    leap_year = 2020
    days_in_month = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
    days_in_month_leap = [31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]

    if (year - leap_year) % 4 != 0:
        deciphered_date = day + sum(days_in_month[:(month - 1)])
    else:
        deciphered_date = day + sum(days_in_month_leap[:(month - 1)])
    return deciphered_date


