from machine import Pin, I2C
import ssd1306
import time

# Configuración de la pantalla OLED
i2c = I2C(0, scl=Pin(23), sda=Pin(22), freq=400000)
oled = ssd1306.SSD1306_I2C(128, 64, i2c)

# Configuración de los botones (Pull-Up interno)
btn1 = Pin(18, Pin.IN, Pin.PULL_UP)
btn2 = Pin(19, Pin.IN, Pin.PULL_UP)
btn3 = Pin(21, Pin.IN, Pin.PULL_UP)
btn4 = Pin(25, Pin.IN, Pin.PULL_UP)

# Función para mostrar en la pantalla
def mostrar_texto(texto):
    oled.fill(0)  # Limpiar pantalla
    oled.text(texto, 10, 30)
    oled.show()

# Bucle principal
try:
    while True:
        if btn1.value() == 1:
            mostrar_texto("Boton 1 presionado")
        elif btn2.value() == 1:
            mostrar_texto("Boton 2 presionado")
        elif btn3.value() == 1:
            mostrar_texto("Boton 3 presionado")
        elif btn4.value() == 1:
            mostrar_texto("Boton 4 presionado")
        else:
            mostrar_texto("Press a btn")

        time.sleep(0.1)  # Pequeña pausa para evitar rebotes

except KeyboardInterrupt:
    print("Programa detenido")
