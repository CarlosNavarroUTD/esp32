from machine import Pin
import time

# Pines para sensores de línea
sensor_izq = Pin(32, Pin.IN)    # Sensor izquierdo
sensor_centro = Pin(33, Pin.IN)  # Nuevo sensor central
sensor_der = Pin (35, Pin.IN)    # Sensor derecho - cambiado a pin 2

# Pines para control de motores
#naranja
motor_izq_adelante = Pin(14, Pin.OUT)
motor_izq_atras = Pin(27, Pin.OUT)
#morado
motor_der_adelante = Pin(26, Pin.OUT)
motor_der_atras = Pin(25, Pin.OUT)

# Pines Enable (ajusta según tu conexión)
enable_izq = Pin(12, Pin.OUT)
enable_der = Pin(13, Pin.OUT)

# Encender enables
enable_izq.value(1)
enable_der.value(1)

def avanzar():
    print("Avanzando")
    motor_izq_adelante.value(1)
    motor_izq_atras.value(0)
    motor_der_adelante.value(1)
    motor_der_atras.value(0)

def girar_izquierda():
    print("Girando a la izquierda")
    motor_izq_adelante.value(0)  # Detener motor izquierdo
    motor_izq_atras.value(0)
    motor_der_adelante.value(1)  # Motor derecho adelante
    motor_der_atras.value(0)

def girar_derecha():
    print("Girando a la derecha")
    motor_izq_adelante.value(1)  # Motor izquierdo adelante
    motor_izq_atras.value(0)
    motor_der_adelante.value(0)  # Detener motor derecho
    motor_der_atras.value(0)

def girar_izquierda_fuerte():
    print("Girando fuertemente a la izquierda")
    motor_izq_adelante.value(0)
    motor_izq_atras.value(1)     # Motor izquierdo atrás
    motor_der_adelante.value(1)  # Motor derecho adelante
    motor_der_atras.value(0)

def girar_derecha_fuerte():
    print("Girando fuertemente a la derecha")
    motor_izq_adelante.value(1)  # Motor izquierdo adelante
    motor_izq_atras.value(0)
    motor_der_adelante.value(0)
    motor_der_atras.value(1)     # Motor derecho atrás

def detener():
    print("Deteniendo motores")
    motor_izq_adelante.value(0)
    motor_izq_atras.value(0)
    motor_der_adelante.value(0)
    motor_der_atras.value(0)

# Función de lectura de sensores
def leer_sensores():
    izq = sensor_izq.value()
    centro = sensor_centro.value()
    der = sensor_der.value()
    print(f"Sensores - Izq: {izq}, Centro: {centro}, Der: {der}")
    return izq, centro, der

# Asumiendo: 1 = línea blanca (fuera de la línea), 0 = línea negra
try:
    print("Iniciando seguidor de línea con 3 sensores")
    time.sleep(1)  # Pausa inicial

    while True:
        izq, centro, der = leer_sensores()
        
        # Lógica de seguimiento de línea
        if centro == 0:  # Sensor central detecta línea
            avanzar()  # Avanzar recto
        elif izq == 0:   # Sensor izquierdo detecta línea
            girar_izquierda()
        elif der == 0:   # Sensor derecho detecta línea
            girar_derecha()
        elif izq == 1 and centro == 1 and der == 1:  # Todos fuera de línea
            # Girar hasta encontrar la línea nuevamente
            # Opcionalmente puedes mantener el último giro
            detener()
            time.sleep(0.1)
            girar_izquierda_fuerte()  # O puedes usar girar_derecha_fuerte()
        
        time.sleep(0.05)  # Pequeña pausa para estabilidad

except KeyboardInterrupt:
    detener()
    print("Programa detenido")