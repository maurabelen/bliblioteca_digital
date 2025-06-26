#include "tdas/list.h"
#include "tdas/map.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_STR 100
#define MAX_LIBROS 1000
#define MAX_USUARIOS 100
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER 13
#define KEY_ESC 27


// Función para detectar teclas especiales (como flechas)
int get_key() {
    int ch = getch();
    if (ch == 0 || ch == 224) { // Tecla especial
        ch = getch();
        switch(ch) {
            case 72: return KEY_UP;
            case 80: return KEY_DOWN;
            case 75: return KEY_LEFT;
            case 77: return KEY_RIGHT;
            case 13: return KEY_ENTER;
            default: return ch;
        }
    }
    return ch;
}

// Función para mostrar el menú
void mostrar_menu(int opcion_seleccionada) {
    system("cls"); // Limpiar pantalla (Windows)
    // system("clear"); // Para Linux/macOS
    
    printf("===== Biblioteca Digital =====\n\n");
    
    char *opciones[] = {
        "1. Registrar nuevo usuario",
        "2. Buscar libro",
        "3. Registrar préstamo",
        "4. Registrar devolución",
        "5. Ver historial de préstamos",
        "0. Salir"
    };
    
    for (int i = 0; i < 8; i++) {
        if (i == opcion_seleccionada) {
            printf("> "); // Indicador de selección
        } else {
            printf("  ");
        }
        printf("%s\n", opciones[i]);
    }
    
    printf("\n=============================\n");
    printf("Use las flechas ↑ ↓ para navegar y ENTER para seleccionar\n");
}

// Menú principal interactivo
void menu_principal(Map *mapaTitulo, Map *mapaAutor, Map *mapaId, Usuario *usuariosHash[]) {
    int opcion_seleccionada = 0;
    int tecla;
    
    do {
        mostrar_menu(opcion_seleccionada);
        tecla = get_key();
        
        switch(tecla) {
            case KEY_UP:
                if (opcion_seleccionada > 0) opcion_seleccionada--;
                break;
            case KEY_DOWN:
                if (opcion_seleccionada < 7) opcion_seleccionada++;
                break;
            case KEY_ENTER:
                system("cls"); // Limpiar pantalla
                switch(opcion_seleccionada) {
                    case 0: // Agregar libro
                        // agregar_libro(mapaTitulo, mapaAutor, mapaId);
                        break;
                    case 1: // Buscar por título
                        // buscar_por_titulo(mapaTitulo);
                        break;
                    case 2: // Buscar por autor
                        // buscar_por_autor(mapaAutor);
                        break;
                    case 3: // Registrar préstamo
                        // registrar_prestamo(usuariosHash, mapaTitulo);
                        break;
                    case 4: // Registrar devolución
                        // registrar_devolucion(usuariosHash, mapaTitulo);
                        break;
                    case 5: // Mostrar catálogo
                        // mostrar_catalogo();
                        break;
                    case 6: // Ver historial
                        // ver_historial();
                        break;
                    case 7: // Salir
                        printf("Saliendo del sistema...\n");
                        return;
                }
                printf("\nPresione cualquier tecla para continuar...");
                getch();
                break;
        }
    } while (tecla != KEY_ESC); // Puedes agregar ESC como salida alternativa
}



int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2;
}

// Estructura para el libro
typedef struct {
    int id;
    char titulo[MAX_STR];
    char autor[MAX_STR];
    char genero[MAX_STR];
    int anio;
    bool bookState;
} Libro;

// Estructura para el un usuario
typedef struct {
    int idUsuario;              // Identificador único
    List *prestamosActuales;    // Lista enlazada de libros actualmente prestados
    List *historial;            // Lista enlazada de todos los libros que ha tomado
} Usuario;

int cargarCatalogo(const char *nombreArchivo, Libro catalogo[], int maxLibros, 
                  Map* mapaAño, Map* mapaCategoria, Map* mapaTitulo, 
                  Map* mapaAutor, Map* mapaId) {
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
    nuevo->prestamosActuales = list_create();
    nuevo->historial = list_create();

    usuariosHash[pos] = nuevo;

    printf("Usuario con ID %d registrado exitosamente.\n", id);
}

// Modificación del main para usar el menú interactivo
int main() {
    
    // Inicialización de estructuras (como antes)
    Map *mapaUsuarios = map_create(is_equal_str);
    Map *mapaAñoPublicacion = map_create(is_equal_str);
    Map *mapaCategoria = map_create(is_equal_str);
    Map *mapaTitulo = map_create(is_equal_str);
    Map *mapaAutor = map_create(is_equal_str);
    Map *mapaId = map_create(is_equal_int);

    Libro catalogo[MAX_LIBROS];
    int totalLibros = cargarCatalogo("tdas/catalogo.csv", catalogo, MAX_LIBROS, 
                                   mapaAñoPublicacion, mapaCategoria, mapaTitulo, 
                                   mapaAutor, mapaId);

    Usuario *usuariosHash[MAX_USUARIOS] = {0};
    
    // Llamar al menú principal
    menu_principal(mapaTitulo, mapaAutor, mapaId, usuariosHash);
    
    return 0;
}
