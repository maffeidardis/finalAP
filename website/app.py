from flask import Flask, redirect
from configs import APP_URL, SECRET_KEY
from web.routes import web
from web.api import api

print(__name__)
app = Flask(__name__, static_url_path=APP_URL)
app.register_blueprint(web)
app.register_blueprint(api)
app.secret_key = SECRET_KEY