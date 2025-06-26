#include "tdas/list.h"
#include "tdas/map.h"
#include "tdas/extra.h"
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
#define DISPONIBLE 1
#define PRESTADO 0


// Estructura para el libro
typedef struct {
    int id;
    char titulo[MAX_STR];
    char autor[MAX_STR];
    char genero[MAX_STR];
    int anio;
    int  estado;
} Libro;

// Estructura para el un usuario
typedef struct {
    int idUsuario;              // Identificador único
    List *prestamosActuales;    // Lista enlazada de libros actualmente prestados
    List *historial;            // Lista enlazada de todos los libros que ha tomado
} Usuario;

int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2;
}



void cargarCatalogo(const char *nombreArchivo , Map *mapaAnioPublicacion, Map * mapaCategoria, Map *mapaTitulo , Map *mapaAutor , Map *mapaId) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombreArchivo);
        return;
    }

    leer_linea_csv(archivo, ',');
    
    char **campos;

    while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
        
        if (campos[0] == NULL || campos[1] == NULL || campos[2] == NULL || 
            campos[3] == NULL || campos[4] == NULL) {
            free(campos);
            continue;
        }
        
        Libro *newBook = malloc(sizeof(Libro));
        if (newBook == NULL) {
            free(campos);
            fclose(archivo);
            return;
        }

        newBook->id = atoi(campos[0]);
        strcpy(newBook->titulo , campos[1]);
        strcpy(newBook->autor , campos[2]);
        newBook->anio =atoi(campos[3]);
        strcpy(newBook->genero , campos[4]);
        newBook->estado = DISPONIBLE;

        int *key_anio = malloc(sizeof(int));
        *key_anio = newBook->anio;

        int *key_id = malloc(sizeof(int));
        *key_id = newBook->id;

        map_insert(mapaAnioPublicacion , key_anio , newBook);
        map_insert(mapaCategoria , newBook->genero , newBook);
        map_insert(mapaTitulo , newBook->titulo , newBook);
        map_insert(mapaAutor , newBook->autor , newBook);
        map_insert(mapaId , key_id , newBook);
        free(campos);

    }
    fclose(archivo);
    printf("Se ha cargado de manera exitosa el catalogo de la biblioteca digital!");
    return;
    
}

void registrarUsuario(Map *mapUsers) {
    int id;
    printf("Ingrese el ID del nuevo usuario: ");
    scanf("%d", &id);

    Usuario *newUsuario = malloc(sizeof(Usuario));
    if (newUsuario == NULL) return;
    newUsuario->idUsuario = id;
    newUsuario->prestamosActuales = list_create();
    newUsuario->historial = list_create();
    int *key_id = malloc(sizeof(int));
    *key_id = newUsuario->idUsuario;

    
    map_insert(mapUsers , key_id , newUsuario);

    printf("Usuario con ID %d registrado exitosamente.\n", id);
}

void buscarTitulo(Map *librosPorTitulo){
    char titulo[MAX_STR];
    printf("Ingrese el titulo del libro a buscar: ");
    getchar();
    fgets(titulo,MAX_STR,stdin);
    titulo[strcpn(titulo, "\n")] = 0;

    Libro *libro = map_search(librosPorTitulo, titulo);

    if(libro == NULL){
        printf("No se encontre el libro .(\n)");
        return;
    }
    printf("Libro encontrado:\n");
    printf("Titulo: %s\n", libro->titulo);
    printf("Autor: %s\n", libro->autor);
    printf("Genero: %s", libro->genero);
    printf("years: %d\n", libro->anio);

}

void buscarAutor(Map *librosPorAutor){
    char autor[MAX_STR];
    printf("Ingrese el nombre autor a buscar: ");
    getchar();
    fgets(autor,MAX_STR,stdin);
    autor[strcpn(autor, "\n")] = 0;

    List *libros = map_search(librosPorAutor,autor);
    if(libros == NULL || list_size(libros) == 0){
        printf("No se encontraron libros del autor.\n");
    }
    printf("\nLibros por autor:\n",autor);
    Libro *libro = list_first(libros);
    while(libro != NULL){
        printf("\nID: %d\n", libro->id);
        printf("Titulo: %s\n", libro->titulo);
        printf("Autor: %s\n", libro->autor);
        printf("Genero: %s", libro->genero);
        printf("years: %d\n", libro->anio);
        libro = list_next(libros);
        }
}

void buscarGenero(Map *librosPorGenero){
    char genero[MAX_STR];
    printf("Ingrese el nombre autor a buscar: ");
    getchar();
    fgets(genero,MAX_STR,stdin);
    genero[strcpn(genero, "\n")] = 0;

    List *libros = map_search(librosPorGenero,genero);
    if(libros == NULL || list_size(libros) == 0){
        printf("No se encontraron libros del autor.\n");
    }
    printf("\nLibros por genero:\n",genero);
    Libro *libro = list_first(libros);
    while(libro != NULL){
        printf("\nID: %d\n", libro->id);
        printf("Titulo: %s\n", libro->titulo);
        printf("Autor: %s\n", libro->autor);
        printf("Genero: %s", libro->genero);
        printf("years: %d\n", libro->anio);
        libro = list_next(libros);
        }
}


void buscarAnio(Map *librosPorAnio){
    char years[MAX_STR];
    printf("Ingrese el nombre autor a buscar: ");
    getchar();
    fgets(years,MAX_STR,stdin);
    years[strcpn(years, "\n")] = 0;

    List *libros = map_search(librosPorAnio,years);
    if(libros == NULL || list_size(libros) == 0){
        printf("No se encontraron libros del autor.\n");
    }
    printf("\nLibros por anio:\n",years);
    Libro *libro = list_first(libros);
    while(libro != NULL){
        printf("\nID: %d\n", libro->id);
        printf("Titulo: %s\n", libro->titulo);
        printf("Autor: %s\n", libro->autor);
        printf("Genero: %s", libro->genero);
        printf("years: %d\n", libro->anio);
        libro = list_next(libros);
        }
}


void RealizarPrestamo(Map *mapaUsuarios , Map *mapaId) 
{
    int idUsuario;
    int idLibro;

    printf("Ingrese su ID de usuario: ");
    scanf("%d", &idUsuario);

    Usuario* usuario = map_search(mapaUsuarios , idUsuario );
    if (usuario == NULL) {
        printf("Usuario no encontrado.\n");
        return;
    }

    printf("Ingrese el id del libro que desea solicitar: ");
    getchar(); // Limpiar buffer
    scanf("%d" , &idLibro);

    Libro* libro = map_search(mapaId , idLibro);
    if (libro == NULL) {
        printf("Libro no encontrado en el catálogo.\n");
        return;
    }

    if (libro->estado == PRESTADO) {
        printf("El libro ya está prestamo actualmente.\n");
        return;
    }

    // Marcar libro como prestado
    libro->estado = PRESTADO;

    // Agregar a lista de préstamos actuales del usuario
    list_push_back(usuario->prestamosActuales, libro);

    // Agregar también al historial del usuario
    list_push_back(usuario->historial, libro);

    printf("El libro '%s' ha sido prestado con éxito al usuario %d.\n", libro->titulo, idUsuario);
}


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
    
    
    printf("===== Biblioteca Digital =====\n\n");
    
    char *opciones[] = {
        "1. Cargar catalogo",
        "2. Registrar nuevo usuario",
        "3. Buscar libro",
        "4. Registrar prestamo",
        "5. Registrar devolucion",
        "6. Ver historial de libros",
        "7. Ver perfil de usuarios",
        "0. Salir",
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
    printf("Use las flechas del tecaldo (las que apuntan en dirrecion hacia arriba y abajo) para navegar y ENTER para seleccionar\n");
}


void MenuLibro(int opcionSeleccionada)
{
    system("cls"); // Limpiar pantalla (Windows)
    
    
    
    char *opciones[] = {
        "1. Por titulo",
        "2. Por año de publicacion",
        "3. Por genero",
        "4. Por autor",
    };
    
    for (int i = 0; i < 8; i++) {
        if (i == opcionSeleccionada) {
            printf("> "); // Indicador de selección
        } else {
            printf("  ");
        }
        printf("%s\n", opciones[i]);
    }

}


// Menú principal interactivo
void menu_principal(Map *mapaUsuarios, Map *mapaAnioPublicacion, Map *mapaCategoria, Map *mapaTitulo , Map *mapaAutor , Map *mapaId) {
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
            system("cls"); 
            switch(opcion_seleccionada) {
                case 0: // Cargar catalogo
                    cargarCatalogo("data/catalogo.csv", mapaAnioPublicacion, mapaCategoria, mapaTitulo, mapaAutor, mapaId);
                    break;
                case 1: // registrar Usuario
                    registrarUsuario(mapaUsuarios);
                    break;
                case 2: // Buscar por autor
                    do {
                        MenuLibro(opcion_seleccionada);
                        tecla = get_key();
                        switch(tecla) {
                            case KEY_UP:
                                if (opcion_seleccionada > 0) opcion_seleccionada--;
                                break;
                            case KEY_DOWN:
                                if (opcion_seleccionada < 7) opcion_seleccionada++;
                                break;
                            case KEY_ENTER:
                                system("cls"); 
                                switch(opcion_seleccionada) {
                                    case 0:
                                        buscarTitulo(mapaTitulo);
                                        break;
                                    case 1:
                                        buscarAnio(mapaAnioPublicacion);
                                        break;
                                    case 2:
                                        buscarGenero(mapaCategoria);
                                        break;
                                    case 3:
                                        buscarAutor(mapaAutor);
                                        break;
                                }
                                break;
                        }
                    } while (tecla != KEY_ESC);
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
                    break;
            }
            printf("\nPresione cualquier tecla para continuar...");
            getch();
            break;
    }
} while (tecla != KEY_ESC); 

int main() 
{
    
    // Inicialización de estructuras (como antes)
    Map *mapaUsuarios = map_create(is_equal_str);
    Map *mapaAnioPublicacion = map_create(is_equal_str);
    Map *mapaCategoria = map_create(is_equal_str);
    Map *mapaTitulo = map_create(is_equal_str);
    Map *mapaAutor = map_create(is_equal_str);
    Map *mapaId = map_create(is_equal_int);
    
    // Llamar al menú principal
    menu_principal(mapaUsuarios, mapaAnioPublicacion, mapaCategoria, mapaTitulo , mapaAutor , mapaId );
    
    return 0;
}
