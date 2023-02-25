from flask import Blueprint, render_template, request, redirect
from configs import APP_URL
from auxiliar import formataData, validarData, validarLote

web = Blueprint('web', __name__, template_folder='templates', url_prefix=APP_URL)

@web.route('/code')
def code():
    return render_template('main.html')