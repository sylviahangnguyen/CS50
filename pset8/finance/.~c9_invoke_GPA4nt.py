import os

from cs50 import SQL
from flask import Flask, Markup, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, have_symbol


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

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Create stocks(portfolio) table
db.execute("""CREATE TABLE IF NOT EXISTS portfolios (user_id INTEGER NOT NULL,
                                                 symbol TEXT PRIMARY KEY NOT NULL,
                                                 name TEXT NOT NULL,
                                                 price NUMERIC NOT NULL,
                                                 shares INTEGER NOT NULL,
                                                 total_price NUMERIC NOT NULL,
                                                 total_expense NUMERIC NOT NULL,
                                                 FOREIGN KEY(user_id) REFERENCES users(id))""")

# Create transactions table
db.execute("""CREATE TABLE IF NOT EXISTS transactions (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
                                                      datetime DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
                                                      user_id INTEGER NOT NULL,
                                                      symbol TEXT NOT NULL,
                                                      price NUMERIC NOT NULL,
                                                      amount_buy INTEGER,
                                                      amount_sell INTEGER,
                                                      total_buy NUMERIC,
                                                      total_sell NUMERIC,
                                                      FOREIGN KEY(user_id) REFERENCES users(id))""")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():

    """Show portfolio of stocks"""
    portfolios = db.execute("""SELECT * FROM portfolios WHERE user_id = :user_id""",
                                        user_id=session["user_id"])
    total_total_price = db.execute("""SELECT TOTAL(total_price) AS current_value FROM portfolios WHERE user_id = :user_id""",
                                        user_id=session["user_id"])
    total_total_expense = db.execute("""SELECT TOTAL(total_expense) AS expense FROM portfolios WHERE user_id = :user_id""",
                                        user_id=session["user_id"])

    if total_total_expense == 0:
        remaining_cash = 10000
    remaining_cash = 10000 - total_total_expense[0]["expense"]

    if total_total_price == 0:
        asset = remaining_cash
    asset = remaining_cash + total_total_price[0]["current_value"]

    list_stocks = db.execute("""SELECT symbol, shares FROM portfolios WHERE user_id = :user_id""",
                                        user_id=session["user_id"])

    for stock in list_stocks:
        stock_info = lookup(stock["symbol"])
        price = float(stock_info["price"])


        # update the current price for each stock
        db.execute("""UPDATE portfolios SET total_price = :total_price
                                        WHERE user_id = :user_id AND symbol = :symbol""",
                                        user_id=session["user_id"],
                                        symbol=stock["symbol"],
                                        total_price=price * stock["shares"])


    return render_template("index.html", portfolios=portfolios, remaining_cash=usd(remaining_cash), asset=usd(asset))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        if not request.form.get("shares") or int(request.form.get("shares")) <= 0:
            return apology("no. of shares must be greater than 0", 403)

        symbol = request.form.get("symbol").upper()
        stock_info = lookup(symbol)

        if not symbol or stock_info == None:
            return apology("invalid symbol", 403)

        name = stock_info["name"]
        price = float(stock_info["price"])

        shares = int(request.form.get("shares"))
        expense = shares * price
        cash = db.execute("SELECT cash FROM users WHERE id = :user_id",
                  user_id=session["user_id"])

        if cash[0]["cash"] < expense:
            return apology("not enough cash", 403)

        user_id = session["user_id"]

        # Add transaction to database
        db.execute("""INSERT INTO transactions (user_id, symbol, price, amount_buy, total_buy)
                                  VALUES(?,?,?,?,?)""", user_id, symbol, price, shares, expense)

        current_symbols = db.execute("""SELECT DISTINCT symbol FROM portfolios""")

        if have_symbol(current_symbols, symbol) == False:
            # if the symbol is not in portfolio
            db.execute("""INSERT INTO portfolios (user_id, symbol, name, price, shares, total_expense, total_price)
                                      VALUES (?,?,?,?,?,?,?)""",user_id, symbol, name, price, shares, expense, expense)
        else:
            # if the symbol is in portfolio
            db.execute("""UPDATE portfolios SET price = :price,
                                                shares = shares + :shares,
                                                total_expense = total_expense + :expense
                                            WHERE user_id = :user_id AND symbol = :symbol""",
                                            user_id=user_id,
                                            symbol=symbol,
                                            price=price,
                                            shares=shares,
                                            expense=expense)

        total_shares = db.execute("""SELECT shares FROM portfolios WHERE user_id = :user_id AND symbol = :symbol""",
                                            user_id=user_id,
                                            symbol=symbol)
        total_shares = total_shares[0]["shares"]
        # update the current price for portfolios
        db.execute("""UPDATE portfolios SET total_price = :total_price
                                        WHERE user_id = :user_id AND symbol = :symbol""",
                                        user_id=user_id,
                                        symbol=symbol,
                                        total_price=price*total_shares)


        # Redirect user to home page
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("""SELECT * FROM transactions WHERE user_id = :user_id""",
                                        user_id=session["user_id"])

    return render_template("history.html", transactions=transactions)


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

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        stock_info = lookup(symbol)
        if stock_info == None:
            return apology("invalid symbol", 403)
        return render_template("quoted.html", name=stock_info["name"],
                                                  symbol=stock_info["symbol"],
                                                  price=stock_info["price"])
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("new_username"):
            return apology("must provide username", 403)

        #elif len(request.form.get("username")) < 8:
        #    return apology("username must include 8 or more characters", 403)

        # Query database for username
        check_username = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("new_username"))

        # Ensure username was available
        if len(check_username) != 0:
            return apology("this username is already taken", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        #elif len(request.form.get("password")) < 8:
        #    return apology("password must include 8 or more characters", 403)

        # Ensure both passwords match up
        elif request.form.get("password") != request.form.get("password_confirm"):
            return apology("Passwords provided do not match up", 403)


        # Add user to database
        db.execute("INSERT INTO users ( username, hash, cash) VALUES(:username, :hash_value, :cash)",
                          username=request.form.get("new_username"),
                          hash_value=generate_password_hash(request.form.get("password")),
                          cash=10000.00)


        #flash(Markup("""Successfully registered, please click <a href="/login" class="alert-link">here</a>
        #to login, or wait until the log in page loads"""))

        # Redirect user to log in page
        return redirect("/login")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    stocks = db.execute("""SELECT DISTINCT symbol FROM portfolios""")
    STOCKS = []
    for stock in stocks:
        STOCKS.append(stock["symbol"])

    if request.method == "POST":

        symbol = request.form.get("symbol")
        if not symbol or symbol not in STOCKS:
            return apology("invalid symbol", 403)

        shares_demand = int(request.form.get("shares"))
        shares_supply = db.execute("""SELECT shares FROM portfolios WHERE symbol = :symbol""", symbol=symbol)
        if not shares_demand or int(shares_demand) <= 0:
            return apology("no. of shares must be greater than 0", 403)
        if int(shares_demand) > shares_supply[0]["shares"]:
            return apology("not enough shares to sell", 403)

        stock_info = lookup(symbol)
        price = float(stock_info["price"])

        revenue = shares_demand * price

        user_id = session["user_id"]

        # Add transaction to database
        db.execute("""INSERT INTO transactions (user_id, symbol, price, amount_sell, total_sell)
                                  VALUES(?,?,?,?,?)""", user_id, symbol, price, shares_demand, revenue)

        db.execute("""UPDATE portfolios SET price = :price,
                                            shares = shares - :shares,
                                            total_expense = total_expense - :revenue
                                        WHERE user_id = :user_id AND symbol = :symbol""",
                                        user_id=user_id,
                                        symbol=symbol,
                                        price=price,
                                        shares=shares_demand,
                                        revenue=revenue)

        total_shares = db.execute("""SELECT shares FROM portfolios WHERE user_id = :user_id AND symbol = :symbol""",
                                            user_id=user_id,
                                            symbol=symbol)
        total_shares = total_shares[0]["shares"]

        # update the current price for portfolios
        db.execute("""UPDATE portfolios SET total_price = :total_price
                                        WHERE user_id = :user_id AND symbol = :symbol""",
                                        user_id=user_id,
                                        symbol=symbol,
                                        total_price=price*total_shares)

        # delete the stock that is no longer in portfolios
        db.execute("""DELETE FROM portfolios WHERE shares = 0""")


        # Redirect user to home page
        return redirect("/")

    else:
        return render_template("sell.html", stocks=STOCKS)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
