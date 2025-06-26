#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_STR 100
#define MAX_LIBROS 1000
#define MAX_USUARIOS 100

//Estructura para el libro
typedef struct {
    int id;
    char titulo[MAX_STR];
    char autor[MAX_STR];
    char genero[MAX_STR];
    int anio;
    bool bookState = true;
} Libro;

// Estructura para el un usuario
typedef struct {
    int idUsuario;              // Identificador único
    List *prestamosActuales = list_create(); // Lista enlazada de libros actualmente prestados
    List *historial = list_create();         // Lista enlazada de todos los libros que ha tomado
} Usuario;

int cargarCatalogo(Map*mapaAño , Map*mapaAño , Map*mapaAño , Map*mapaAño ,Map*mapaAño ) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombreArchivo);
        return 0;
    }

    char linea[500];
    int cantidad = 0;

    fgets(linea, sizeof(linea), archivo);

    while (fgets(linea, sizeof(linea), archivo) && cantidad < maxLibros) {
        // Separar los campos por coma
        char *token = strtok(linea, ",");
        if (token == NULL) continue;

        catalogo[cantidad].id = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strncpy(catalogo[cantidad].titulo, token, MAX_STR);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strncpy(catalogo[cantidad].autor, token, MAX_STR);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strncpy(catalogo[cantidad].genero, token, MAX_STR);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        catalogo[cantidad].anio = atoi(token);

        cantidad++;
    }

    fclose(archivo);
    return cantidad;
}


int hash(int id) {
    return id % MAX_USUARIOS;
}

void registrarUsuario(Usuario *usuariosHash[MAX_USUARIOS]) {
    int id;
    printf("Ingrese el ID del nuevo usuario: ");
    scanf("%d", &id);

    int pos = hash(id);

    // Buscar espacio libre o existente (colisión lineal)
    int original = pos;
    while (usuariosHash[pos] != NULL && usuariosHash[pos]->idUsuario != id) {
        pos = (pos + 1) % MAX_USUARIOS;
        if (pos == original) {
            printf("Error: el mapa de usuarios está lleno.\n");
            return;
        }
    }

    if (usuariosHash[pos] != NULL) {
        printf("El ID %d ya está registrado.\n", id);
        return;
    }

    Usuario *nuevo = malloc(sizeof(Usuario));
    if (nuevo == NULL) {
        printf("Error al asignar memoria.\n");
        return;
    }

    nuevo->idUsuario = id;
    nuevo->prestamosActuales = NULL;
    nuevo->historial = NULL;

    usuariosHash[pos] = nuevo;

    printf("Usuario con ID %d registrado exitosamente.\n", id);
}

void buscarTitulo(Libro catalogo[], int total){
    char titulo[MAX_STR];
    printf("Ingrese el titulo del libro a buscar: ");
    getchar();
    fgets(titulo,MAX_STR,stdin);
    titulo[strcpn(titulo, "\n")] = 0;
    int encontrado = 0;
    for(int i = 0; i < total; i++){
        if(strcmp(catalogo[i].titulo,titulo) == 0){
            printf("Libro encontrado:\n");
            printf("Titulo: %s\n", catalogo[i].titulo);
            printf("Autor: %s\n", catalogo[i].autor);
            printf("Genero: %s", catalogo[i].genero);
            printf("years: %d\n", catalogo[i].anio);
            encontrado = 1;
            break;
        }
    }
    if(!encontrado){
        printf("No se encontro :(\n");

    }

}
void buscarAutor(Libro catalogo[], int total){
    char autor[MAX_STR];
    printf("Ingrese el nombre autor a buscar: ");
    getchar();
    fgets(autor,MAX_STR,stdin);
    autor[strcpn(autor, "\n")] = 0;
    int encontrado = 0;
    printf("\nLibros por autor:\n",autor);
    for(int i = 0; i < total; i++){
        if(strcmp(catalogo[i].autor,autor) == 0){
            printf("\nID: %d\n", catalogo[i].id);
            printf("Titulo: %s\n", catalogo[i].titulo);
            printf("Autor: %s\n", catalogo[i].autor);
            printf("Genero: %s", catalogo[i].genero);
            printf("years: %d\n", catalogo[i].anio);
            encontrado = 1;
            break;
        }
    }
    if(!encontrado){
        printf("No se encontro :(\n");

    }
}

void buscarGenero(Libro catalogo[], int total){
    char genero[MAX_STR];
    printf("Ingrese el genero a buscar: ");
    getchar();
    fgets(genero,MAX_STR,stdin);
    genero[strcpn(genero, "\n")] = 0;
    int encontrado = 0;
    printf("\nLibros por genero:\n",genero);
    for(int i = 0; i < total; i++){
        if(strcmp(catalogo[i].autor,genero) == 0){
            printf("\nID: %d\n", catalogo[i].id);
            printf("Titulo: %s\n", catalogo[i].titulo);
            printf("Autor: %s\n", catalogo[i].autor);
            printf("Genero: %s", catalogo[i].genero);
            printf("years: %d\n", catalogo[i].anio);
            encontrado = 1;
            break;
        }
    }
    if(!encontrado){
        printf("No se encontro :(\n");

    }
}
void buscarAnio(Libro catalogo[], int total){
    char years[MAX_STR];
    printf("Ingrese el anio a buscar: ");
    getchar();
    fgets(years,MAX_STR,stdin);
    years[strcpn(years, "\n")] = 0;
    int encontrado = 0;
    printf("\nLibros por anio:\n",years);
    for(int i = 0; i < total; i++){
        if(strcmp(catalogo[i].autor,years) == 0){
            printf("\nID: %d\n", catalogo[i].id);
            printf("Titulo: %s\n", catalogo[i].titulo);
            printf("Autor: %s\n", catalogo[i].autor);
            printf("Genero: %s", catalogo[i].genero);
            printf("years: %d\n", catalogo[i].anio);
            encontrado = 1;
            break;
        }
    }
    if(!encontrado){
        printf("No se encontro :(\n");

    }
}





int main() {
    Map *mapaAñoPublicacion = map_create();
    Map *mapaCategoria = map_create();
    Map *mapaTitulo = map_create();
    Map *mapaAutor = map_create();
    Map *mapaId = map_create();

    Libro catalogo[MAX_LIBROS];
    int totalLibros = cargarCatalogo("catalogo.csv", catalogo, MAX_LIBROS);

    printf("Se cargaron %d libros.\n", totalLibros);
    for (int i = 0; i < totalLibros && i < 5; i++) {
        printf("Libro %d: %s (%d) - %s\n", catalogo[i].id, catalogo[i].titulo, catalogo[i].anio, catalogo[i].autor);
    }
    Usuario *usuariosHash[MAX_USUARIOS] = {0};  // inicializado en NULL
    registrarUsuario(usuariosHash);
    return 0;
}
