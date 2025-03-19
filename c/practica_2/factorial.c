#include <stdio.h>

// Función recursiva para calcular el factorial
int get_factorial(int n) {
    if (n == 0) {
        return 1;
    }
    return n * get_factorial(n - 1);
}

int main() {
    int n;

    // Bucle para asegurar que el número esté en el rango válido
    do {
        printf("Introduce un número (entre 0 y 33): ");
        int valid_input = scanf("%d", &n);  // Leer la entrada

        // Vcode erificar si la entrada es válida
        if (valid_input != 1) {
            // Si no es un número, limpiar el buffer y mostrar mensaje de error
            while(getchar() != '\n');
            printf("Por favor, introduce un número válido.\n");
            continue;
        }

        // Validar que el número esté en el rango permitido
        if (n < 0 || n > 33) {
            printf("El número debe estar entre 0 y 33.\n");
        } else {
            // Si el número es válido, calcular y mostrar el factorial
            printf("El factorial de %d es %d\n", n, get_factorial(n));
            break;  // Salir del bucle
        }
    } while (1);  // Continuar hasta que se ingrese un número válido

    return 0;
}
