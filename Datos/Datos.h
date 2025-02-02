// Datos/Datos.h
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class FileHandler {
	private:
		string nombreArchivo; // Nombre del archivo
		
	public:
		// Constructor para inicializar el nombre del archivo
		FileHandler(const string& nombreArchivo) : nombreArchivo(nombreArchivo) {}

		// Método para agregar texto al archivo sin borrar el contenido existente
		void agregarAlArchivo(const string& texto);
		void agregarAlArchivo(const vector<string>& lineas);

		// Método para sobrescribir el contenido del archivo con nuevo texto
		void sobrescribirArchivo(const string& texto);
		void sobrescribirArchivo(const vector<string>& lineas);
};