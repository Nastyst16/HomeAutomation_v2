from flask import Flask, render_template_string, request, redirect, url_for
import serial
import threading

app = Flask(__name__)

# Configurăm portul serial
ser = serial.Serial('/dev/ttyACM1', 9600, timeout=1)

# Variabilă globală pentru stocarea mesajelor
messages = []

# Funcție pentru citirea mesajelor din UART
def read_from_uart():
    global messages
    while True:
        if ser.in_waiting > 0:
            message = ser.readline().decode('utf-8').strip()
            if message:
                messages.append(message)
                # Limităm numărul de mesaje pentru a evita utilizarea excesivă a memoriei
                if len(messages) > 100:
                    messages.pop(0)

# Pornim thread-ul pentru citirea de la UART
uart_thread = threading.Thread(target=read_from_uart)
uart_thread.daemon = True
uart_thread.start()

# Pagina web pentru a afișa mesajele și controla LED-ul
@app.route('/')
def index():
    return render_template_string('''
        <!doctype html>
        <html lang="en">
        <head>
            <meta charset="utf-8">
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <title>Control LED UART</title>
        </head>
        <body>
            <h1>Mesaje UART</h1>
            <ul>
                {% for message in messages %}
                    <li>{{ message }}</li>
                {% endfor %}
            </ul>
            <h2>Control LED</h2>
            <form action="/led/on" method="post">
                <button type="submit">Aprinde LED</button>
            </form>
            <form action="/led/off" method="post">
                <button type="submit">Stinge LED</button>
            </form>
        </body>
        </html>
    ''', messages=messages)

@app.route('/led/on', methods=['POST'])
def led_on():
    ser.write(b'1\n')  # Trimitere comanda de aprindere a LED-ului
    return redirect(url_for('index'))

@app.route('/led/off', methods=['POST'])
def led_off():
    ser.write(b'0\n')  # Trimitere comanda de stingere a LED-ului
    return redirect(url_for('index'))

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
