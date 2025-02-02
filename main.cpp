#include <iostream>
#include <string>
#include <regex>
#include <thread>
#include "Presentacion/Menu.h"
#include "Logica/WebAnalyzer.h"
#include "Datos/Datos.h"

bool esUrlValida(const string& url);

int main() {
	vector<string> menuOptions = {"Ingresar Enlace", "Ingresar Palabra Clave", "Analizar Enlace", "Buscar Palabra Clave", "Mostrar Enlaces", "Guardar", "Salir"};
	vector<string> listaDeEnlaces;
	vector<string> caminoDeEnlaces;
	MenuUI menu(80); // Ancho de la consola definido como 80 caracteres
	WebAnalyzer sistema;
	FileHandler controlArchivos("Lista de Enlaces");
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

						sistema.setURL(url);
						//cout << url << "	" << sistema.getDominio();
					} else {
						cout << "				La URL NO es válida." << endl;
					}
					break;
				}
				case 1: {
					string palabraClave = menu.solicitarDato<string>("Ingrese una Palabra Clave: ");
					sistema.setPalabraClave(palabraClave);
					cout << "				Palabra Clave Guardada." << endl;
					break;
				}
				case 2: {
					sistema.Analyze();
					if(sistema.verificarDominios()) {
						listaDeEnlaces = sistema.getEnlaces();
						cout << "				Lista de enlaces adquirida." << endl;
					} else {
						cout << "				Error. No se pudo adquirir la lista de enlaces." << endl;
					}
					break;
				}
				case 3: {
					menu.iniciarPantallaDeCarga();
					if (sistema.encontrarCaminoPalbraClave()) {
						menu.detenerPantallaDeCarga();
						system("cls");
						cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
						cout << "				Ruta encontrada:" << endl;
						cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
						caminoDeEnlaces = sistema.getCaminoDeEnlaces();
						int n = 0;
						for (const string& enlace : caminoDeEnlaces) {
							for (int i = 0; i < n; i++) {
								cout << "|	";
							}
							cout << RESET << "├── " << CYAN << enlace << RESET << endl;
							n++;
						}
					} else {
						menu.detenerPantallaDeCarga();
						system("cls");
						cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
						cout << "				No se encontró una ruta que lleve a la palabra clave." << endl;
					}

					break;
				}
				case 4: {
					system("cls");
					cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
					if (!listaDeEnlaces.empty()) {
						cout << "				Lista de Enlaces:" << endl;
						cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
						for (const string& enlace : listaDeEnlaces) {
							cout << RESET << "			├── " << CYAN << enlace << RESET << endl;
						}
					} else {
						cout << "				No hay enlaces guardados." << endl;
					}
					break;
				}
				case 5: {
					if (menu.confirmacion("¿Desea sobrescribir el archivo de texto?")) {
						controlArchivos.sobrescribirArchivo("URL Base: " + sistema.getURL());
						controlArchivos.agregarAlArchivo(listaDeEnlaces);
						cout << "				Informacion sobreescrita." << endl;
					} else {
						controlArchivos.agregarAlArchivo("URL Base: " + sistema.getURL());
						controlArchivos.agregarAlArchivo(listaDeEnlaces);
						cout << "				Informacion guardada." << endl;
					}
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
			cout << endl << PURPURA << SEPARADOR << RESET << endl << endl;
			getch();
			
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