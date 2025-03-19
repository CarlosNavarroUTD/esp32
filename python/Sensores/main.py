from machine import Pin
import time

# Pines para sensores de línea
sensor_izq = Pin(32, Pin.IN)  # Sensor izquierdo
sensor_centro = Pin(33, Pin.IN)  # Sensor izquierdo
sensor_der = Pin(35, Pin.IN)  # Sensor derecho

# Bucle principal para leer sensores
try:
    while True:

        izq = sensor_izq.value()
        centro = sensor_centro.value()
        der = sensor_der.value()
        print(f"Sensor Izq: {izq}, Centro: {centro} Sensor Der: {der}")
        time.sleep(0.1)  # Pequeña pausa  para evitar saturar la terminal
except KeyboardInterrupt:
    print("Programa detenido por el usuario")
