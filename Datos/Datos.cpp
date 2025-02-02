// Datos/Datos.cpp
#include "Datos.h"

using namespace std;

// Implementación del método para agregar texto al archivo (una sola cadena)
void FileHandler::agregarAlArchivo(const string& texto) {
    ofstream archivo;
    archivo.open(nombreArchivo, std::ios_base::app); // Abre el archivo en modo de agregación
    if (archivo.is_open()) {
        archivo << texto << endl;
        archivo.close();
    } else {
        cerr << "Error al abrir el archivo " << nombreArchivo << endl;
    }
}

// Implementación del método para agregar texto al archivo (vector de cadenas)
void FileHandler::agregarAlArchivo(const vector<string>& lineas) {
    ofstream archivo;
    archivo.open(nombreArchivo, std::ios_base::app); // Abre el archivo en modo de agregación
    if (archivo.is_open()) {
        for (const string& linea : lineas) {
            archivo << linea << endl;
        }
        archivo.close();
    } else {
        cerr << "Error al abrir el archivo " << nombreArchivo << endl;
    }
}

// Implementación del método para sobrescribir el contenido del archivo (una sola cadena)
void FileHandler::sobrescribirArchivo(const string& texto) {
    ofstream archivo;
    archivo.open(nombreArchivo, std::ios_base::out); // Abre el archivo en modo de salida (sobrescribe)
    if (archivo.is_open()) {
        archivo << texto << endl;
        archivo.close();
    } else {
        cerr << "Error al abrir el archivo " << nombreArchivo << endl;
    }
}

// Implementación del método para sobrescribir el contenido del archivo (vector de cadenas)
void FileHandler::sobrescribirArchivo(const vector<string>& lineas) {
    ofstream archivo;
    archivo.open(nombreArchivo, std::ios_base::out); // Abre el archivo en modo de salida (sobrescribe)
    if (archivo.is_open()) {
        for (const string& linea : lineas) {
            archivo << linea << endl;
        }
        archivo.close();
    } else {
        cerr << "Error al abrir el archivo " << nombreArchivo << endl;
    }
}