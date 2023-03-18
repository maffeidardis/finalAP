from flask import Blueprint, request, jsonify, flash
from configs import APP_URL

api = Blueprint('api', __name__, url_prefix=f'{APP_URL}/api')

@api.route('/getform', methods=['POST'])
def getForm():
    cpf = request.form.get('cpf')
    nome = request.form.get('nome')
    # if nome is not None:
    #     flash(f'Olá, {nome}. Seja bem vindo a uma nova fase da indústria farmacêutica.', 'info')
    return jsonify({'msg': 'Testando'}), 200