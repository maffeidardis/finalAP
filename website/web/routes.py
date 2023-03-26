from flask import Blueprint, render_template, request, redirect
from configs import APP_URL
from auxiliar import formataData, validarData, validarLote
import os

web = Blueprint('web', __name__, template_folder='templates', url_prefix=APP_URL)

@web.route('')
def code():
    print(os.getcwd())
    listdir = os.listdir('./static/images')
    list_dict_infos = []
    #Iterating through all folders
    for file in listdir:
        dir = os.path.join('./static/images', file)
        #Checking if it's a subfolder
        if os.path.isdir(dir):
            dict_infos = {}
            for subfile in os.listdir(dir):
                if subfile.endswith('.jpg'):
                    dict_infos['img'] = os.path.join(dir.replace('./static/', '/espci/'), subfile)
                if subfile.endswith('.txt'):
                    txt = os.path.join(dir, subfile)
                    with open(txt) as f:
                        list_lines = [line for line in f.readlines()]
                    dict_infos['txt'] = list_lines
            list_dict_infos.append(dict_infos)
    print(list_dict_infos)
    return render_template('main.html', list_dict_infos = list_dict_infos)