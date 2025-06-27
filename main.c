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
    titulo[strcspn (titulo, "\n")] = 0;

    MapPair *pair = map_search(librosPorTitulo, titulo);

    if(pair == NULL){
        printf("No se encontre el libro .(\n)");
        return;
    }
    Libro *libro = (Libro *)pair->value;

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
    autor[strcspn (autor, "\n")] = 0;

    MapPair *pair = map_first(librosPorAutor);
    int encontrado = 0;
    printf("\nLibros por autor:\n",autor);
    while(pair != NULL){
        Libro *libro = (Libro *)pair->value;
        if(strcmp(libro->autor, autor) == 0) {
            printf("\nID: %d\n", libro->id);
            printf("Titulo: %s\n", libro->titulo);
            printf("Autor: %s\n", libro->autor);
            printf("Genero: %s", libro->genero);
            printf("years: %d\n", libro->anio);
            encontrado = 1;
        }
        pair = map_next(librosPorAutor);
    }
    if(!encontrado) {
        printf("No se encontre libro por autor");
    }
}

void buscarGenero(Map *librosPorGenero){
    char genero[MAX_STR];
    printf("Ingrese el nombre autor a buscar: ");
    getchar();
    fgets(genero,MAX_STR,stdin);
    genero[strcspn (genero, "\n")] = 0;

    MapPair *pair = map_first(librosPorGenero);
    int encontrado = 0;
    printf("\nLibros por autor:\n",genero);
    while(pair != NULL){
        Libro *libro = (Libro *)pair->value;
        if(strcmp(libro->autor, genero) == 0) {
            printf("\nID: %d\n", libro->id);
            printf("Titulo: %s\n", libro->titulo);
            printf("Autor: %s\n", libro->autor);
            printf("Genero: %s", libro->genero);
            printf("Anio de publicacion: %d\n", libro->anio);
            encontrado = 1;
        }
        pair = map_next(librosPorGenero);
    }
    if(!encontrado) {
        printf("No se encontro libro por genero");
    }
}


void buscarAnio(Map *librosPorAnio){
    char years[MAX_STR];
    printf("Ingrese el nombre autor a buscar: ");
    getchar();
    fgets(years,MAX_STR,stdin);
    years[strcspn (years, "\n")] = 0;
    
    MapPair *pair = map_first(librosPorAnio);
    int encontrado = 0;
    printf("\nLibros por autor:\n",years);
    while(pair != NULL){
        Libro *libro = (Libro *)pair->value;
        if(strcmp(libro->autor, years) == 0) {
            printf("\nID: %d\n", libro->id);
            printf("Titulo: %s\n", libro->titulo);
            printf("Autor: %s\n", libro->autor);
            printf("Genero: %s", libro->genero);
            printf("Anio de publicacion: %d\n", libro->anio);
            encontrado = 1;
        }
        pair = map_next(librosPorAnio);
    }
    if(!encontrado) {
        printf("No se encontro libro por anio");
    }

}


void RealizarPrestamo(Map *mapaUsuarios , Map *mapaId) 
{
    int idUsuario;
    int idLibro;

    printf("Ingrese su ID de usuario: ");
    scanf("%d", &idUsuario);

    MapPair *pairUsuario = map_search(mapaUsuarios , &idUsuario );
    if (pairUsuario == NULL) {
        printf("Usuario no encontrado.\n");
        return;
    }
    Usuario *usuario = (Usuario *)pairUsuario->value;
    

    printf("Ingrese el id del libro que desea solicitar: ");
    getchar(); // Limpiar buffer
    scanf("%d" , &idLibro);

    MapPair *pairLibro = map_search(mapaId, &idLibro);
    if (pairLibro == NULL) {
        printf("Libro no encontrado en el catálogo.\n");
        return;
    }

    Libro *libro = (Libro *)pairLibro->value;
    
    if(libro->estado == PRESTADO) {
        printf("El libro ya esta prestado.\n");
        return;
    }
    libro->estado = PRESTADO;

    // Agregar a lista de préstamos actuales del usuario
    list_pushBack(usuario->prestamosActuales, libro);

    // Agregar también al historial del usuario
    list_pushBack(usuario->historial, libro);

    printf("El libro '%s' ha sido prestado con éxito al usuario %d.\n", libro->titulo, idUsuario);
}


void devolverLibro(Usuario *usuarios[MAX_USUARIOS]){
    int id;
    printf("Ingrese su ID usario: ");
    scanf("%d",&id);

    int pos = id % MAX_USUARIOS;
    int original = pos;

    while(usuarios[pos] != NULL && usuarios[pos]->idUsuario != id){
        pos = (pos + 1) % MAX_USUARIOS;
        if(pos == original) {
            printf("Usuario no encontrado.\n");
            return;
        }
    }
    Usuario  *usuario = usuarios[pos];
    if(usuario == NULL){
        printf("Usuario no encontrado.\n");
        return;
    }
    if(list_size(usuario->prestamosActuales)== 0){
        printf("No tienes libros prestados actualmente.\n");
        return;
    }
    printf("\nLibros prestados:\n");
    List *prestamos = usuario->prestamosActuales;
    Libro *libro = list_first(prestamos);
    int i = 1;

    while(libro != NULL){
        printf("%d. %s\n", i++, libro->titulo);
        libro = list_next(prestamos);
    }

    char titulo[MAX_STR];
    printf("Ingrese el titulo dl libro a devolver: ");
    getchar();
    fgets(titulo, MAX_STR, stdin);
    titulo[strcspn(titulo, "\n")] = 0;
    List *newList = list_create();
    prestamos = usuario->prestamosActuales;
    libro = list_first(prestamos);
    
    int encontrado = 0;
    while(libro != NULL){
        if(strcmp(libro->titulo, titulo) == 0 && !encontrado){
            libro->estado = true;
            list_pushBack(usuario->historial,libro);
            encontrado = 1;
        } else {
            list_pushBack(newList,libro);
        }
        libro = list_next(prestamos);

    }
    if(!encontrado) {
        printf("No se encontro libro en prestamos.\n");
        list_clean(newList);
        return;
    }

    list_clean(usuario->prestamosActuales);
    usuario->prestamosActuales = newList;
    printf("Libro %s devuelto correctamente.\n",titulo);

}

void *int_to_ptr(int n) {
    return (void *)(long)n;
}


void consultarPerfilUsario(Map *Usuarios) {
    int id;
    printf("Ingrese ID usuario: ");
    scanf("%d", &id);

    void *key = int_to_ptr(id);
    MapPair *pair = map_search(Usuarios,key);
    

    if(pair == NULL){
        printf(" Usario con ID %d no encontrado.\n",id);
        return;
    }
    Usuario *usuario = (Usuario *)pair->value;
    printf("\n Perfil del Usuario ID: %d\n",id);

    printf("\n Libros actualmente prestados:\n");
    if(list_size(usuario->prestamosActuales) == 0) {
        printf(" - NO tiene libros prestados actualmente.\n");
    } else {
        Libro *libro = list_first(usuario->prestamosActuales);
        while(libro != NULL) {
            printf("%s - %s - %s - %d\n",libro->titulo,libro->autor,libro->genero,libro->anio);
            libro = list_next(usuario->prestamosActuales);
        }
    }
    printf("\n Historial de prestamos:\n");
    if(list_size(usuario->historial) == 0) {
        printf("No tiene libros en historial.\n");
    } else {
        Libro *libro = list_first(usuario->historial);
        while(libro != NULL){
            printf("%s - %s - %s - %d\n",libro->titulo,libro->autor,libro->genero,libro->anio);
            libro = list_next(usuario->historial);
        }
    }

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
    
    for (int i = 0; i < 4; i++) {
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
}

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
