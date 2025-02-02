// Presentacion/Menu.h
#pragma once
#include <vector>
#include <conio.h>
#include <iomanip>
#include <string>
#include <iostream>
#include <thread>
#include <sstream>
//#include <cstdlib>
#include <stdexcept>
#include <type_traits>
#include <chrono> // Para la funcion sleep
#include <atomic> // Para banderas atomicas

#define SEPARADOR "---------------------------------------------------------------------------------------------------------------------"
#define RESET "\033[0m" // Restablece el color de la fuente.
#define YELLOW "\033[33m" // Cambia el color de la fuente a amarillo.
#define PURPURA "\033[35m" // Cambia el color de la fuente a purpura.
#define CYAN "\033[36m" // Cambia el color de la fuente a cian.
#define RED "\033[31m" // Cambia el color de la fuente a rojo.
#define GREEN "\033[32m" // Cambia el color de la fuente a verde.
using namespace std;

class MenuUI {
private:
    int cursor;
    int consoleWidth; // Ancho de la consola para centrar el menu horizontalmente
    atomic<bool> loading; // Para detener la pantalla de carga
public:
    MenuUI(int width);
    enum Tecla { ARRIBA, ABAJO, ENTER, ESCAPE, OTRA };
    void mostrarMenu(const vector<string>& opcionesMenu) const;
    Tecla getTecla() const;
    void moverCursor(const vector<string>& opcionesMenu, MenuUI::Tecla tecla);
    int getCursor() const;
    void setCursor(int posicion);
    
    // Metodo para la pantalla de carga y su hilo
    void pantallaDeCarga();
    void iniciarPantallaDeCarga();
    void detenerPantallaDeCarga();
    
    // Metodos para transiciones
    void transicionDeslizante(const vector<string>& menuActual, const vector<string>& siguienteMenu) const;
    void mostrarMenuConTransicion(const vector<string>& menuActual, const vector<string>& siguienteMenu);
	
	bool confirmacion(const string& mensaje);
	
	// Metodo para solicitar datos al usuario
    template <typename T>
    T solicitarDato(const string& mensaje) const;
};

// Implementacion de la plantilla en el mismo archivo
template <typename T>
T MenuUI::solicitarDato(const string& mensaje) const {
        T dato;
        string entrada;

        // Mostrar el mensaje centrado
        system("cls");
        cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
        cout << setw((consoleWidth+25) / 2) << mensaje << endl;
        cout << setw((consoleWidth+25) / 2) << GREEN << "-> " << RESET;
        getline(cin, entrada);

        // Si estamos solicitando una cadena, devolvemos la entrada directamente
        if constexpr (is_same<T, string>::value) {
            return entrada;
        } else {
            stringstream ss(entrada);
            ss >> dato;

            // Revisar si la conversion fallo (para manejar correctamente los textos con espacios)
            if (ss.fail()) {
                cerr << "Error: La entrada no es valida." << endl;
                throw std::invalid_argument("Conversion fallida");
            }

            return dato;
        }      
    }