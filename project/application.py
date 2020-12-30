import os

from cs50 import SQL
from flask import Flask, Markup, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
import datetime as dt
from wordcloud import WordCloud, STOPWORDS
import matplotlib.pyplot as plt
from PIL import Image

from helpers import apology, login_required, decipher_date_to_int

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///trackers.db")

# Create stocks(portfolio) table
db.execute("""CREATE TABLE IF NOT EXISTS affirmations (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
                                                                 user_id INTEGER NOT NULL,
                                                                 date DATE NOT NULL DEFAULT (CURRENT_DATE),
                                                                 affirmations TEXT NOT NULL,
                                                                 FOREIGN KEY(user_id) REFERENCES users(id))""")

db.execute("""CREATE TABLE IF NOT EXISTS tags (user_id INTEGER NOT NULL,
                                                 affirmation_id INTEGER NOT NULL,
                                                 tags VARCHAR(20) NOT NULL)""")

db.execute("""CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
                                                username TEXT NOT NULL,
                                                hash TEXT NOT NULL)""")

@app.route("/")
@login_required
def index():
    """Show history of logs"""
    grids = db.execute("""SELECT DISTINCT date, COUNT(date) AS streak, STRFTIME('%d/%m/%Y', date) AS formatted_date,
                             strftime('%d', date) AS DAY,
                             strftime('%m', date) AS MONTH,
                             strftime('%Y', date) AS YEAR,
                             affirmations
                             FROM affirmations
                             WHERE user_id = ? AND year = strftime('%Y',date('now'))
                             GROUP BY date""", session["user_id"])
    streaks = []
    leap_year = 2020
    now = dt.datetime.now()
    if (now.year - leap_year) % 4 != 0:
        streaks = [0] * 366
    else:
        streaks = [0] * 367


    for grid in grids:
        deciphered_date = decipher_date_to_int( int(grid["DAY"]), int(grid["MONTH"]), int(grid["YEAR"]))
        streaks[deciphered_date - 1] = grid["streak"]

    texts = db.execute("""SELECT affirmations
                             FROM affirmations
                             WHERE user_id = ? AND strftime('%Y', date) = strftime('%Y',date('now'))""", session["user_id"])

    long_text = ""
    for text in texts:
        long_text = long_text + " " + text["affirmations"]
    print(type(long_text))
    wordcloud = WordCloud(width = 1000, height = 200, random_state=1, background_color='white',
                          colormap='Pastel1', collocations=False, stopwords = STOPWORDS).generate(long_text)

    wordcloud.to_file("static/wordcloud/wordcloud.png")

    return render_template("index.html",
                            length=len(streaks),
                            streaks=streaks,
                            peak=max(streaks))

@app.route("/write", methods=["GET", "POST"])
@login_required
def write():
    """Write new positive affirmation"""
    if request.method == "POST":

        if not request.form.get("text") or len(request.form.get("text")) < 1:
            return apology("too short affirmation", 400)

        affirmation = request.form.get("text")

        if not request.form.get("tags"):
            return apology("miss tags", 400)

        tags = request.form.get("tags").split(",")

        db.execute("""INSERT INTO affirmations (user_id, affirmations) VALUES (?,?)""", session["user_id"], affirmation)

        affirmation_id = db.execute(""" SELECT id FROM affirmations WHERE affirmations = :affirmations""", affirmations=affirmation)

        for tag in tags:
            db.execute("""INSERT INTO tags (user_id, affirmation_id, tags) VALUES (?,?,?)""",
                        session["user_id"], tag, affirmation_id[0]["id"])

        flash('Entry recorded!')
        # Redirect user to home page
        return redirect("/")

    else:
        return render_template("write.html")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        flash('Login successful!')

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    flash('Logged out!')

    # Redirect user to login form
    return redirect("/")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        #elif len(request.form.get("username")) < 8:
        #    return apology("username must include 8 or more characters", 403)

        # Query database for username
        check_username = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username was available
        if len(check_username) != 0:
            return apology("this username is already taken", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        #elif len(request.form.get("password")) < 8:
        #    return apology("password must include 8 or more characters", 403)

        # Ensure both passwords match up
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("Passwords provided do not match up", 400)


        # Add user to database
        db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash_value)",
                          username=request.form.get("username"),
                          hash_value=generate_password_hash(request.form.get("password")))


        flash('Registered!')
        # Redirect user to log in page
        return redirect("/login")

    else:
        return render_template("register.html")

@app.route("/delete", methods=["POST"])
def delete():

    # Remove affimations
    id = request.form.get("id")

    if id:
        db.execute("DELETE FROM affirmations WHERE id = ?", id)
        db.execute("DELETE FROM tags WHERE affirmation_id = ?", id)

    return redirect(request.referrer)


@app.route("/search", methods=["GET", "POST"])
@login_required
def search():
    """search for logs and read"""

    if request.method == "POST":
        select = str(request.form.get("select"))
        if select == "form_all":
            results = db.execute("""SELECT id, affirmations, STRFTIME('%d/%m/%Y', date) AS formatted_date
                                    FROM affirmations WHERE user_id = ? ORDER BY id DESC""", session["user_id"])
            length = len(results)
            return render_template("read.html", results=results, length=length)

        elif select == "form_date":
            if not request.form.get("date"):
                return apology("must provide a date", 400)
            date = request.form.get("date")
            #search date
            results = db.execute("""SELECT id, affirmations, STRFTIME('%d/%m/%Y', date) AS formatted_date
                                    FROM affirmations WHERE user_id = ? and date = ?  ORDER BY id DESC""",
                                    session["user_id"], date)
            length = len(results)
            return render_template("read.html", results=results, length=length)

        elif select == "form_tags":
            if not request.form.get("tags"):
                return apology("must provide a tag", 400)
            tags = request.form.get("tags").split(",")

            #search tags
            affirmations_id_lists = []
            for tag in tags:
                affirmations = db.execute("SELECT affirmation_id AS id FROM tags WHERE user_id = ? and tags = ?",session["user_id"], tag)
                affirmation_id_list = []
                for affirmation in affirmations:
                    affirmation_id_list.append(affirmation["id"])
                affirmations_id_lists.append(affirmation_id_list)
            affirmations_id_set = set.intersection(*[set(list) for list in affirmations_id_lists])
            if len(affirmations_id_set) < 1:
                return render_template("read.html", results=0, length=0)
            id_list = list(affirmations_id_set)
            syntax = "("
            for item in id_list:
                syntax = syntax + str(item) + ","
            syntax = syntax[:-1] + ") user_id = ?"

            query = "SELECT id, affirmations, STRFTIME('%d/%m/%Y', date) AS formatted_date FROM affirmations WHERE id IN " + syntax + "ORDER BY id DESC"
            results = db.execute(query, session["user_id"])
            length = len(results)
            return render_template("read.html", results=results, length=length)

        elif select == "form_word":
            if not request.form.get("word"):
                return apology("must provide a word", 400)
            word = request.form.get("word")
            results = db.execute("""SELECT id, affirmations, STRFTIME('%d/%m/%Y', date) AS formatted_date
                                         FROM affirmations
                                         WHERE user_id = ? and affirmations LIKE '%' || ? || '%' ORDER BY id DESC""",
                                         session["user_id"],
                                         word)
            length = len(results)
            return render_template("read.html", results=results, length=length)


    else:
        return render_template("search.html")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
