#include "tdas/list.h"
#include "tdas/map.h"
#include "tdas/extra.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
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

void limpiarBufferEntrada();

// Estructura para el libro
typedef struct
{
    char id[MAX_STR];
    char titulo[MAX_STR];
    char autor[MAX_STR];
    char genero[MAX_STR];
    int anio;
    int estado;
} Libro;

// Estructura para el un usuario
typedef struct
{
    int idUsuario;           // Identificador único
    List *prestamosActuales; // Lista enlazada de libros actualmente prestados
    List *historial;         // Lista enlazada de todos los libros que ha tomado
} Usuario;

char *normalizar(const char *str)
{
    static char normalizado[MAX_STR];
    int j = 0;

    for (int i = 0; str[i] != '\0' && j < MAX_STR - 1; i++)
    {
        if ((unsigned char)str[i] == 0xC3)
        {
            i++;
            if (str[i] == '\0')
                break;
            switch ((unsigned char)str[i])
            {
            case 0xA1:
                normalizado[j++] = 'a';
                break; // á
            case 0xA9:
                normalizado[j++] = 'e';
                break; // é
            case 0xAD:
                normalizado[j++] = 'i';
                break; // í
            case 0xB3:
                normalizado[j++] = 'o';
                break; // ó
            case 0xBA:
                normalizado[j++] = 'u';
                break; // ú
            case 0x81:
                normalizado[j++] = 'a';
                break;
            case 0x89:
                normalizado[j++] = 'e';
                break;
            case 0x8D:
                normalizado[j++] = 'i';
                break;
            case 0x93:
                normalizado[j++] = 'o';
                break;
            case 0x9A:
                normalizado[j++] = 'u';
                break;
            case 0xB1:
                normalizado[j++] = 'n';
                break; // ñ
            case 0x91:
                normalizado[j++] = 'n';
                break;
            default:
                break;
            }
        }
        else
        {
            char c = str[i];
            if (c >= 'A' && c <= 'Z')
                c += 32; // a minúscula
            normalizado[j++] = c;
        }
    }
    normalizado[j] = '\0';

    // Trim al final
    while (j > 0 && (normalizado[j - 1] == ' ' || normalizado[j - 1] == '\t'))
        normalizado[--j] = '\0';

    return normalizado;
}

int is_equal_str(void *key1, void *key2)
{
    char *a = (char *)key1;
    char *b = (char *)key2;
    int result = strcmp(a, b) == 0;
    return result;
}

int is_equal_int(void *key1, void *key2) //  Esta función recibe dos punteros que se espera que apunten a enteros
{
    return *(int *)key1 == *(int *)key2; // Se convierten los punteros void * a punteros a int, se accede al valor entero apuntado por key1, se comparan los valores enteros que contienen ambos punteros.
}

// esta funcion carga un archivo CSV y llena 5 mapas distintos
void cargarCatalogo(const char *nombreArchivo, Map *mapaAnioPublicacion, Map *mapaCategoria, Map *mapaTitulo, Map *mapaAutor, Map *mapaId)
{
    FILE *archivo = fopen(nombreArchivo, "r"); // se abre el archivo
    if (archivo == NULL)                       // si no se pudo abrir el archivo
    {
        perror("Error al abrir el archivo"); // mostramos error
        printf("Ruta esperada: %s\n", nombreArchivo);
        printf("Presione una tecla para salir...\n");
        getch(); // Pausa
        return;
    }

    leer_linea_csv(archivo, ','); // salta la primera linea delCSV

    char **campos; // almacenara los datos leidos desde cada linea

    while ((campos = leer_linea_csv(archivo, ',')) != NULL) // Lee línea por línea el archivo CSV, separando por comas
    {
        // si falta algun dato importante salta la linea
        if (campos[0] == NULL || campos[1] == NULL || campos[2] == NULL ||
            campos[3] == NULL || campos[4] == NULL)
        {
            // free(campos);
            continue;
        }

        Libro *newBook = malloc(sizeof(Libro)); // reserva memoria para el nuevo libro
        if (newBook == NULL)
        {
            // free(campos);
            fclose(archivo);
            return;
        }
        // copiamos los datos de csv al nuevo libro
        strcpy(newBook->id, campos[0]);
        strcpy(newBook->titulo, campos[1]);
        strcpy(newBook->autor, campos[2]);
        newBook->anio = atoi(campos[3]);
        strcpy(newBook->genero, campos[4]);
        newBook->estado = DISPONIBLE;

        // reserva memoria para las claves tipo int
        int *key_anio = malloc(sizeof(int));
        *key_anio = newBook->anio;

        char *key_id = strdup(normalizar(newBook->id));

        // inserta el libro por mapa
        map_insert(mapaAnioPublicacion, key_anio, newBook);
        map_insert(mapaCategoria, strdup(normalizar(newBook->genero)), newBook);
        map_insert(mapaTitulo, strdup(normalizar(newBook->titulo)), newBook);
        map_insert(mapaAutor, strdup(normalizar(newBook->autor)), newBook);
        map_insert(mapaId, strdup(campos[0]), newBook);
        // free(campos);
    }
    fclose(archivo);
    printf("Se ha cargado de manera exitosa el catalogo de la biblioteca digital!");
    return;
}

void registrarUsuario(Map *mapUsers)
{
    int id;
    printf("Ingrese el ID del nuevo usuario: ");
    scanf("%d", &id);

    Usuario *newUsuario = malloc(sizeof(Usuario));
    if (newUsuario == NULL)
        return;
    newUsuario->idUsuario = id;
    newUsuario->prestamosActuales = list_create();
    newUsuario->historial = list_create();
    int *key_id = malloc(sizeof(int));
    *key_id = newUsuario->idUsuario;

    map_insert(mapUsers, key_id, newUsuario);

    printf("Usuario con ID %d registrado exitosamente.\n", id);
}

void buscarTitulo(Map *librosPorTitulo)
{
    char titulo[MAX_STR];
    char titulo_normalizado[MAX_STR];

    printf("Ingrese el titulo del libro a buscar: ");
    fgets(titulo, MAX_STR, stdin);
    titulo[strcspn(titulo, "\n")] = 0;
    strcpy(titulo_normalizado, normalizar(titulo));
    MapPair *pair = map_search(librosPorTitulo, titulo_normalizado);

    if (pair == NULL)
    {
        printf("No se encontró el libro .(\n");
        return;
    }

    Libro *libro = (Libro *)pair->value;

    printf("Libro encontrado:\n");
    printf("Título: %s\n", libro->titulo);
    printf("Autor: %s\n", libro->autor);
    printf("Género: %s\n", libro->genero);
    printf("Año de publicación: %d\n", libro->anio);
}

void buscarAutor(Map *librosPorAutor)
{
    char autor[MAX_STR];
    printf("Ingrese el nombre autor a buscar: ");
    fgets(autor, MAX_STR, stdin);
    autor[strcspn(autor, "\n")] = 0;
    strcpy(autor, normalizar(autor));
    MapPair *pair = map_first(librosPorAutor);
    int encontrado = 0;
    printf("\nLibros por autor: %s\n", autor);
    while (pair != NULL)
    {
        Libro *libro = (Libro *)pair->value;
        if (strcmp(normalizar(libro->autor), autor) == 0)
        {
            printf("\nID: %d\n", libro->id);
            printf("Titulo: %s\n", libro->titulo);
            printf("Autor: %s\n", libro->autor);
            printf("Genero: %s", libro->genero);
            printf("years: %d\n", libro->anio);
            encontrado = 1;
        }
        pair = map_next(librosPorAutor);
    }
    if (!encontrado)
    {
        printf("No se encontre libro por autor");
    }
}

void buscarGenero(Map *librosPorGenero)
{
    char genero[MAX_STR];
    printf("Ingrese el nombre del a buscar: ");
    fgets(genero, MAX_STR, stdin);
    genero[strcspn(genero, "\n")] = 0;
    strcpy(genero, normalizar(genero));
    MapPair *pair = map_first(librosPorGenero);
    int encontrado = 0;
    printf("\nLibros por genero: %s\n", genero);
    while (pair != NULL)
    {
        Libro *libro = (Libro *)pair->value;
        if (strcmp(normalizar(libro->genero), genero) == 0)
        {
            printf("\nID: %d\n", libro->id);
            printf("Titulo: %s\n", libro->titulo);
            printf("Autor: %s\n", libro->autor);
            printf("Genero: %s", libro->genero);
            printf("Anio de publicacion: %d\n", libro->anio);
            encontrado = 1;
        }
        pair = map_next(librosPorGenero);
    }
    if (!encontrado)
    {
        printf("No se encontro libro por genero");
    }
}

void buscarAnio(Map *librosPorAnio)
{
    char input[MAX_STR];
    printf("Ingrese el año a buscar: ");
    fgets(input, MAX_STR, stdin);
    input[strcspn(input, "\n")] = 0;

    int anio = atoi(input);
    MapPair *pair = map_search(librosPorAnio, &anio);

    if (!pair)
    {
        printf("No se encontró libro por año\n");
        return;
    }

    printf("\nLibro encontrado:\n");
    Libro *libro = pair->value;
    printf("ID: %d\n", libro->id);
    printf("Título: %s\n", libro->titulo);
    printf("Autor: %s\n", libro->autor);
    printf("Género: %s\n", libro->genero);
    printf("Año de publicación: %d\n", libro->anio);
}

void RealizarPrestamo(Map *mapaUsuarios, Map *mapaId)
{
    int idUsuario;
    char idLibro[MAX_STR];

    printf("Ingrese su ID de usuario: ");
    scanf("%d", &idUsuario);

    MapPair *pairUsuario = map_search(mapaUsuarios, &idUsuario);
    if (pairUsuario == NULL)
    {
        printf("Usuario no encontrado.\n");
        return;
    }
    Usuario *usuario = (Usuario *)pairUsuario->value;

    limpiarBufferEntrada();
    printf("Ingrese el ID del libro que desea solicitar (ej. B0015): ");
    fgets(idLibro, MAX_STR, stdin);
    idLibro[strcspn(idLibro, "\n")] = 0;

    char *clave = strdup(idLibro); 
    MapPair *pairLibro = map_search(mapaId, clave);
    free(clave);

    if (pairLibro == NULL)
    {
        printf("Libro no encontrado en el catálogo.\n");
        return;
    }

    Libro *libro = (Libro *)pairLibro->value;

    if (libro->estado == PRESTADO)
    {
        printf("El libro ya está prestado.\n");
        return;
    }

    libro->estado = PRESTADO;
    list_pushBack(usuario->prestamosActuales, libro);
    list_pushBack(usuario->historial, libro);

    printf("El libro '%s' ha sido prestado con éxito al usuario ID %d.\n", libro->titulo, idUsuario);
}

void devolverLibro(Map *mapaUsuarios)
{
    int idUsuario;
    char titulo[MAX_STR];

    printf("📤 Registrar Devolución\n");

    // Solicitar ID del usuario
    printf("Ingrese ID de usuario: ");
    scanf("%d", &idUsuario);
    limpiarBufferEntrada(); // limpia salto de línea pendiente

    // Buscar usuario
    MapPair *pairUsuario = map_search(mapaUsuarios, &idUsuario);
    if (pairUsuario == NULL) {
        printf("Usuario no encontrado.\n");
        return;
    }

    Usuario *usuario = (Usuario *)pairUsuario->value;

    // Verificar si tiene libros prestados
    if (list_size(usuario->prestamosActuales) == 0) {
        printf("El usuario no tiene libros prestados actualmente.\n");
        return;
    }

    // Mostrar libros prestados
    printf("Libros actualmente prestados:\n");
    Libro *libro = list_first(usuario->prestamosActuales);
    while (libro != NULL) {
        printf("- %s\n", libro->titulo);
        libro = list_next(usuario->prestamosActuales);
    }

    // Pedir título del libro a devolver
    printf("Ingrese título del libro a devolver: ");
    fgets(titulo, MAX_STR, stdin);
    titulo[strcspn(titulo, "\n")] = '\0';

    // Buscar el libro en prestamosActuales
    List *nuevaLista = list_create();
    int encontrado = 0;
    libro = list_first(usuario->prestamosActuales);

    while (libro != NULL) {
        if (strcmp(libro->titulo, titulo) == 0 && !encontrado) {
            libro->estado = DISPONIBLE;
            list_pushBack(usuario->historial, libro); // sigue en historial
            encontrado = 1;
        } else {
            list_pushBack(nuevaLista, libro);
        }
        libro = list_next(usuario->prestamosActuales);
    }

    if (!encontrado) {
        printf("No se encontró el libro '%s' en los préstamos del usuario.\n", titulo);
        list_clean(nuevaLista);
        return;
    }

    // Reemplazar la lista de préstamos con la nueva lista
    list_clean(usuario->prestamosActuales);
    usuario->prestamosActuales = nuevaLista;

    printf("[✔] Devolución registrada correctamente.\n");
    printf("Presione cualquier tecla para volver al menú...\n");
    getch();
}

void int_to_ptr(int n)
{
    return (void)(intptr_t)n;
}

int ptr_to_int(void *ptr)
{
    return (int)(intptr_t)ptr;
}
void consultarPerfilUsario(Map *Usuarios)
{
    int id;
    printf("Ingrese ID usuario: ");
    scanf("%d", &id);

    int *key = malloc(sizeof(int));
    *key = id;
    MapPair *pair = map_search(Usuarios, key);
    free(key);

    if (pair == NULL)
    {
        printf(" Usario con ID %d no encontrado.\n", id);
        return;
    }
    Usuario *usuario = (Usuario *)pair->value;
    printf("\n Perfil del Usuario ID: %d\n", id);

    printf("\n Libros actualmente prestados:\n");
    if (list_size(usuario->prestamosActuales) == 0)
    {
        printf(" - NO tiene libros prestados actualmente.\n");
    }
    else
    {
        Libro *libro = list_first(usuario->prestamosActuales);
        while (libro != NULL)
        {
            printf("%s - %s - %s - %d\n", libro->titulo, libro->autor, libro->genero, libro->anio);
            libro = list_next(usuario->prestamosActuales);
        }
    }
    printf("\n Historial de prestamos:\n");
    if (list_size(usuario->historial) == 0)
    {
        printf("No tiene libros en historial.\n");
    }
    else
    {
        Libro *libro = list_first(usuario->historial);
        while (libro != NULL)
        {
            printf("%s - %s - %s - %d\n", libro->titulo, libro->autor, libro->genero, libro->anio);
            libro = list_next(usuario->historial);
        }
    }
}
void mostrar_catalogo(Map *mapaTitulo)
{
    MapPair *pair = map_first(mapaTitulo);
    if (!pair)
    {
        printf("No hay libros en el catálogo.\n");
        return;
    }

    printf("Catálogo de libros:\n");
    while (pair != NULL)
    {
        Libro *libro = (Libro *)pair->value;
        printf("\nID: %d\n", libro->id);
        printf("Título: %s\n", libro->titulo);
        printf("Autor: %s\n", libro->autor);
        printf("Género: %s\n", libro->genero);
        printf("Año de publicación: %d\n", libro->anio);
        printf("Estado: %s\n", libro->estado == DISPONIBLE ? "Disponible" : "Prestado");

        pair = map_next(mapaTitulo);
    }
}

bool libroYaEnLista(List *lista, Libro *libro) {
    Libro *actual = list_first(lista);
    while (actual != NULL) {
        if (strcmp(actual->titulo, libro->titulo) == 0 &&
            strcmp(actual->autor, libro->autor) == 0) {
            return true;
        }
        actual = list_next(lista);
    }
    return false;
}

void ver_historial(Map *mapaUsuarios)
{
    int id;
    printf("Ingrese ID de usuario para ver historial: ");
    scanf("%d", &id);
    limpiarBufferEntrada();

    MapPair *pair = map_search(mapaUsuarios, &id);
    if (pair == NULL)
    {
        printf("Usuario no encontrado.\n");
        return;
    }

    Usuario *usuario = (Usuario *)pair->value;
    printf("\n📑 Historial de préstamos para el usuario %d:\n", id);

    if (list_size(usuario->historial) == 0)
    {
        printf("No hay libros en el historial.\n");
        return;
    }

    // Lista temporal para no repetir libros
    List *vistos = list_create();
    Libro *libro = list_first(usuario->historial);
    int contador = 0;

    while (libro != NULL)
    {
        if (!libroYaEnLista(vistos, libro))
        {
            printf("- %s | %s | %s | %d\n", libro->titulo, libro->autor, libro->genero, libro->anio);
            list_pushBack(vistos, libro);
            contador++;
        }
        libro = list_next(usuario->historial);
    }

    if (contador == 0)
        printf("No hay libros únicos en el historial.\n");

    list_clean(vistos); // Liberar lista temporal
}

void limpiarBufferEntrada()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
// Función para detectar teclas especiales (como flechas)
int get_key()
{
    int ch = getch();
    if (ch == 0 || ch == 224)
    { // Tecla especial
        ch = getch();
        switch (ch)
        {
        case 72:
            return KEY_UP;
        case 80:
            return KEY_DOWN;
        case 75:
            return KEY_LEFT;
        case 77:
            return KEY_RIGHT;
        case 13:
            return KEY_ENTER;
        default:
            return ch;
        }
    }
    return ch;
}

// Función para mostrar el menú
void mostrar_menu(int opcion_seleccionada)
{
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

    for (int i = 0; i < 8; i++)
    {
        if (i == opcion_seleccionada)
        {
            printf("> "); // Indicador de selección
        }
        else
        {
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

    for (int i = 0; i < 4; i++)
    {
        if (i == opcionSeleccionada)
        {
            printf("> "); // Indicador de selección
        }
        else
        {
            printf("  ");
        }
        printf("%s\n", opciones[i]);
    }
}

// Menú principal interactivo
void menu_principal(Map *mapaUsuarios, Map *mapaAnioPublicacion, Map *mapaCategoria, Map *mapaTitulo, Map *mapaAutor, Map *mapaId)
{
    int opcion_seleccionada = 0;
    int tecla;

    do
    {
        mostrar_menu(opcion_seleccionada);
        tecla = get_key();

        switch (tecla)
        {
        case KEY_UP:
            if (opcion_seleccionada > 0)
                opcion_seleccionada--;
            break;
        case KEY_DOWN:
            if (opcion_seleccionada < 7)
                opcion_seleccionada++;
            break;
        case KEY_ENTER:
            system("cls");
            switch (opcion_seleccionada)
            {
            case 0: // Cargar catalogo
                cargarCatalogo("data/catalogo.csv", mapaAnioPublicacion, mapaCategoria, mapaTitulo, mapaAutor, mapaId);
                break;
            case 1: // registrar Usuario
                registrarUsuario(mapaUsuarios);
                break;
            case 2:
            { // Buscar libro
                int sub_opcion = 0;
                int tecla_busqueda;

                do
                {
                    MenuLibro(sub_opcion);
                    tecla_busqueda = get_key();

                    switch (tecla_busqueda)
                    {
                    case KEY_UP:
                        if (sub_opcion > 0)
                            sub_opcion--;
                        break;
                    case KEY_DOWN:
                        if (sub_opcion < 3)
                            sub_opcion++;
                        break;
                    case KEY_ENTER:
                        system("cls");
                        switch (sub_opcion)
                        {
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
                        printf("\nPresione cualquier tecla para volver al menú de búsqueda...");
                        getch();
                        break;
                    }
                } while (tecla_busqueda != KEY_ESC);
                break;
            }
            case 3: // Registrar préstamo
                RealizarPrestamo(mapaUsuarios, mapaId);
                break;
            case 4: // Registrar devolución
                devolverLibro(mapaUsuarios);
                break;
            case 5: // Mostrar catálogo
                mostrar_catalogo(mapaTitulo);
                break;
            case 6: // Ver historial
                ver_historial(mapaUsuarios);
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
    Map *mapaUsuarios = map_create(is_equal_int);
    Map *mapaAnioPublicacion = map_create(is_equal_int);
    Map *mapaCategoria = map_create(is_equal_str);
    Map *mapaTitulo = map_create(is_equal_str);
    Map *mapaAutor = map_create(is_equal_str);
    Map *mapaId = map_create(is_equal_str);

    // Llamar al menú principal
    menu_principal(mapaUsuarios, mapaAnioPublicacion, mapaCategoria, mapaTitulo, mapaAutor, mapaId);

    return 0;
}