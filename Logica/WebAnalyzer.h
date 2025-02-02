// Logica/WebAnalyzer.h
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <curl/curl.h>
#include <queue>
#include <unordered_map>

using namespace std;

class WebAnalyzer {
	private:
		string url;
		string palabraClave;
		string dominio;
		string output;  // Moved output to be a class member
		vector<string> enlaces;
		
		//vector<string> caminoDeEnlaces;

		// Funcion para obtener el contenido HTML de una URL
		string getContenidoHTML(const string& url);

		// Funcion para extraer el dominio de la URL
		void extraerDominio();

		// Funcion para extraer enlaces del mismo dominio
		vector<string> extraerEnlacesDeURL(const string& _url);
		
		size_t WriteCallback(void* contents, size_t size, size_t nmemb);
		
		static size_t WriteCallbackWrapper(void* contents, size_t size, size_t nmemb, void* userp);


		void ExtractLinks(const string& htmlContent, const string& baseDomain, vector<string>& links);

	public:
		// Constructor.
		WebAnalyzer();
		// Destructor.
		~WebAnalyzer();

		// Setters.
		void setURL(const string& _url);
		void setPalabraClave(string& _palabraClave);

		// Getters.
		string getURL() const;
		string getPalabraClave() const;
		vector<string> getEnlaces() const;
		string getDominio() const;

		// Metodo para extraer enlaces del mismo dominio
		void extraerEnlaces();

		// Metodo para verificar que todos los enlaces pertenecen al mismo dominio
		bool verificarDominios() const;

		// Metodo para encontrar la ruta de enlaces que llevan a la palabra clave
		vector<string> buscarCaminoPalbraClave();

		void analizador();
		//string ExtractDomain(const string& url);

		/*
		// Metodo para guardar los enlaces en un archivo (delegado a la capa de datos)
		void save_links_to_file(const std::string& filename) const {
			std::ofstream file(filename);
			if (!file.is_open()) {
				std::cerr << "Error: No se pudo abrir el archivo." << std::endl;
				return;
			}

			for (const auto& link : links) {
				file << link << std::endl;
			}
			file.close();
		}
		*/
};