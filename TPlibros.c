/*

Tema: LIBROS
 Integrantes: Fabrizio Riello y Julieta Sily

  Programa que administra un catalogo de libros guardado en un ARCHIVO DE TEXTO.
 Cada linea del archivo es un libro y sus campos van separados por punto y coma:

      titulo;autor;anio;precio;disponible
     Cien anios de soledad;Gabriel Garcia Marquez;1967;18500.00;1

  Menu de opciones:
      1. Agregar un libro al archivo
      2. Listar todos los libros
      3. Listar los libros con precio menor o igual a un maximo
      4. Dividir el archivo en disponibles.txt y prestados.txt
     5. Salir
 */

//bibliotecas -J
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

//Macros y definiciones -J
#define MAX_STR 60
#define MAX_LINEA 100
#define CAMPOS_LIBRO 5
#define SEPARADOR ';'
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
void leerLinea(char *destino, int tam);
void leerTexto(const char *mensaje, char *destino, int tam);
int leerEntero(const char *mensaje, int min, int max);
FILE *abrirArchivo(const char *nombre, const char *modo);
bool leerLibro(FILE *archivo, t_libro *libro);
void escribirLibro(FILE *archivo, const t_libro *libro);
void cargarLibro(t_libro *libro);
void agregarLibro(const char *nombreArchivo);
void dividirArchivo(const char *nombreOrigen, const char *nombreDisponibles,
                    const char *nombrePrestados);

//Prototipos de funciones (J)
void mostrarMenu(void);
float leerFloatPositivo(const char *mensaje);
bool leerSiNo(const char *mensaje);
void mostrarEncabezado(void);
void mostrarLibro(const t_libro *libro);
bool cumpleCondicion(const t_libro *libro, float precioMaximo);
void listarLibros(const char *nombreArchivo, bool filtrar, float precioMaximo);
void listarPorPrecio(const char *nombreArchivo);

int main(void) {




    return 0;
}
