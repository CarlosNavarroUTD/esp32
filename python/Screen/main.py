from machine import Pin, I2C
import ssd1306 as ssd1306
import time

# Configuración del I2C para la pantalla OLED
i2c = I2C(0, scl=Pin(23), sda=Pin(22), freq=400000)

# Inicializar la pantalla OLED (128x64)
oled = ssd1306.SSD1306_I2C(128, 64, i2c)

# Función para mostrar un mensaje en la pantalla
def mostrar_texto():
    oled.fill(0)  # Limpia la pantalla
    oled.text("Hola, ESP32!", 10, 10)  # Texto (x=10, y=10)
    oled.text("MicroPython", 10, 30)
    oled.show()  # Muestra el contenido en la pantalla

# Mostrar mensaje cada 2 segundos
try:
    while True:
        mostrar_texto()
        time.sleep(5)
except KeyboardInterrupt:
    print("Programa detenido")
