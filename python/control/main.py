import network
import socket
import json
import time
from machine import Pin, I2C
import ssd1306

# Configuración de la pantalla OLED
i2c = I2C(0, scl=Pin(23), sda=Pin(22), freq=400000)
oled = ssd1306.SSD1306_I2C(128, 64, i2c)

# LED indicador
led = Pin(2, Pin.OUT)

# Función para mostrar texto en la OLED
def mostrar_texto(texto, x=0, y=0, clear=True):
    if clear:
        oled.fill(0)  # Limpiar pantalla
    oled.text(texto, x, y)
    oled.show()

# Función para mostrar botones presionados
def mostrar_botones(botones_presionados):
    oled.fill(0)  # Limpiar pantalla
    
    # Mostrar título
    mostrar_texto("Gamepad", 32, 0, False)
    mostrar_texto("Botones:", 0, 16, False)
    
    # Mostrar botones presionados
    y_pos = 26
    if not botones_presionados:
        mostrar_texto("- Ninguno -", 10, y_pos, False)
    else:
        for i, boton in enumerate(botones_presionados):
            if i < 4:  # Limitamos a 4 botones en pantalla
                mostrar_texto(f"- {boton}", 10, y_pos + (i * 10), False)
    
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
            print('Esperando conexión...')
            time.sleep(1)
    
    if wlan.isconnected():
        ip = wlan.ifconfig()[0]
        mostrar_texto(f"IP: {ip}")
        return ip
    else:
        mostrar_texto("Error WiFi")
        return None

# Extraer JSON de una solicitud HTTP POST
def extract_json(request):
    try:
        # Buscar doble retorno de carro que marca el inicio del body
        pos = request.find(b'\r\n\r\n')
        if pos > 0:
            # Obtener body (parte después del encabezado)
            body = request[pos + 4:]
            # Convertir a string y eliminar caracteres no deseados
            body_str = body.decode('utf-8').strip()
            
            # Intentar parsear como JSON
            if body_str:
                try:
                    return json.loads(body_str)
                except ValueError as e:
                    print("Error al analizar JSON:", e)
                    print("Contenido del body:", body_str)
            else:
                print("Body vacío")
        else:
            print("No se encontró el separador de encabezado/body")
    except Exception as e:
        print("Error al extraer JSON:", e)
    
    return None

# Servidor web para recibir datos del gamepad
def gamepad_server(ip):
    # Crear socket
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(('', 80))
    s.listen(5)
    
    # Mostrar IP para conectar la app
    mostrar_texto("Gamepad Server", 0, 0)
    mostrar_texto(f"IP: {ip}", 0, 20)
    mostrar_texto("Esperando app...", 0, 40)
    
    print(f"Servidor iniciado en http://{ip}")
    
    # Página web básica para verificar que el servidor está funcionando
    html = """<!DOCTYPE html>
    <html>
        <head>
            <title>ESP32 Gamepad Server</title>
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <style>
                body { font-family: Arial; text-align: center; padding: 20px; }
            </style>
        </head>
        <body>
            <h1>ESP32 Gamepad Server</h1>
            <p>Servidor activo. Usa la app React Native para conectarte.</p>
        </body>
    </html>
    """
    
    # Mantener el servidor funcionando
    while True:
        try:
            # Aceptar conexión
            conn, addr = s.accept()
            print('Cliente conectado desde:', addr)
            led.value(1)  # Encender LED al recibir conexión
            
            # Recibir datos
            request = conn.recv(1024)
            request_str = str(request)
            
            # Verificar el tipo de solicitud
            if 'POST /gamepad' in request_str:
                print("Recibida solicitud POST /gamepad")
                
                # Extraer JSON del cuerpo de la solicitud
                json_data = extract_json(request)
                
                if json_data:
                    # Acceder a la lista de botones
                    botones_presionados = json_data.get('buttons', [])
                    print("Botones presionados:", botones_presionados)
                    
                    # Verificar si hay un ping de prueba
                    if 'TEST_PING' in botones_presionados:
                        mostrar_texto("TEST PING", 20, 30)
                        print("Recibido ping de prueba")
                    else:
                        # Mostrar en la pantalla OLED
                        mostrar_botones(botones_presionados)
                    
                    # Enviar respuesta de éxito
                    conn.send(b'HTTP/1.1 200 OK\r\n')
                    conn.send(b'Content-Type: application/json\r\n')
                    conn.send(b'Access-Control-Allow-Origin: *\r\n')
                    conn.send(b'Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n')
                    conn.send(b'Access-Control-Allow-Headers: Content-Type\r\n')
                    conn.send(b'Connection: close\r\n\r\n')
                    conn.send(b'{"status":"ok"}')
                else:
                    print("JSON no encontrado o inválido")
                    conn.send(b'HTTP/1.1 400 Bad Request\r\n')
                    conn.send(b'Content-Type: text/plain\r\n')
                    conn.send(b'Connection: close\r\n\r\n')
                    conn.send(b'Invalid JSON data')
            
            # Si es una solicitud HTTP normal, devolver la página web básica
            elif 'GET /' in request_str:
                print("Recibida solicitud GET /")
                conn.send(b'HTTP/1.1 200 OK\r\n')
                conn.send(b'Content-Type: text/html\r\n')
                conn.send(b'Connection: close\r\n\r\n')
                conn.send(html.encode('utf-8'))
            
            # Agregar soporte para CORS en solicitudes OPTIONS (preflight)
            elif 'OPTIONS' in request_str:
                print("Recibida solicitud OPTIONS (CORS preflight)")
                conn.send(b'HTTP/1.1 200 OK\r\n')
                conn.send(b'Access-Control-Allow-Origin: *\r\n')
                conn.send(b'Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n')
                conn.send(b'Access-Control-Allow-Headers: Content-Type\r\n')
                conn.send(b'Connection: close\r\n\r\n')
            
            # Cualquier otra solicitud
            else:
                print("Solicitud no reconocida:", request_str[:50])
                conn.send(b'HTTP/1.1 404 Not Found\r\n')
                conn.send(b'Connection: close\r\n\r\n')
            
            # Cerrar conexión
            conn.close()
            led.value(0)  # Apagar LED después de procesar
            
        except Exception as e:
            print("Error en el servidor:", e)
            led.value(0)
            time.sleep(0.1)

# Programa principal
if __name__ == "__main__":
    ip = connect_wifi()
    if ip:
        gamepad_server(ip)
    else:
        while True:
            mostrar_texto("ERROR WiFi")
            time.sleep(1)
            mostrar_texto("Reinicia ESP32")
            time.sleep(1)