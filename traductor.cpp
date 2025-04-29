#include "librerias.h"
using namespace std;


struct Palabra {
    char palabra[50];
    char traduccion[50];
    char funcionalidad[256];
};


const char *nombre_archivo = "traductor.dat";


void Crear();
void Leer();
void Actualizar();
void Borrar();
void TraducirCodigo();

int main() {
    int opcion;
    do {
        cout << "------ Traductor ------" << endl;
        cout << "1. Crear" << endl;
        cout << "2. Leer" << endl;
        cout << "3. Actualizar" << endl;
        cout << "4. Borrar" << endl;
        cout << "5. Traducir codigo" << endl;
        cout << "0. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1: 
				Crear(); 
				break;
				
            case 2: 
			    Leer();
			    break;
			    
            case 3: 
				Actualizar(); 
				break;
				
            case 4: 
				Borrar(); 
				break;
				
            case 5: 
				TraducirCodigo(); 
				break;
				
            case 0: 
				cout << "Saliendo del programa... " << endl; 
				break;
				
            default: 
				cout << "Opcion no valida." << endl;
        }
    } while (opcion != 0);

    return 0;
}


void Crear() {
    FILE* archivo = fopen(nombre_archivo, "a+b");
    if (!archivo) {
        cout << "No se pudo abrir el archivo." << endl;
        return;
    }

    char res;
    Palabra palabra;
    do {
        cout << "Ingrese Palabra (clave en el codigo): ";
        cin.getline(palabra.palabra, 50);

        cout << "Ingrese Traduccion (al lenguaje destino): ";
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


void Leer() {
    FILE* archivo = fopen(nombre_archivo, "rb");
    if (!archivo) {
        archivo = fopen(nombre_archivo, "w+b");
    }

    Palabra palabra;
    int id = 0;

    cout << "ID | Palabra | Traduccion | Funcionalidad" << endl;
    cout << "-----------------------------------------------------------" << endl;

    fread(&palabra, sizeof(Palabra), 1, archivo);
    while (!feof(archivo)) {
        cout << id << " | " << palabra.palabra << " | " << palabra.traduccion
             << " | " << palabra.funcionalidad << endl;
        fread(&palabra, sizeof(Palabra), 1, archivo);
        id++;
    }

    fclose(archivo);
}


void Actualizar() {
    FILE* archivo = fopen(nombre_archivo, "r+b");
    if (!archivo) {
        cout << "No se pudo abrir el archivo." << endl;
        return;
    }

    int id;
    cout << "Ingrese el ID de la palabra a actualizar: ";
    cin >> id;
    cin.ignore();

    Palabra palabra;

    fseek(archivo, id * sizeof(Palabra), SEEK_SET);

    cout << "Ingrese nueva Palabra: ";
    cin.getline(palabra.palabra, 50);

    cout << "Ingrese nueva Traduccion: ";
    cin.getline(palabra.traduccion, 50);

    cout << "Ingrese nueva Funcionalidad: ";
    cin.getline(palabra.funcionalidad, 256);

    fwrite(&palabra, sizeof(Palabra), 1, archivo);

    fclose(archivo);
    Leer();
}


void Borrar() {
    const char *nombre_temp = "temp.dat";
    FILE* archivo = fopen(nombre_archivo, "rb");
    FILE* archivo_temp = fopen(nombre_temp, "w+b");

    if (!archivo || !archivo_temp) {
        cout << "No se pudo abrir los archivos." << endl;
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


void TraducirCodigo() {
    FILE* archivo = fopen(nombre_archivo, "rb");
    if (!archivo) {
        cout << "No se encontraron palabras guardadas para traducir." << endl;
        return;
    }

    vector<Palabra> palabras;
    Palabra palabra;

    while (fread(&palabra, sizeof(Palabra), 1, archivo)) {
        palabras.push_back(palabra);
    }
    fclose(archivo);

    vector<string> todosTextos;
    char continuar = 's';

    while (continuar == 's' || continuar == 'S') {
        cout << "Ingrese una porcion de codigo: " << endl;
        string input, line;
        int lineCount = 0;

        while (true) {
            getline(cin, line);
            if (line.empty()) {
                lineCount++;
                if (lineCount >= 2) break;
            } else {
                lineCount = 0;
            }
            input += line + "\n";
        }

        if (!input.empty()) {
            todosTextos.push_back(input);
            cout << "\nCódigo Ingresado:\n" << input;
        }

        cout << "\n¿Deseas ingresar mas codigo? (s/n): ";
        cin >> continuar;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "\n--- Traducciones ---\n";
    for (const auto& codigo : todosTextos) {
        string codigoTraducido = codigo;

        for (const auto& palabra : palabras) {
            size_t pos = 0;

       
            while ((pos = codigoTraducido.find(palabra.palabra, pos)) != string::npos) {
                
                bool esDelimitada =
                    (pos == 0 || !isalnum(codigoTraducido[pos - 1])) && 
                    (pos + strlen(palabra.palabra) == codigoTraducido.size() || !isalnum(codigoTraducido[pos + strlen(palabra.palabra)])); // No hay caracteres alfanuméricos después

                if (esDelimitada) {
                    codigoTraducido.replace(pos, strlen(palabra.palabra), palabra.traduccion);
                }

                pos += strlen(palabra.palabra); 
            }
        }

        cout << "\nOriginal:\n" << codigo;
        cout << "Traducido:\n" << codigoTraducido << endl;
    }
}

