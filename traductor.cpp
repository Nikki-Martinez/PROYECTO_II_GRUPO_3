#include <iostream>
using namespace std;

// Estructura para representar una palabra
struct Palabra {
    char palabra[50];
    char traduccion[50];
    char funcionalidad[256];
};

// Nombre del archivo binario
const char *nombre_archivo = "traductor.dat";

// Declaraciones de funciones
void Crear();
void Leer();
void Actualizar();
void Borrar();

int main() {
    int opcion;
    do {
        cout << "\n------ Traductor ------\n";
        cout << "1. Crear\n";
        cout << "2. Leer\n";
        cout << "3. Actualizar\n";
        cout << "4. Borrar\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1: Crear(); break;
            case 2: Leer(); break;
            case 3: Actualizar(); break;
            case 4: Borrar(); break;
            case 0: cout << "Saliendo del programa...\n"; break;
            default: cout << "Opcion no valida. Intente nuevamente.\n";
        }
    } while (opcion != 0);

    system("pause");
    return 0;
}

// Función para crear nuevos registros
void Crear() {
    FILE* archivo = fopen(nombre_archivo, "a+b");
    if (!archivo) {
        cout << "No se pudo abrir el archivo.\n";
        return;
    }

    char res;
    Palabra palabra;
    do {
        cout << "\nIngrese Palabra: ";
        cin.getline(palabra.palabra, 50);

        cout << "Ingrese Traduccion: ";
        cin.getline(palabra.traduccion, 50);

        cout << "Ingrese Funcionalidad: ";
        cin.getline(palabra.funcionalidad, 256);

        fwrite(&palabra, sizeof(Palabra), 1, archivo);

        cout << "¿Desea ingresar otra palabra? (s/n): ";
        cin >> res;
        cin.ignore();
    } while (res == 's' || res == 'S');

    fclose(archivo);
    Leer();
}

// Función para leer y mostrar registros
void Leer() {
    system("cls");
    FILE* archivo = fopen(nombre_archivo, "rb");
    if (!archivo) {
        archivo = fopen(nombre_archivo, "w+b");
    }

    Palabra palabra;
    int id = 0;

    cout << "\nID | Palabra | Traduccion | Funcionalidad\n";
    cout << "-----------------------------------------------------------\n";

    fread(&palabra, sizeof(Palabra), 1, archivo);
    while (!feof(archivo)) {
        cout << id << " | " << palabra.palabra << " | " << palabra.traduccion
             << " | " << palabra.funcionalidad << endl;
        fread(&palabra, sizeof(Palabra), 1, archivo);
        id++;
    }

    fclose(archivo);
}

// Función para actualizar un registro por ID
void Actualizar() {
    FILE* archivo = fopen(nombre_archivo, "r+b");
    if (!archivo) {
        cout << "No se pudo abrir el archivo.\n";
        return;
    }

    int id;
    cout << "Ingrese el ID de la palabra a actualizar: ";
    cin >> id;
    cin.ignore();

    Palabra palabra;

    fseek(archivo, id * sizeof(Palabra), SEEK_SET);

    cout << "\nIngrese nueva Palabra: ";
    cin.getline(palabra.palabra, 50);

    cout << "Ingrese nueva Traduccion: ";
    cin.getline(palabra.traduccion, 50);

    cout << "Ingrese nueva Funcionalidad: ";
    cin.getline(palabra.funcionalidad, 256);

    fwrite(&palabra, sizeof(Palabra), 1, archivo);

    fclose(archivo);
    Leer();
}

// Función para borrar un registro por ID
void Borrar() {
    const char *nombre_temp = "temp.dat";
    FILE* archivo = fopen(nombre_archivo, "rb");
    FILE* archivo_temp = fopen(nombre_temp, "w+b");

    if (!archivo || !archivo_temp) {
        cout << "No se pudo abrir los archivos.\n";
        return;
    }

    int id_borrar;
    int id_actual = 0;

    cout << "Ingrese el ID que desea eliminar: ";
    cin >> id_borrar;

    Palabra palabra;
    while (fread(&palabra, sizeof(Palabra), 1, archivo)) {
        if (id_actual != id_borrar) {
            fwrite(&palabra, sizeof(Palabra), 1, archivo_temp);
        }
        id_actual++;
    }

    fclose(archivo);
    fclose(archivo_temp);

    remove(nombre_archivo);
    rename(nombre_temp, nombre_archivo);

    Leer();
}
