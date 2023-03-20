from flask import Blueprint, render_template, request, redirect
from configs import APP_URL
from auxiliar import formataData, validarData, validarLote
import os

web = Blueprint('web', __name__, template_folder='templates', url_prefix=APP_URL)

@web.route('/code')
def code():
    listdir = os.listdir('./website/static/images')
    list_dict_infos = []
    #Iterating through all folders
    for file in listdir:
        dir = os.path.join('./website/static/images', file)
        #Checking if it's a subfolder
        if os.path.isdir(dir):
            dict_infos = {}
            for subfile in os.listdir(dir):
                #If it's an image, append path to list_images, 
                #If it's a txt, append path to list_txt
                if subfile.endswith('.jpg'):
                    dict_infos['img'] = os.path.join(dir.replace('./website/static/', '/espci/'), subfile)
                    #list_images.append(os.path.join(dir.replace('./website/static/', '/espci/'), subfile))
                if subfile.endswith('.txt'):
                    txt = os.path.join(dir, subfile)
                    with open(txt) as f:
                        list_lines = [line for line in f.readlines()]
                    dict_infos['txt'] = list_lines
            list_dict_infos.append(dict_infos)
    print(list_dict_infos)
    return render_template('main.html', list_dict_infos = list_dict_infos)