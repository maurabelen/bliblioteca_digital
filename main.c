#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STR 100
#define MAX_LIBROS 1000
#define MAX_USUARIOS 100

//Estructura para el libro
typedef struct {
    int id;
    char titulo[MAX_STR];
    char autor[MAX_STR];
    char genero[MAX_STR];
    int years;
} Libro;

// Estructura para el un usuario
typedef struct {
    int idUsuario;                 // Identificador único
    NodoLibro *prestamosActuales; // Lista enlazada de libros actualmente prestados
    NodoLibro *historial;         // Lista enlazada de todos los libros que ha tomado
} Usuario;

int cargarCatalogo(const char *nombreArchivo, Libro catalogo[], int maxLibros) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombreArchivo);
        return 0;
    }

    char linea[512];
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
        catalogo[cantidad].years = atoi(token);

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

int main() {
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
