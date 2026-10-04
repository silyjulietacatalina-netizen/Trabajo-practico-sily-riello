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

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_STR 60
#define MAX_LINEA 100
#define CAMPOS_LIBRO 5
#define SEPARADOR ';'
#define ANIO_MIN 1450
#define ANIO_MAX 2026

#define ARCHIVO_LIBROS "libros.txt"
#define ARCHIVO_DISPONIBLES "disponibles.txt"
#define ARCHIVO_PRESTADOS "prestados.txt"


int main(void) {
    return 0;
}