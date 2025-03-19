from machine import Pin
import time

# Pines para sensores de línea
sensor_izq = Pin(0, Pin.IN)  # Sensor izquierdo
sensor_der = Pin(0, Pin.IN)  # Sensor derecho

# Pines para control de motores
motor_izq_adelante = Pin(14, Pin.OUT)
motor_izq_atras = Pin(27, Pin.OUT)
motor_der_adelante = Pin(26, Pin.OUT)
motor_der_atras = Pin(25, Pin.OUT)

# Pines Enable (ajusta según tu conexión)
enable_izq = Pin(12, Pin.OUT)
enable_der = Pin(13, Pin.OUT)

# Encender enables
enable_izq.value(1)
enable_der.value(1)

def leftEnginneFront():
    print("Probando motor izquierdo adelante")
    motor_izq_adelante.value(1)
    motor_izq_atras.value(0)

def leftEnginneBack():
    print("Probando motor izquierdo atrás")
    motor_izq_adelante.value(0)
    motor_izq_atras.value(1)

def leftEnginneStop():
    print("Deteniendo motor izquierdo")
    motor_izq_adelante.value(0)
    motor_izq_atras.value(0)

def rightEnginneFront():
    print("Probando motor derecho adelante")
    motor_der_adelante.value(1)
    motor_der_atras.value(0)

def rightEnginneBack():
    print("Probando motor derecho atrás")
    motor_der_adelante.value(0)
    motor_der_atras.value(1)

def rightEnginneStop():
    print("Deteniendo motor izquierdo")
    motor_der_adelante.value(0)
    motor_der_atras.value(0)


# Mostrar mensaje cada 2 segundos
try:
    while True:
        izq = sensor_izq.value()
        der = sensor_der.value()
        print(f"Sensor Izq: {izq}, Sensor Der: {der}")
        time.sleep(0.1)  # Pequeña pausa para evitar saturar la terminal

        rightEnginneStop()
        leftEnginneStop()
        print ("Motores detenidos")

except KeyboardInterrupt:
    print("Programa detenido")



