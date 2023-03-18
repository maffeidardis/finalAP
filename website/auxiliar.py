from datetime import datetime as dt

def formataData(string_data):
    data = string_data[0:2] + '/' + string_data[2:]
    return data

def validarData(data):
    data_min = dt.strptime('102021', '%m%Y')
    try:
        data_dttime = dt.strptime(data, '%m%Y')
        if data_dttime >= data_min:
            return True
        else:
            return False
    except:
        return False
def validarLote(lote):
    if len(str(lote)) != 8:
        return False
    try:
        lote = int(lote)
        return True
    except:
        return False