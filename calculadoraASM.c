/**
    @Author Diego Abundis
    @Program Calculdora Cientifica en C con Inline assembler
**/
#include <stdio.h>
#include <stdlib.h>

/*
    Algunas convenciones del programa
    * Se agrega la variante de instrucción: l, para especificar que es un valor doble flotante
    * fld carga el registro m64fp (%1, %2, etc) en la pila FPU.
    * fstp copia el valor de ST0 en m64fp (previamente declarado como doble flotante).
    * En fstp el operador m restringe el uso de memoria almacenado dentro de asm en una variable doble flotante el valor obtenido de ST0

    * Excelente documentación respecto a las instrucciones: https://www.felixcloutier.com/x86/index.html
    * Manual de software de programacion de arquitecturas intel en: https://software.intel.com/sites/default/files/managed/39/c5/325462-sdm-vol-1-2abcd-3abcd.pdf
    * Explicación de asm volatile, restricciones de uso común y otros aspectos en: http://ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html#s4

*/

double suma(double a, double b);
double resta(double a, double b);
double multiplicacion(double a, double b);
double division(double a, double b);
double raiz(double a);
double seno(double grados);
double coseno(double grados);
double tangente(double seno, double coseno);
double conversorGrados(double grados);
int main()
{
  double a, b, resultado, senoA, cosenoA, grados;
  printf("Introduce el valor de a: ");
  scanf("%lf", &a);
  printf("Introduce el valor de b: ");
  scanf("%lf",&b);
  resultado = suma(a,b);
  printf("La suma de a y b es: %lf\n", resultado);
  resultado = resta(a,b);
  printf("La resta de a y b es: %lf\n",resultado);
  resultado = multiplicacion(a,b);
  printf("La multiplicaci\242n de a y b es: %lf\n",resultado);
  resultado = division(a,b);
  printf("La divisi\242n de a y b es: %lf\n",resultado);
  resultado = raiz(a);
  printf("La ra\241z de a es: %lf\n",resultado);
  resultado = raiz(b);
  printf("La ra\241z de b es: %lf\n",resultado);
  /*Si necesitas el valor de las funciones trigonometricas en radianes comenta la siguiente linea y pasa a grados el valor de a */
  grados = conversorGrados(a);
  senoA = seno(grados);
  printf("El seno de a es: %lf\n",senoA);
  cosenoA = coseno(grados);
  printf("El coseno de a es: %lf \n", cosenoA);
  resultado = tangente(senoA, cosenoA);
  printf("La tangente es: %lf", resultado);
  return 0;
}

double suma(double a,double b){
    double resultado;
    __asm__ ("fldl  %1;"
             "fldl  %2;"
             "faddp;"   /* sumar mediante los registros de la pila PFU, guardar en ST0 y vaciar el registro de la pila*/
             "fstpl %0;" : "=m" (resultado) : "m" (a), "m" (b) ); /* Guardar en resultado el valor de ST0*/
    return resultado;
}

double resta(double a,double b){
    double resultado;
   __asm__ ("fldl %2;"
            "fldl %1;"
            "fsubp;" /* restar mediante los registro de la pila FPU, guardar en st0 y vaciar el registro de la pila */
            "fstpl %0;" : "=m" (resultado) : "m" (a), "m" (b) );
    return resultado;
}

double multiplicacion(double a, double b){
    double resultado;
    __asm__ ("fldl %1;"
             "fldl %2;"
             "fmulp;" /* multiplicar mediante los registro de la pila, guardar en st0 y vaciar el registro de la pila*/
             "fstpl %0;" : "=m" (resultado) : "m" (a), "m" (b) );
    return resultado;
}

double division(double a, double b){
    double resultado;
    __asm__ ("fldl %2;"
             "fldl %1;"
             "fdivp;" /* dividir mediante el registro de la pila, guardar en st0 y vaciar el registro de la pila*/
             "fstpl %0;" : "=m" (resultado) : "m" (a), "m" (b) );
    return resultado;
}

double raiz(double a){
    double resultado;
    __asm__ ("fldl %1;"
             "fsqrt;" /* sacar raiz cuadrada del registro de la pila y guardar en st0 */
             "fstpl %0;" : "=m" (resultado) : "m" (a) );
    return resultado;
}

double seno(double grados){
    double resultado;
    __asm__ ("fldl %1;"
             "fsin;" /* calcular la funcion seno del valor en grados del registro de la pila y guardar en st0 */
             "fstpl %0;" : "=m" (resultado) : "m" (grados));
    return resultado;
}

double coseno(double grados){
    double resultado;
    __asm__ ("fldl %1;"
             "fcos;" /* calcular la funcion coseno del valor en grados del registro de la pila y guardar en st0 */
             "fstpl %0;" : "=m" (resultado) : "m" (grados));
    return resultado;
}

double tangente(double seno, double coseno){
    double resultado;
    __asm__ ("fldl %2;"
             "fldl %1;"
             "fdivp;" /* calcular la tangente la cual es la relacion entre seno y coseno */
             "fstpl %0;" : "=m" (resultado) : "m" (seno), "m" (coseno) );
    return resultado;
}

double conversorGrados(double grados){
    const double segundoCuadrante = 180;
    double resultado;
    __asm__ ("fldl %1;"
             "fldl %2;"
             "fldpi;"
             "fmulp;"
             "fdivp;" /* Mediante la formula (grados *(pi / segundoCuadrante)) obtenemos el valor en grados */
             "fstpl %0;" : "=m" (resultado) : "m" (segundoCuadrante), "m" (grados));
    return resultado;
}
