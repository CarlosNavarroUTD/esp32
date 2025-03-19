import network
import time
#import os
#from dotenv import load_dotenv

def connect_wifi(ssid, password):
    # Crear objeto de red WiFi
    wlan = network.WLAN(network.STA_IF)
    
    # Activar la interfaz WiFi
    wlan.active(True)
    
    # Comprobar si ya está conectado
    if not wlan.isconnected():
        print('Conectando a la red...')
        
        # Conectar a la red WiFi
        wlan.connect(ssid, password)
        
        # Esperar hasta que se conecte o falle
        max_wait = 10
        while max_wait > 0:
            if wlan.isconnected():
                break
            max_wait -= 1
            print('Esperando conexion...')
            time.sleep(1)
    
    # Verificar la conexión
    if wlan.isconnected():
        # Obtener información de la conexión
        ip, subnet, gateway, dns = wlan.ifconfig()
        print(f'Conectado a {ssid}')
        print(f'Dirección IP: {ip}')
        return True
    else:
        print('Error al conectar')
        return False

# Ejemplo de uso:
if __name__ == "__main__":
    # Cargar variables de entorno desde el archivo .env
    #load_dotenv()

    # Obtener las variables de entorno
    #ssid = os.getenv('WIFI_SSID')
    #password = os.getenv('WIFI_PASSWORD')
    ssid = 'Mega-2.4G-98B1'
    password = '2TW2FpZpdS'
    
    connect_wifi(ssid, password)