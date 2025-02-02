// Logica/WebAnalyzer.cpp
#include "WebAnalyzer.h"

// Funcion para obtener el contenido HTML de una URL
string WebAnalyzer::getContenidoHTML(const string& url) {
	CURL* curl;
	CURLcode res;
	string contenidoHTML;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](void* ptr, size_t size, size_t nmemb, std::string* data) {
			data->append((char*)ptr, size * nmemb);
			return size * nmemb;
		});
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &contenidoHTML);
		
		// Especificar la ruta al archivo de certificados CA
        curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");
        
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		if (res != CURLE_OK) {
			cerr << "cURL error: " << curl_easy_strerror(res) << endl;
			return "";
		}
	}

	return contenidoHTML;
}

// Funcion para extraer el dominio de la URL
void WebAnalyzer::extraerDominio() {
	regex domain_regex(R"(https?://([^/]+))");
	smatch match;
	if (regex_search(url, match, domain_regex)) {
		dominio = match.str(1);
	} else {
		dominio = "";
	}
}

// Constructor de la clase WebAnalyzer.
WebAnalyzer::WebAnalyzer() {}

// Destructor de la clase WebAnalyzer.
WebAnalyzer::~WebAnalyzer() {
}

// Setters.
void WebAnalyzer::setURL(const string& _url) {
	url = _url;
	extraerDominio();
}

void WebAnalyzer::setPalabraClave(string& _palabraClave) {
	palabraClave = _palabraClave;
}

// Getters.
string WebAnalyzer::getURL() const {
	return url;
}

string WebAnalyzer::getPalabraClave() const {
	return palabraClave;
}

vector<string> WebAnalyzer::getEnlaces() const {
	return enlaces;
}

string WebAnalyzer::getDominio() const {
	return dominio;
}

// Metodo para extraer enlaces del mismo dominio.
void WebAnalyzer::extraerEnlaces() {
	enlaces.clear();
	string contenidoHTML = getContenidoHTML(url);
	if (contenidoHTML.empty()) {
		cerr << "Error: No se pudo obtener el contenido HTML." << endl;
		return;
	}

	regex link_regex(R"(<a href="([^"]+))");
	auto links_begin = sregex_iterator(contenidoHTML.begin(), contenidoHTML.end(), link_regex);
	auto links_end = sregex_iterator();

	for (sregex_iterator i = links_begin; i != links_end; ++i) {
		smatch match = *i;
		string link = match.str(1);
		if (link.find(dominio) != string::npos) {
			enlaces.push_back(link);
		}
	}
}

// Metodo para verificar que todos los enlaces pertenecen al mismo dominio
bool WebAnalyzer::verificarDominios() const {
	for (const auto& enlace : enlaces) {
		if (enlace.find(dominio) == string::npos) {
			return false;
		}
	}
	return true;
}

// Funcion para extraer enlaces del mismo dominio
vector<string> WebAnalyzer::extraerEnlacesDeURL(const string& _url) {
	vector<string> links;
	string contenidoHTML = getContenidoHTML(_url);
	if (contenidoHTML.empty()) {
		return links;
	}

	regex link_regex(R"(<a href="([^"]+))");
	auto links_begin = sregex_iterator(contenidoHTML.begin(), contenidoHTML.end(), link_regex);
	auto links_end = sregex_iterator();

	for (sregex_iterator i = links_begin; i != links_end; ++i) {
		smatch match = *i;
		string link = match.str(1);
		if (link.find(dominio) != string::npos) {
			links.push_back(link);
		}
	}

	return links;
}

// Metodo para encontrar la ruta de enlaces que llevan a la palabra clave
vector<string> WebAnalyzer::buscarCaminoPalbraClave() {
	queue<string> cola;
	unordered_map<string, string> padre; // Almacena el enlace padre de cada enlace

	cola.push(url);
	padre[url] = ""; // La URL inicial no tiene padre

	while (!cola.empty()) {
		string actualURL = cola.front();
		cola.pop();

		// Extraer enlaces de la URL actual
		vector<string> enlacesActuales = extraerEnlacesDeURL(actualURL);

		for (const string& enlace : enlacesActuales) {
			// Si el enlace contiene la palabra clave, reconstruir la ruta
			if (enlace.find(palabraClave) != string::npos) {
				vector<string> camino;
				string nodo = enlace;

				// Reconstruir la ruta desde el enlace final hasta la URL inicial
				while (!nodo.empty()) {
					camino.push_back(nodo);
					nodo = padre[nodo];
				}

				// Invertir la ruta para que quede en el orden correcto
				reverse(camino.begin(), camino.end());
				return camino;
			}

			// Si el enlace no ha sido visitado, agregarlo a la cola
			if (padre.find(enlace) == padre.end()) {
				padre[enlace] = actualURL;
				cola.push(enlace);
			}
		}
	}

	// Si no se encuentra la palabra clave, devolver un vector vacio
	return {};
}