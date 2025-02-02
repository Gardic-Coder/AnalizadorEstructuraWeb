// Logica/WebAnalyzer.h
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <curl/curl.h>
//#include <libxml/HTMLparser.h>

using namespace std;

class WebAnalyzer {
	private:
		string url;
		string palabraClave;
		string dominio;
		vector<string> enlaces;
		vector<string> caminoDeEnlaces;

		// Funcion para obtener el contenido HTML de una URL
		string getContenidoHTML(const string& url);

		// Funcion para extraer el dominio de la URL
		void extraerDominio();

		/*
		// Callback para escribir datos recibidos por curl
		static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s);*/

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
		/*
		// Metodo para filtrar enlaces que contienen la palabra clave
		vector<string> filter_links_by_keyword() const {
			std::vector<std::string> result;
			for (const auto& link : links) {
				if (link.find(keyword) != std::string::npos) {
					result.push_back(link);
				}
			}
			return result;
		}

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

/*
// Descargar el contenido HTML de una URL
string downloadHtml(const string& url);

// Extraer enlaces de un documento HTML
vector<string> extractLinks(const string& html, const string& baseUrl);

// Verificar si un enlace es interno
bool isInternalLink(const string& link, const string& baseUrl);

// Encontrar enlaces que contienen una palabra clave en la URL
vector<string> findKeywordLinks(const vector<string>& links, const string& keyword);
};*/