import network
import socket
import time
from machine import Pin, I2C
import ssd1306

# Configuración de la pantalla OLED (usando tu código)
i2c = I2C(0, scl=Pin(23), sda=Pin(22), freq=400000)
oled = ssd1306.SSD1306_I2C(128, 64, i2c)

# LED integrado para mostrar estado
led = Pin(2, Pin.OUT)

# Función para mostrar texto (usando tu función)
def mostrar_texto(texto):
    oled.fill(0)  # Limpiar pantalla
    oled.text(texto, 0, 30)
    oled.show()

# Configuración WiFi
def connect_wifi():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    
    ssid = 'Mega-2.4G-98B1'  # Reemplaza con el nombre de tu red WiFi
    password = '2TW2FpZpdS'  # Reemplaza con tu contraseña WiFi
    
    if not wlan.isconnected():
        mostrar_texto("Conectando WiFi")
        wlan.connect(ssid, password)
        
        # Esperar a la conexión
        max_wait = 10
        while max_wait > 0:
            if wlan.isconnected():
                break
            max_wait -= 1
            print('Esperando conexion...')
            time.sleep(1)
    
    if wlan.isconnected():
        ip = wlan.ifconfig()[0]
        mostrar_texto('Fine')
        return ip
    else:
        mostrar_texto("Error WiFi")
        return None

# Crear un servidor web
def web_server(ip):
    # Crear socket
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(('', 80))
    s.listen(5)
    
    mostrar_texto(ip)
    
    # HTML para la página web
    html = """<!DOCTYPE html>
    <html>
        <head>
            <title>Control ESP32</title>
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <style>
                body { font-family: Arial; text-align: center; margin:0; padding:20px; }
                .button { background-color: #4CAF50; color: white; padding: 15px 32px;
                         text-align: center; display: inline-block; font-size: 16px;
                         margin: 10px; cursor: pointer; border-radius: 5px; }
                .button.off { background-color: #f44336; }
            </style>
        </head>
        <body>
            <h1>ESP32 Control</h1>
            <p>Estado del LED: <strong id="estado">%s</strong></p>
            <a href="/on" class="button">ENCENDER</a>
            <a href="/off" class="button off">APAGAR</a>
        </body>
    </html>
    """
    
    while True:
        try:
            # Aceptar conexión
            conn, addr = s.accept()
            print('Cliente conectado desde:', addr)
            
            # Recibir solicitud
            request = conn.recv(1024)
            request = str(request)
            
            # Verificar acciones
            led_on = request.find('/on') != -1
            led_off = request.find('/off') != -1
            
            # Cambiar estado del LED según la solicitud
            if led_on:
                led.value(1)
                mostrar_texto("LED: ON")
            if led_off:
                led.value(0)
                mostrar_texto("LED: OFF")
                
            # Preparar respuesta
            estado = "ENCENDIDO" if led.value() else "APAGADO"
            response = html % estado
            
            # Enviar respuesta
            conn.send('HTTP/1.1 200 OK\n')
            conn.send('Content-Type: text/html\n')
            conn.send('Connection: close\n\n')
            conn.sendall(response)
            
            # Cerrar conexión
            conn.close()
            
        except Exception as e:
            print("Error:", e)
            break

# Programa principal
if __name__ == "__main__":
    ip = connect_wifi()
    if ip:
        web_server(ip)