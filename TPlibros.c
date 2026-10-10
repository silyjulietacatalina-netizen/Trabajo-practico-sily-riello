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

//bibliotecas -J=
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

void leerLinea(char *destino, int tam) {
    size_t largo;
    int c;

    if (fgets(destino, tam, stdin) == NULL) {
        destino[0] = '\0';
    } else {
        largo = strlen(destino);
        if (largo > 0 && destino[largo - 1] == '\n') {
            destino[largo - 1] = '\0';
        } else {
            while ((c = getchar()) != '\n' && c != EOF) {
                /* descarta el resto de la linea */
            }
        }
    }
}

/* -F Pide un texto hasta que sea valido y lo guarda */
void leerTexto(const char *mensaje, char *destino, int tam) {
    bool valido;

    do {
        printf("%s", mensaje);
        leerLinea(destino, tam);
        valido = destino[0] != '\0'
                 && !isspace((unsigned char) destino[0])
                 && strchr(destino, SEPARADOR) == NULL;
        if (!valido) {
            printf("  Error: no puede estar vacio, empezar con espacio ni contener '%c'.\n",
                   SEPARADOR);
        }
    } while (!valido);
}

/* [F] Pide un entero entre min y max (inclusive) hasta que sea valido.
 * sscanf con "%d %c" devuelve 1 solo si hay un numero y nada mas despues:
 * asi se rechazan entradas como "12abc" o "3 4". */
int leerEntero(const char *mensaje, int min, int max) {
    char linea[MAX_LINEA];
    int valor = 0;
    char extra;
    bool valido;

    do {
        printf("%s", mensaje);
        leerLinea(linea, MAX_LINEA);
        valido = sscanf(linea, "%d %c", &valor, &extra) == 1
                 && valor >= min && valor <= max;
        if (!valido) {
            printf("  Error: ingrese un numero entero entre %d y %d.\n", min, max);
        }
    } while (!valido);

    return valor;
}

/* [J] Pide un numero real mayor a cero (un precio) hasta que sea valido.
 * Misma tecnica que leerEntero. El separador decimal es el punto. */
float leerFloatPositivo(const char *mensaje) {
    char linea[MAX_LINEA];
    float valor = 0;
    char extra;
    bool valido;

    do {
        printf("%s", mensaje);
        leerLinea(linea, MAX_LINEA);
        valido = sscanf(linea, "%f %c", &valor, &extra) == 1 && valor > 0;
        if (!valido) {
            printf("  Error: ingrese un numero mayor a 0 (use punto para los decimales).\n");
        }
    } while (!valido);

    return valor;
}

/* [J] Hace una pregunta de si/no. Acepta S/s/N/n.
 * Devuelve true si la respuesta es S y false si es N. */
bool leerSiNo(const char *mensaje) {
    char linea[MAX_LINEA];
    char respuesta;
    bool valido;

    do {
        printf("%s (S/N): ", mensaje);
        leerLinea(linea, MAX_LINEA);
        respuesta = (char) toupper((unsigned char) linea[0]);
        valido = strlen(linea) == 1 && (respuesta == 'S' || respuesta == 'N');
        if (!valido) {
            printf("  Error: responda S o N.\n");
        }
    } while (!valido);

    return respuesta == 'S';
}

/* -F Abre un archivo y avisa si no se pudo.
 * Centraliza el control de fopen == NULL para no repetirlo en cada opcion.
 * Devuelve el puntero al archivo, o NULL si fallo. */
FILE *abrirArchivo(const char *nombre, const char *modo) {
    FILE *archivo = fopen(nombre, modo);

    if (archivo == NULL) {
        printf("Error: no se pudo abrir el archivo \"%s\".\n", nombre);
    }
    return archivo;
}

/* -F Lee UN libro (una linea) del archivo y lo guarda en *libro.
 * - " " al inicio del formato salta el '\n' que dejo la linea anterior.
 * - %59[^;] lee hasta 59 caracteres que no sean ';' (59 = MAX_STR - 1,
 *   se deja lugar para el '\0'). Asi se pueden leer textos con espacios.
 * - El bool se lee primero en un int: fscanf no tiene formato para bool.
 * Devuelve true si leyo los 5 campos y false al llegar al final del archivo
 * (o si la linea esta mal formada). */
bool leerLibro(FILE *archivo, t_libro *libro) {
    int disponible;
    bool leido = false;

    if (fscanf(archivo, " %59[^;];%59[^;];%d;%f;%d",
               libro->titulo, libro->autor, &libro->anio,
               &libro->precio, &disponible) == CAMPOS_LIBRO) {
        libro->disponible = (disponible == 1);
        leido = true;
    }
    return leido;
}

/* -F Escribe UN libro como una linea del archivo, con el mismo formato que
 * lee leerLibro. El '\n' final es obligatorio para separar registros.
 * Recibe un puntero const: no copia la estructura y no la puede modificar. */
void escribirLibro(FILE *archivo, const t_libro *libro) {
    fprintf(archivo, "%s;%s;%d;%.2f;%d\n",
            libro->titulo, libro->autor, libro->anio,
            libro->precio, libro->disponible ? 1 : 0);
}

/* -F Carga por teclado todos los campos de un libro, validados.
 * Recibe un puntero para modificar la variable del que llama
 * (pasaje por referencia); se accede a los campos con "->". */
void cargarLibro(t_libro *libro) {
    leerTexto("Titulo: ", libro->titulo, MAX_STR);
    leerTexto("Autor: ", libro->autor, MAX_STR);
    libro->anio = leerEntero("Anio de publicacion: ", ANIO_MIN, ANIO_MAX);
    libro->precio = leerFloatPositivo("Precio: $");
    libro->disponible = leerSiNo("Esta disponible?");
}

/*Opciones del menu*/

/* -F Opcion 1: carga un libro y lo agrega al final del archivo.
 * Modo "a" (append): agrega sin borrar lo que ya habia y, si el archivo
 * no existe, lo crea. Primero se cargan los datos y despues se abre el
 * archivo, para tenerlo abierto el menor tiempo posible. */
void agregarLibro(const char *nombreArchivo) {
    t_libro libro;
    FILE *archivo;

    printf("\n--- Agregar libro ---\n");
    cargarLibro(&libro);

    archivo = abrirArchivo(nombreArchivo, "a");
    if (archivo != NULL) {
        escribirLibro(archivo, &libro);
        fclose(archivo);
        printf("Libro agregado correctamente.\n");
    }
}

/* -F Opcion 4: recorre el archivo de origen y copia cada libro a uno de dos
 * archivos nuevos segun el campo booleano "disponible".
 * Los archivos destino se abren en modo "w" A PROPOSITO: se regeneran
 * completos cada vez que se divide, para que no queden libros duplicados.
 * Se lee de un archivo y se escribe en otros: nunca lectura y escritura
 * sobre el mismo archivo a la vez. */
void dividirArchivo(const char *nombreOrigen, const char *nombreDisponibles,
                    const char *nombrePrestados) {
    FILE *origen;
    FILE *disponibles;
    FILE *prestados;
    t_libro libro;
    int cantDisponibles = 0;
    int cantPrestados = 0;

    origen = abrirArchivo(nombreOrigen, "r");
    if (origen != NULL) {
        disponibles = abrirArchivo(nombreDisponibles, "w");
        prestados = abrirArchivo(nombrePrestados, "w");

        if (disponibles != NULL && prestados != NULL) {
            while (leerLibro(origen, &libro)) {
                if (libro.disponible) {
                    escribirLibro(disponibles, &libro);
                    cantDisponibles++;
                } else {
                    escribirLibro(prestados, &libro);
                    cantPrestados++;
                }
            }
            printf("\nArchivo dividido correctamente:\n");
            printf("  %-16s %d libros\n", nombreDisponibles, cantDisponibles);
            printf("  %-16s %d libros\n", nombrePrestados, cantPrestados);
        }

        /* Se cierra cada archivo que se haya podido abrir. */
        if (disponibles != NULL) {
            fclose(disponibles);
        }
        if (prestados != NULL) {
            fclose(prestados);
        }
        fclose(origen);
    }
}

/* [J] Muestra las opciones del menu. */
void mostrarMenu(void) {
    printf("\n========== CATALOGO DE LIBROS ==========\n");
    printf("%d. Agregar un libro\n", OP_AGREGAR);
    printf("%d. Listar todos los libros\n", OP_LISTAR);
    printf("%d. Listar libros hasta un precio maximo\n", OP_FILTRAR);
    printf("%d. Dividir en disponibles / prestados\n", OP_DIVIDIR);
    printf("%d. Salir\n", OP_SALIR);
    printf("========================================\n");
}

/* [J] Imprime los titulos de las columnas del listado. */
void mostrarEncabezado(void) {
    printf("\n%-30s %-22s %5s %11s  %-10s\n",
           "TITULO", "AUTOR", "ANIO", "PRECIO", "ESTADO");
    printf("%-30s %-22s %5s %11s  %-10s\n",
           "------------------------------", "----------------------",
           "-----", "-----------", "----------");
}

/* [J] Imprime un libro en una fila, alineado con el encabezado.
 * %-30.30s: alinea a la izquierda en 30 lugares y corta si es mas largo. */
void mostrarLibro(const t_libro *libro) {
    printf("%-30.30s %-22.22s %5d %11.2f  %-10s\n",
           libro->titulo, libro->autor, libro->anio, libro->precio,
           libro->disponible ? "Disponible" : "Prestado");
}

/* [J] Condicion elegida por el equipo para la opcion 3:
 * el precio del libro es menor o igual al precio maximo pedido.
 * Esta en una funcion propia para poder cambiar la condicion en un solo lugar. */
bool cumpleCondicion(const t_libro *libro, float precioMaximo) {
    return libro->precio <= precioMaximo;
}

/* [J] Recorre el archivo de principio a fin (acceso secuencial) y muestra los
 * libros por pantalla. La usan las opciones 2 y 3 para no repetir codigo:
 *   - filtrar = false: muestra todos (precioMaximo no se usa).
 *   - filtrar = true:  muestra solo los que cumplen la condicion.
 * El ciclo termina cuando leerLibro devuelve false (fin del archivo). */
void listarLibros(const char *nombreArchivo, bool filtrar, float precioMaximo) {
    FILE *archivo;
    t_libro libro;
    int mostrados = 0;

    archivo = abrirArchivo(nombreArchivo, "r");
    if (archivo != NULL) {
        mostrarEncabezado();
        while (leerLibro(archivo, &libro)) {
            if (!filtrar || cumpleCondicion(&libro, precioMaximo)) {
                mostrarLibro(&libro);
                mostrados++;
            }
        }
        fclose(archivo);

        if (mostrados == 0) {
            printf("No hay libros para mostrar.\n");
        } else {
            printf("\nTotal de libros mostrados: %d\n", mostrados);
        }
    }
}

/* [J] Opcion 3: pide el precio maximo y lista los libros que no lo superan. */
void listarPorPrecio(const char *nombreArchivo) {
    float precioMaximo;

    printf("\n--- Libros hasta un precio maximo ---\n");
    precioMaximo = leerFloatPositivo("Precio maximo: $");
    listarLibros(nombreArchivo, true, precioMaximo);
}



    return 0;
}
