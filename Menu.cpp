// Presentacion/Menu.cpp
#include "Menu.h"

MenuUI::MenuUI(int width)
    : cursor(0), consoleWidth(width) {}

void MenuUI::mostrarMenu(const vector<string>& opcionesMenu) const {
    system("cls");
    cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
    for(size_t i = 0; i < opcionesMenu.size(); ++i) { // Imprime todas las opciones del menu.
        cout << std::setw(((consoleWidth+5) / 2)) << ""; // Asegura que todas empiezan en el mismo margen izquierdo
        if(i == cursor) { // Coloca una flecha en la posicion del cursor.
            cout << YELLOW << "->>" << RESET;
        } else { 
            cout << "   ";
        }
        cout << opcionesMenu[i] << endl;
    }
    cout << endl << PURPURA << SEPARADOR << RESET << endl;
}

MenuUI::Tecla MenuUI::getTecla() const {
    int c = _getch();
    switch (c) {
        case 72:
            return MenuUI::ARRIBA;
        case 80:
            return MenuUI::ABAJO;
        case 13:
            return MenuUI::ENTER;
        case 27:
            return MenuUI::ESCAPE;
        default:
            return MenuUI::OTRA;
    }
}

void MenuUI::moverCursor(const vector<string>& opcionesMenu, MenuUI::Tecla tecla) {
    switch (tecla) { 
        case MenuUI::ARRIBA: 
            if (cursor > 0) cursor--; 
            break; 
        case MenuUI::ABAJO: 
            if (cursor < opcionesMenu.size() - 1) cursor++; 
            break; 
        default: 
            break; 
    }
}

int MenuUI::getCursor() const { 
    return cursor; 
} 

void MenuUI::setCursor(int posicion) { 
    cursor = posicion; 
}

void MenuUI::pantallaDeCarga() {
    const std::vector<std::string> frames = {"-", "\\", "|", "/"};
    const std::vector<std::string> colors = {RED, GREEN, YELLOW, CYAN, PURPURA};
    int frameIndex = 0;
    int colorIndex = 0;

    while (loading) {
        system("cls");
        cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
        std::cout << std::setw((consoleWidth+25) / 2) << " Cargando " << colors[colorIndex] << frames[frameIndex] << RESET << std::endl;
        frameIndex = (frameIndex + 1) % frames.size();
        colorIndex = (colorIndex + 1) % colors.size();
        cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void MenuUI::iniciarPantallaDeCarga() {
    loading = true;
    thread t(&MenuUI::pantallaDeCarga, this);
    t.detach();
}

void MenuUI::detenerPantallaDeCarga() {
    loading = false;
    std::cout << std::setw(consoleWidth / 2 - 4) << "Cargando... Completado!           " << std::endl;
}

void MenuUI::transicionDeslizante(const vector<string>& menuActual, const vector<string>& siguienteMenu) const {
    const int pasos = 60;
    const int delay = 50; // Milisegundos

    for (int i = 40; i <= pasos; ++i) {
        system("cls");
        cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
        for (size_t j = 0; j < menuActual.size(); ++j) {
            cout << string(i, ' ') << menuActual[j] << endl;
        }
        cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
        this_thread::sleep_for(std::chrono::milliseconds(delay));
    }

    for (int i = pasos; i >= 50; --i) {
        system("cls");
        cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
        for (size_t j = 0; j < siguienteMenu.size(); ++j) {
            cout << string(i, ' ') << siguienteMenu[j] << endl;
        }
        cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
        this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}

void MenuUI::mostrarMenuConTransicion(const vector<string>& menuActual, const vector<string>& siguienteMenu) {
    transicionDeslizante(menuActual, siguienteMenu);
    mostrarMenu(siguienteMenu);
}


bool MenuUI::confirmacion(const string& mensaje) {
    vector<string> opciones = {"Si", "No"};
    int seleccion = 0;
    while (true) {
        system("cls");
        cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
        cout << std::setw(((consoleWidth+5) / 2)) << "";
        cout << mensaje << endl << endl; 
        for (int i = 0; i < opciones.size(); ++i) {
            cout << std::setw(((consoleWidth+5) / 2)) << ""; // Asegura que todas empiezan en el mismo margen izquierdo
            if (i == seleccion) {
                cout << YELLOW << "->>" << RESET;
            } else {
                cout << "   ";
            }
            cout << opciones[i] << endl;
        }
        cout << endl << PURPURA << SEPARADOR << RESET << endl;
        Tecla tecla = getTecla();
        if (tecla == ARRIBA && seleccion > 0) {
            seleccion--;
        } else if (tecla == ABAJO && seleccion < opciones.size() - 1) {
            seleccion++;
        } else if (tecla == ENTER) {
            return seleccion == 0;
        }
    }
}
