class factorial_num:
    def calculo (self, n):      
        if n == 0:
            return 1
        return n * self.calculo(n-1)

factorial_num = factorial_num()
print(factorial_num.calculo(5))

#calculo(5) va a llacar a calculo(4)
#se repite hasta que n == 0 y devuelve 1
#reescribe el valor del paso 1 para realizar la operación de nuevo
