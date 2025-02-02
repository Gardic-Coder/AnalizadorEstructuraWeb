// Logica/WebAnalyzer.cpp
#include "WebAnalyzer.h"

// Funcion para obtener el contenido HTML de una URL
void WebAnalyzer::Analyze() {
	CURL* curl;
	CURLcode res;
	output.clear(); // Asegurarnos de que output esté limpio antes de la solicitud
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WebAnalyzer::WriteCallbackWrapper);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		// Impresión de depuración antes de la solicitud
		//cout << "Contenido de output antes de curl_easy_perform: " << output << endl;

		res = curl_easy_perform(curl);

		// Impresión de depuración después de la solicitud
		if (res != CURLE_OK) {
			cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
		} else {
			//cout << "Contenido de output después de curl_easy_perform: " << output << endl;
		}
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();

	ExtractLinks(output, dominio, enlaces);
}

size_t WebAnalyzer::WriteCallbackWrapper(void* contents, size_t size, size_t nmemb, void* userp) {
	return static_cast<WebAnalyzer*>(userp)->WriteCallback(contents, size, nmemb);
}

size_t WebAnalyzer::WriteCallback(void* contents, size_t size, size_t nmemb) {
	size_t totalSize = size * nmemb;
	output.append(static_cast<char*>(contents), totalSize);
	return totalSize;
}

// Metodo para extraer enlaces del mismo dominio.
void WebAnalyzer::ExtractLinks(const string& htmlContent, const string& baseDomain, vector<string>& links) {
	enlaces.clear();

	regex urlRegex(R"((href|src)\s*=\s*['"]([^'"]+)['"])");
	smatch urlMatches;
	string::const_iterator searchStart(htmlContent.cbegin());
	while (regex_search(searchStart, htmlContent.cend(), urlMatches, urlRegex)) {
		string url = urlMatches[2].str();
		if (url.find(baseDomain) != std::string::npos) {
			links.push_back(url);
		}
		searchStart = urlMatches.suffix().first;
	}
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

vector<string> WebAnalyzer::getCaminoDeEnlaces() const {
	return caminoDeEnlaces;
}

// Metodo para verificar que todos los enlaces pertenecen al mismo dominio
bool WebAnalyzer::verificarDominios() const {
	if(enlaces.empty()) return false;
	for (const auto& enlace : enlaces) {
		if (enlace.find(dominio) == string::npos) {
			return false;
		}
	}
	return true;
}

// Metodo para encontrar la ruta de enlaces que llevan a la palabra clave
bool WebAnalyzer::encontrarCaminoPalbraClave() {
	queue<string> cola;
	unordered_set<string> visitado;
	unordered_map<string, string> mapaDePadres;

	cola.push(url);
	visitado.insert(url);

	while (!cola.empty()) {
		string actualUrl = cola.front();
		cola.pop();
		output.clear();

		CURL* curl = curl_easy_init();
		if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, actualUrl.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallbackWrapper);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
			CURLcode res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);

			if (res == CURLE_OK) {
				vector<string> links;
				ExtractLinks(output, dominio, links);

				for (const string& link : links) {
					if (link.find(palabraClave) != string::npos) {
						caminoDeEnlaces.push_back(link);
						string padre = actualUrl;
						while (padre != url) {
							caminoDeEnlaces.push_back(padre);
							padre = mapaDePadres[padre];
						}
						caminoDeEnlaces.push_back(url);
						reverse(caminoDeEnlaces.begin(), caminoDeEnlaces.end());
						return true;
					}

					if (visitado.find(link) == visitado.end()) {
						cola.push(link);
						visitado.insert(link);
						mapaDePadres[link] = actualUrl;
					}
				}
			}
		}
	}
	return false;
}
/*
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
}*/