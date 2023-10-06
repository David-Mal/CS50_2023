import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    portfolio = db.execute(
        "SELECT * FROM current WHERE user_id IN(?)", session["user_id"]
    )
    user = db.execute("SELECT cash FROM users WHERE id IN(?)", session["user_id"])
    s = 0
    for i in portfolio:
        s += i["price"] * i["shares"]
    cash = "{:.2f}".format(user[0]["cash"])
    s = "{:.2f}".format(s)
    return render_template("index.html", portfolio=portfolio, cash=cash, sum=s)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        if not symbol:
            return apology("No symbol provded", 400)
        elif lookup(symbol) == None:
            return apology("Symbol does not exist", 400)
        if not shares.isnumeric():
            return apology("Must provide a valid number of shares", 400)
        if int(shares) <= 0:
            return apology("Can't buy less than 1 share", 400)
        rows = lookup(symbol)
        price = rows["price"]
        total = price * int(shares)
        curr_price = db.execute(
            "SELECT * FROM users WHERE id = ?", session.get("user_id")
        )
        if curr_price[0]["cash"] < total:
            return apology("Insufficient funds", 400)
        name = curr_price[0]["username"]
        time = datetime.now().strftime("%m/%d/%Y, %H:%M:%S")

        db.execute(
            "INSERT INTO stocks (user_id, symbol, shares, price, time) VALUES (?, ?, ?, ?, ?)",
            session["user_id"],
            symbol,
            shares,
            price,
            time,
        )
        db.execute(
            "UPDATE users SET cash = ? WHERE id IN(?)",
            curr_price[0]["cash"] - total,
            session["user_id"],
        )
        curr = db.execute(
            "SELECT shares FROM current WHERE user_id IN(?) AND symbol IN(?)",
            session["user_id"],
            symbol,
        )

        if len(curr) < 1:
            db.execute(
                "INSERT INTO current (user_id, symbol, shares, price) VALUES(?, ?, ?, ?)",
                session["user_id"],
                symbol,
                shares,
                price,
            )
        else:
            db.execute(
                "UPDATE current SET shares = shares + ? WHERE user_id IN(?) AND symbol IN(?)",
                int(shares),
                session["user_id"],
                symbol,
            )

        flash("Bought!")
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    stocks = db.execute("SELECT * FROM stocks WHERE user_id IN(?)", session["user_id"])
    return render_template("history.html", stocks=stocks)


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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        flash("Logged id!")
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
    if request.method == "GET":
        return render_template("quote.html")
    else:
        stock = request.form.get("symbol")
        if not stock:
            return apology("Must provide stock symbol")
        rows = lookup(stock)
        if not rows:
            return apology("Symbol does not exist", 400)
        name = rows["name"]
        price = "{:.2f}".format(rows["price"])
        symbol = rows["symbol"]
        return render_template("stock.html", name=name, price=price, symbol=symbol)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    else:
        if not request.form.get("username"):
            return apology("must provide username", 400)

        elif not request.form.get("password"):
            return apology("must provide password", 400)

        elif not request.form.get("confirmation"):
            return apology("must confirm your password", 400)

        user = request.form.get("username")
        passw = request.form.get("password")
        conf = request.form.get("confirmation")
        if conf != passw:
            return apology("password and confirmation don't match", 400)
        names = db.execute("SELECT * FROM users WHERE username = ?", user)
        if names:
            return apology("username already in use", 400)
        key = generate_password_hash(passw)
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", user, key)
        rows = db.execute("SELECT * FROM users WHERE username = ?", user)
        session["user_id"] = rows[0]["id"]
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    stocks = db.execute("SELECT * FROM current WHERE user_id IN(?)", session["user_id"])

    if request.method == "GET":
        return render_template("sell.html", stocks=stocks)
    else:
        stock = request.form.get("symbol")
        share = request.form.get("shares")
        limit = db.execute(
            "SELECT * FROM current WHERE user_id = ? AND symbol = ?",
            session["user_id"],
            stock,
        )
        owned_shares = limit[0]["shares"]
        price = limit[0]["price"]
        limit = limit[0]["shares"]
        if not share.isnumeric():
            return apology("Invalid number of shares", 400)
        if int(share) > limit or int(share) < 1:
            return apology("Invalid number of shares", 400)

        time = datetime.now().strftime("%Y-%m-%d, %H:%M:%S")

        current_price = db.execute(
            "SELECT cash FROM users WHERE id IN(?)", session["user_id"]
        )
        current_price = current_price[0]["cash"]

        cost = int(share) * price

        shares = int(share)
        value = -shares

        curr = db.execute(
            "SELECT shares FROM current WHERE user_id IN(?) AND symbol IN(?)",
            session["user_id"],
            stock,
        )
        curr = curr[0]["shares"]
        db.execute(
            "INSERT INTO stocks (user_id, symbol, shares, price, time) VALUES (?, ?, ?, ?, ?)",
            session["user_id"],
            stock,
            value,
            price,
            time,
        )
        db.execute(
            "UPDATE users SET cash = ? WHERE id IN(?)",
            current_price + cost,
            session["user_id"],
        )
        if owned_shares > shares:
            db.execute(
                "UPDATE current SET shares = shares - ? WHERE user_id IN(?) AND symbol IN(?)",
                int(shares),
                session["user_id"],
                stock,
            )
        else:
            db.execute(
                "DELETE FROM current WHERE user_id = ? AND symbol = ?",
                session["user_id"],
                stock,
            )

        flash("Sold!")
        return redirect("/")


@app.route("/add_cash", methods=["GET", "POST"])
@login_required
def add_cash():
    """Add cash into your account"""
    if request.method == "GET":
        return render_template("add_cash.html")
    else:
        value = request.form.get("value")
        value = int(value)
        db.execute(
            "UPDATE users SET cash = cash + ? WHERE id = ?", value, session["user_id"]
        )
        flash("Cash added!")
        return redirect("/")
