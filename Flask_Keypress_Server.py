from flask import Flask, request
import pyautogui
import socket

app = Flask(__name__)


def get_ip():
    # Bilgisayarın IP adresini al
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.connect(('10.255.255.255', 1))
        IP = s.getsockname()[0]
    except Exception:
        IP = '127.0.0.1'
    finally:
        s.close()
    return IP


@app.route('/receiveData', methods=['POST'])
def receive_data():
    if request.form.get('buttonPressed') == 'true':
        pyautogui.press("a")
        print("Buton basıldı - 'a' tuşuna basıldı!")
        return "Tuşa basma işlemi başarılı!", 200
    return "Geçersiz veri!", 400


if __name__ == '__main__':
    server_ip = get_ip()
    print(f"Server IP: {server_ip}")
    app.run(host=server_ip, port=5000, debug=True)
