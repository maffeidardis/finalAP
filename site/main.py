from configs import APP_URL, APP_DEV_PORT
from app import app

if __name__ == '__main__':
    print(f'Acesse seu site em: http://localhost:{APP_DEV_PORT}{APP_URL}')
    app.run(host='0.0.0.0', port=APP_DEV_PORT, debug=True)