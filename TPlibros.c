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

//bibliotecas -J=
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

/* -F Abre un archivo y avisa si no se pudo.
 * Centraliza el control de fopen == NULL para no repetirlo en cada opcion.
 * Devuelve el puntero al archivo, o NULL si fallo. */
FILE *abrirArchivo(char nombre[], char modo[]) {
    FILE *archivo = fopen(nombre, modo);

    if (archivo == NULL) {
        printf("Error: no se pudo abrir el archivo \"%s\".\n", nombre);
    }
    return archivo;
}

/* -F Lee UN libro (una linea) del archivo y lo guarda en *libro.
- Recibe un puntero para modificar la variable del que llama (->).
- El formato es igual al del apunte: campos separados por espacios.
- El bool se lee primero en un int: fscanf no tiene formato para bool.
- Devuelve true si leyo los 5 campos y false al llegar al final del archivo. */
bool leerLibro(FILE *archivo, t_libro *libro) {
    int disponible;
    bool leido = false;

    if (fscanf(archivo, "%59s %59s %d %f %d",
               libro->titulo, libro->autor, &libro->anio,
               &libro->precio, &disponible) == CAMPOS_LIBRO) {
        if (disponible == 1) {
            libro->disponible = true;
        } else {
            libro->disponible = false;
        }
        leido = true;
    }
    return leido;
}

/* -F Escribe UN libro como una linea del archivo, con el mismo formato que
- lee leerLibro. El '\n' final es obligatorio para separar registros.
- El libro se recibe por valor (una copia), como en el apunte de estructuras. */
void escribirLibro(FILE *archivo, t_libro libro) {
    int disponible;

    if (libro.disponible) {
        disponible = 1;
    } else {
        disponible = 0;
    }
    fprintf(archivo, "%s %s %d %.2f %d\n",
            libro.titulo, libro.autor, libro.anio, libro.precio, disponible);
}

/* -F Carga por teclado todos los campos de un libro, validados, y devuelve
- el libro completo con return (como cargarDatos() del apunte de archivos). */
t_libro cargarLibro(void) {
    t_libro libro;

    leerTexto("Titulo (use _ en lugar de espacios): ", libro.titulo);
    leerTexto("Autor (use _ en lugar de espacios): ", libro.autor);
    libro.anio = leerEntero("Anio de publicacion: ", ANIO_MIN, ANIO_MAX);
    libro.precio = leerFloatPositivo("Precio: $");
    libro.disponible = leerSiNo("Esta disponible?");

    return libro;
}

/*Opciones del menu*/

/* -F Opcion 1: carga un libro y lo agrega al final del archivo.
 * Modo "a" (append): agrega sin borrar lo que ya habia y, si el archivo
 * no existe, lo crea. Primero se cargan los datos y despues se abre el
 * archivo, para tenerlo abierto el menor tiempo posible. */
void agregarLibro(char nombreArchivo[]) {
    t_libro libro;
    FILE *archivo;

    printf("\n--- Agregar libro ---\n");
    libro = cargarLibro();

    archivo = abrirArchivo(nombreArchivo, "a");
    if (archivo != NULL) {
        escribirLibro(archivo, libro);
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
void dividirArchivo(char nombreOrigen[], char nombreDisponibles[],
                    char nombrePrestados[]) {
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
                    escribirLibro(disponibles, libro);
                    cantDisponibles++;
                } else {
                    escribirLibro(prestados, libro);
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
    printf("\n%-30s %-25s %5s %11s  %-10s\n",
           "TITULO", "AUTOR", "ANIO", "PRECIO", "ESTADO");
    printf("%-30s %-25s %5s %11s  %-10s\n",
           "------------------------------", "-------------------------",
           "-----", "-----------", "----------");
}

/* [J] Imprime un libro en una fila, alineado con el encabezado.
- %-30s: alinea a la izquierda ocupando 30 lugares. */
void mostrarLibro(t_libro libro) {
    printf("%-30s %-25s %5d %11.2f  ",
           libro.titulo, libro.autor, libro.anio, libro.precio);
    if (libro.disponible) {
        printf("Disponible\n");
    } else {
        printf("Prestado\n");
    }
}

/* [J] Condicion elegida por el equipo para la opcion 3:
 * el precio del libro es menor o igual al precio maximo pedido.
 * Esta en una funcion propia para poder cambiar la condicion en un solo lugar. */
bool cumpleCondicion(t_libro libro, float precioMaximo) {
    return libro.precio <= precioMaximo;
}

/* [J] Recorre el archivo de principio a fin (acceso secuencial) y muestra los
 * libros por pantalla. La usan las opciones 2 y 3 para no repetir codigo:
 *   - filtrar = false: muestra todos (precioMaximo no se usa).
 *   - filtrar = true:  muestra solo los que cumplen la condicion.
 * El ciclo termina cuando leerLibro devuelve false (fin del archivo). */
void listarLibros(char nombreArchivo[], bool filtrar, float precioMaximo) {
    FILE *archivo;
    t_libro libro;
    int mostrados = 0;

    archivo = abrirArchivo(nombreArchivo, "r");
    if (archivo != NULL) {
        mostrarEncabezado();
        while (leerLibro(archivo, &libro)) {
            if (!filtrar || cumpleCondicion(libro, precioMaximo)) {
                mostrarLibro(libro);
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
void listarPorPrecio(char nombreArchivo[]) {
    float precioMaximo;

    printf("\n--- Libros hasta un precio maximo ---\n");
    precioMaximo = leerFloatPositivo("Precio maximo: $");
    listarLibros(nombreArchivo, true, precioMaximo);
}
