# Trabajo-practico-sily-riello
 TP Programación 1 - Catálogo de libros

Integrantes: Fabrizio Riello y Julieta Sily
USAL - Turno noche - 2026

El programa maneja un catálogo de libros que se guarda en un archivo de texto (libros.txt). Cada libro se guarda en una estructura y desde un menú se puede:

1. Agregar un libro
2. Listar todos los libros
3. Listar los libros que cuestan menos o igual que un precio máximo
4. Dividir el archivo en disponibles.txt y prestados.txt
5. Salir

 Cómo ejecutarlo

En CLion se abre la carpeta del proyecto y se ejecuta con el botón de play. Hay que poner la carpeta del proyecto en Run > Edit Configurations > Working directory

 Cómo funciona

- main tiene un do-while con el menú. La opción se pide con leerEntero, que solo acepta números del 1 al 5, y después un switch llama a la función de cada opción.
- Para leer del teclado usamos scanf y después limpiarBuffer, que borra lo que quedó hasta el enter. Lo usamos en vez de fflush(stdin) porque eso no es C estándar.
- leerLibro lee un libro del archivo con fscanf y devuelve false cuando llega al final, así los ciclos de lectura cortan solos. escribirLibro guarda un libro con el mismo formato.
- Opción 1: carga los datos validados y abre el archivo en modo "a" para agregar al final sin borrar lo anterior.
- Opciones 2 y 3: las dos usan la misma función, listarLibros. En la 2 muestra todo y en la 3 solo los libros con precio menor o igual al máximo que se ingresa (esa es la condición que elegimos). La condición está en cumpleCondicion.
- Opción 4: lee libros.txt y, según el campo disponible, escribe cada libro en disponibles.txt o en prestados.txt. Los abre en modo "w" para que, si se divide dos veces, no se dupliquen los libros.

Testeo

Probamos todas las opciones con el libros.txt que está en el repo:
Listar todo muestra los 15 libros.
 Con precio máximo 15000 muestra 9 libros.
 Al agregar un libro, aparece al final de la lista.
 Dividir genera 9 libros disponibles y 6 prestados.
 Si se pone una opción que no existe, letras, un año fuera de rango o un precio negativo, el programa avisa y lo vuelve a pedir.
