#include <stdio.h>

// Función para calcular el nuevo saldo del préstamo
double calcular_prestamo(double prestamo, double interes, double pago_anual) {
    return prestamo + (prestamo * interes) - pago_anual;
}

int main() {
    // Declaración de variables
    double prestamo, interes, pago_anual, target;
    int years = 0;

    // Entrada de datos
    printf("Ingrese el monto inicial del prestamo: ");
    scanf("%lf", &prestamo);
    printf("Ingrese el interes anual (en porcentaje, ej: 0.05 para 5%%): ");
    scanf("%lf", &interes);
    printf("Ingrese el pago anual: ");
    scanf("%lf", &pago_anual);
    printf("Ingrese el monto objetivo: ");
    scanf("%lf", &target);

    // Ciclo para calcular el progreso del préstamo
    while (prestamo > target) {
        prestamo = calcular_prestamo(prestamo, interes, pago_anual);
        years++;
    }

    // Resultados
    printf("El prestamo alcanzó el objetivo en %d años.\n", years);
    printf("Saldo restante del prestamo: %.2f\n", prestamo);

    return 0;
}

