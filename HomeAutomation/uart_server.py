from flask import Flask, render_template, request, redirect, url_for
import serial
import threading

app = Flask(__name__)

# Configurăm portul serial
try:
    ser = serial.Serial('/dev/ttyACM1', 9600, timeout=1)
    print("Conexiune serial deschisă")
except Exception as e:
    print(f"Eroare la deschiderea portului serial: {e}")
    ser = None

# Variabilă globală pentru stocarea mesajelor și a datelor de temperatură/umiditate
messages = []
temperature = "N/A"
humidity = "N/A"

# Funcție pentru citirea mesajelor din UART
def read_from_uart():
    global messages, temperature, humidity
    if ser:
        while True:
            if ser.in_waiting > 0:
                try:
                    message = ser.readline().decode('utf-8').strip()
                except UnicodeDecodeError:
                    message = ser.readline().decode('latin1').strip()  # Use a fallback encoding
                if message:
                    messages.append(message)
                    # Limităm numărul de mesaje pentru a evita utilizarea excesivă a memoriei
                    if len(messages) > 100:
                        messages.pop(0)

                    # Verificăm dacă mesajul conține date de temperatură și umiditate
                    if "Temp:" in message and "Hum:" in message:
                        try:
                            parts = message.split(',')
                            temp_part = parts[0].split(':')[1].strip()
                            hum_part = parts[1].split(':')[1].strip()
                            temperature = temp_part
                            humidity = hum_part
                        except Exception as e:
                            print(f"Eroare la parsarea mesajului: {e}")

# Pornim thread-ul pentru citirea de la UART
if ser:
    uart_thread = threading.Thread(target=read_from_uart)
    uart_thread.daemon = True
    uart_thread.start()

# Pagina web pentru a afișa mesajele, datele de temperatură/umiditate și controla LED-ul
@app.route('/')
def index():
    return render_template('index.html', messages=messages, temperature=temperature, humidity=humidity)

@app.route('/led/on', methods=['POST'])
def led_on():
    if ser:
        ser.write(b'1\n')
        print("Comanda LED ON trimisă")
    else:
        print("Portul serial nu este deschis")
    return redirect(url_for('index'))

@app.route('/led/off', methods=['POST'])
def led_off():
    if ser:
        ser.write(b'0\n')
        print("Comanda LED OFF trimisă")
    else:
        print("Portul serial nu este deschis")
    return redirect(url_for('index'))

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)