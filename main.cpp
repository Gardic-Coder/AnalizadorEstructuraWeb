#include <iostream>
#include <string>
#include <regex>
#include "Presentacion/Menu.h"
#include "Logica/WebAnalyzer.h"

#include <thread>

bool esUrlValida(const string& url);

int main() {
	vector<string> menuOptions = {"Ingresar Enlace", "Ingresar Palabra Clave", "Analizar Enlace", "Buscar Palabra Clave", "Mostrar Enlaces", "Guardar", "Salir"};
	vector<string> listaDeEnlaces;
	vector<string> caminoDeEnlaces;
	MenuUI menu(80); // Ancho de la consola definido como 80 caracteres
	WebAnalyzer sistema;
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
						cout << url;
						sistema.setURL(url);
					} else {
						cout << "				La URL NO es válida." << endl;
					}
					cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
					break;
				}
				case 1: {
					string palabraClave = menu.solicitarDato<string>("Ingrese una Palabra Clave: ");
					sistema.setPalabraClave(palabraClave);
					cout << "				Palabra Clave Guardada." << endl;
					cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
					break;
				}
				case 2: {
					sistema.extraerEnlaces();
					if(sistema.verificarDominios()) {
						listaDeEnlaces = sistema.getEnlaces();
					} else {
						cout << "				Error." << endl;
					}
					break;
				}
				case 3: {
					caminoDeEnlaces = sistema.buscarCaminoPalbraClave();
					// Mostrar la ruta encontrada
					cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
					if (!caminoDeEnlaces.empty()) {
						cout << "				Ruta encontrada:" << endl;
						int n = 0;
						for (const string& enlace : caminoDeEnlaces) {
							for (int i = 0; i < n; i++) {
								cout << "|	";
							}
							cout << RESET << "├── " << CYAN << enlace << RESET << endl;
							n++;
						}
					} else {
						cout << "				No se encontró una ruta que lleve a la palabra clave." << endl;
					}
					cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
					break;
				}
				case 4: {
					cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
					if (!listaDeEnlaces.empty()) {
						cout << "				Lista de Enlaces:" << endl;
						for (const string& enlace : caminoDeEnlaces) {
							cout << RESET << "				├── " << CYAN << enlace << RESET << endl;
						}
					} else {
						cout << "				No hay enlaces guardados." << endl;
					}
					cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
					break;
				}
				case 5: {
						cout << "				En desarrollo." << endl;
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
			getch();

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