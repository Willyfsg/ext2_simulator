# Author Sebastián Fco. Gutiérrez R.

Este programa en C es un sistema de archivos ext2, el cual se encarga de simular un sistema de archivos de Linux. Utiliza estructuras como el superbloque, la tabla de mapas de bytes, la tabla de inodos y la tabla de directorios para llevar a cabo funciones como la lectura, impresión, renombre, borrado y copia de archivos.

El programa tiene funciones para imprimir los mapas de bytes, comprobar un comando dado, leer el superbloque, buscar un archivo dado por nombre, imprimir un archivo, renombrar un archivo, borrar un archivo y copiar un archivo.

Además, el programa utiliza la librería "cabeceras.h" para almacenar las estructuras y las constantes necesarias para el funcionamiento del sistema de archivos.

Es importante mencionar que el esqueleto del código fue desarrollado por el profesor [Javier García-Algarra](https://github.com/jgalgarra).

El programa se compone de varias funciones, estas son:

* Printbytemaps: imprime los mapas de bytes.
* ComprobarComando: comprueba si un comando es válido.
* LeeSuperBloque: lee el superbloque.
* BuscaFich: busca un archivo dado por nombre en la tabla de directorios.
* Directorio: imprime la tabla de directorios.
* Renombrar: renombra un archivo.
* Imprimir: imprime.
