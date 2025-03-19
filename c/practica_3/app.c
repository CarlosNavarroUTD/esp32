#include <stdio.h>

double calculate_discount(double total_amount) {
    if (total_amount <= 1000) {
        return total_amount; // No discount
    }
    return calculate_discount( total_amount * 0.9); // Apply 10% discount
}

int main() {
    double total_amount;
    printf("Enter the total amount: ");
    scanf("%lf", &total_amount);
    double discounted_amount = calculate_discount(total_amount);
    printf("The discounted amount is: %.2f\n", discounted_amount);
    return 0;
}
