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




    return 0;
}
