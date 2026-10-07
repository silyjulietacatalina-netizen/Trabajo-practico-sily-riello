/*

Tema: LIBROS
 Integrantes: Fabrizio Riello y Julieta Sily

  Programa que administra un catalogo de libros guardado en un ARCHIVO DE TEXTO.
 Cada linea del archivo es un libro y sus campos van separados por espacios:

      titulo autor anio precio disponible
      Cien_anios_de_soledad Gabriel_Garcia_Marquez 1967 18500.00 1

 Como %s corta la lectura en el primer espacio, los titulos y autores se
 escriben con guion bajo (_) en lugar de espacios.

  Menu de opciones:
      1. Agregar un libro al archivo
      2. Listar todos los libros
      3. Listar los libros con precio menor o igual a un maximo
      4. Dividir el archivo en disponibles.txt y prestados.txt
     5. Salir
 */

//bibliotecas -J
#include <stdio.h>
#include <stdbool.h>

//Macros y definiciones -J
#define MAX_STR 60
#define CAMPOS_LIBRO 5
#define ANIO_MIN 1450
#define ANIO_MAX 2026

#define ARCHIVO_LIBROS "libros.txt"
#define ARCHIVO_DISPONIBLES "disponibles.txt"
#define ARCHIVO_PRESTADOS "prestados.txt"

//Definicion de estructuras -J
typedef struct {
    char titulo[MAX_STR];
    char autor[MAX_STR];
    int anio;
    float precio;
    bool disponible;
} t_libro;

typedef enum {
    OP_AGREGAR = 1,
    OP_LISTAR,
    OP_FILTRAR,
    OP_DIVIDIR,
    OP_SALIR
} t_opcion;

//DECLARACION FUNCIONES -F
void limpiarBuffer(void);
void leerTexto(char mensaje[], char destino[]);
int leerEntero(char mensaje[], int min, int max);
FILE *abrirArchivo(char nombre[], char modo[]);
bool leerLibro(FILE *archivo, t_libro *libro);
void escribirLibro(FILE *archivo, t_libro libro);
t_libro cargarLibro(void);
void agregarLibro(char nombreArchivo[]);
void dividirArchivo(char nombreOrigen[], char nombreDisponibles[],
                    char nombrePrestados[]);

//Prototipos de funciones (J)
void mostrarMenu(void);
float leerFloatPositivo(char mensaje[]);
bool leerSiNo(char mensaje[]);
void mostrarEncabezado(void);
void mostrarLibro(t_libro libro);
bool cumpleCondicion(t_libro libro, float precioMaximo);
void listarLibros(char nombreArchivo[], bool filtrar, float precioMaximo);
void listarPorPrecio(char nombreArchivo[]);

int main(void) {
    int opcion;

    do {
        mostrarMenu();
        /* leerEntero ya valida que la opcion este entre 1 y 5, */
        opcion = leerEntero("Opcion: ", OP_AGREGAR, OP_SALIR);

        switch (opcion) {
            case OP_AGREGAR:
                agregarLibro(ARCHIVO_LIBROS);
                break;
            case OP_LISTAR:
                listarLibros(ARCHIVO_LIBROS, false, 0);
                break;
            case OP_FILTRAR:
                listarPorPrecio(ARCHIVO_LIBROS);
                break;
            case OP_DIVIDIR:
                dividirArchivo(ARCHIVO_LIBROS, ARCHIVO_DISPONIBLES, ARCHIVO_PRESTADOS);
                break;
            case OP_SALIR:
                printf("\nFin del programa. Hasta luego.\n");
                break;
        }
    } while (opcion != OP_SALIR);

    return 0;
}

/* Funciones de input -F*/

/* -F Descarta lo que quedo en el teclado hasta el enter, para que no se
- mezcle con la proxima lectura. Se usa en lugar de fflush(stdin), que no
- es C estandar (solo funciona en Windows). */
void limpiarBuffer(void) {
    int c;

    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

/* -F Pide un texto de una sola palabra y lo guarda en destino.
- %59s lee como maximo 59 letras (MAX_STR - 1) para no escribir fuera del
- arreglo y dejar lugar para el '\0'. */
void leerTexto(char mensaje[], char destino[]) {
    printf("%s", mensaje);
    scanf("%59s", destino);
    limpiarBuffer();
}

/* [F] Pide un entero entre min y max (inclusive) hasta que sea valido.
- Si el usuario escribe letras, scanf no guarda nada: por eso valor arranca
- fuera del rango y el ciclo vuelve a pedirlo. */
int leerEntero(char mensaje[], int min, int max) {
    int valor;

    do {
        printf("%s", mensaje);
        valor = min - 1;
        scanf("%d", &valor);
        limpiarBuffer();
        if (valor < min || valor > max) {
            printf("  Error: ingrese un numero entero entre %d y %d.\n", min, max);
        }
    } while (valor < min || valor > max);

    return valor;
}

/* [J] Pide un numero real mayor a cero (un precio) hasta que sea valido.
- Misma tecnica que leerEntero. El separador decimal es el punto. */
float leerFloatPositivo(char mensaje[]) {
    float valor;

    do {
        printf("%s", mensaje);
        valor = 0;
        scanf("%f", &valor);
        limpiarBuffer();
        if (valor <= 0) {
            printf("  Error: ingrese un numero mayor a 0 (use punto para los decimales).\n");
        }
    } while (valor <= 0);

    return valor;
}

/* [J] Hace una pregunta de si/no. Acepta S/s/N/n.
- Devuelve true si la respuesta es S y false si es N.
- El espacio en " %c" saltea los enters que hayan quedado antes. */
bool leerSiNo(char mensaje[]) {
    char respuesta;
    bool valido;

    do {
        printf("%s (S/N): ", mensaje);
        scanf(" %c", &respuesta);
        limpiarBuffer();
        valido = respuesta == 'S' || respuesta == 's' || respuesta == 'N' || respuesta == 'n';
        if (!valido) {
            printf("  Error: responda S o N.\n");
        }
    } while (!valido);

    return respuesta == 'S' || respuesta == 's';
}


    return 0;
}
