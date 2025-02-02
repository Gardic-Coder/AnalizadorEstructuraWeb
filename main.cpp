#include <iostream>
#include <string>
#include <regex>
#include "Menu.h"

#include <thread>

bool esUrlValida(const string& url);

int main() {
	vector<string> menuOptions = {"Ingresar Enlace", "Ingresar Palabra Clave", "Analizar Enlace", "Buscar Palabra Clave", "Mostrar Enlaces", "Guardar", "Salir"};
	vector<string> subMenuOptions = {"Configurar", "Volver"};
	MenuUI menu(80); // Ancho de la consola definido como 80 caracteres
	bool salir = false;

	while (!salir) {
		menu.mostrarMenu(menuOptions);
		MenuUI::Tecla tecla = menu.getTecla();

		if (tecla == MenuUI::ENTER) {
			int opcionSeleccionada = menu.getCursor();
			switch(opcionSeleccionada) {
				case 0: {
					string url = menu.solicitarDato<string>("Ingrese una Direccion URL: ");
					if (esUrlValida(url)) {
						cout << "				La URL es válida." << endl;
					} else {
						cout << "				La URL NO es válida." << endl;
					}
					break;
				}
				case 1: {
					string palabraClave = menu.solicitarDato<string>("Ingrese una Palabra Clave: ");
					break;
				}
				case 2: {

					break;
				}
				case 3: {

					break;
				}
				case 4: {

					break;
				}
				case 5: {

					break;
				}
				case 6: {
					if (menu.confirmacion("¿Estas seguro que deseas salir?")) {
						cout << "Saliendo del menú..." << endl;
						salir = !salir;
					}
					break;
				}
			}

			/*if (opcionSeleccionada == 0) { // Cuando seleccionamos "Inicio" para probar
				menu.transicionDeslizante(menuOptions, subMenuOptions);
				while (true) { // Permitir navegar en el submenú sin mostrar la transición de nuevo
					menu.mostrarMenu(subMenuOptions);
					MenuUI::Tecla subTecla = menu.getTecla();
					if (subTecla == MenuUI::ENTER || subTecla == MenuUI::ESCAPE) {
						break; // Salir del submenú al presionar Enter o Escape
					}
					menu.moverCursor(subMenuOptions, subTecla);
				}
			} else if (opcionSeleccionada == 2) { // Solicitar Dato


				cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
				getch();
			} else if (opcionSeleccionada == 3) {
				if (menu.confirmacion("¿Estas seguro que deseas salir?")) {
					cout << "Saliendo del menú..." << endl;
					break;
				} else {
					menu.mostrarMenu(menuOptions);
				}
			}*/
		} else if (tecla == MenuUI::ESCAPE) {
			if (menu.confirmacion("¿Estás seguro que deseas salir?")) {
				cout << "Saliendo del menú..." << endl;
				salir = !salir;
				break;
			}
		} else {
			menu.moverCursor(menuOptions, tecla);
		}
	}

	return 0;
}

bool esUrlValida(const string& url) {
	const regex patronUrl(
	    R"(^(https?://)?([a-zA-Z0-9-]+\.)+[a-zA-Z]{2,6}(/[\w.-]*)*/?$)"
	);
	return regex_match(url, patronUrl);
}