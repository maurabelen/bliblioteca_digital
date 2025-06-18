int main() {
    Libro catalogo[MAX_LIBROS];
    int totalLibros = cargarCatalogo("catalogo.csv", catalogo, MAX_LIBROS);

    printf("Se cargaron %d libros.\n", totalLibros);
    for (int i = 0; i < totalLibros && i < 5; i++) {
        printf("Libro %d: %s (%d) - %s\n", catalogo[i].id, catalogo[i].titulo, catalogo[i].anio, catalogo[i].autor);
    }

    return 0;
}
