// Logica/WebAnalyzer.cpp
#include "WebAnalyzer.h"

size_t WebAnalyzer::WriteCallback(void* contents, size_t size, size_t nmemb) {
	size_t totalSize = size * nmemb;
	output.append(static_cast<char*>(contents), totalSize);
	return totalSize;
}

size_t WebAnalyzer::WriteCallbackWrapper(void* contents, size_t size, size_t nmemb, void* userp) {
	return static_cast<WebAnalyzer*>(userp)->WriteCallback(contents, size, nmemb);
}

void WebAnalyzer::ExtractLinks(const string& htmlContent, const string& baseDomain, vector<string>& links) {
	regex urlRegex(R"((href|src)\s*=\s*['"]([^'"]+)['"])");
	smatch urlMatches;
	string::const_iterator searchStart(htmlContent.cbegin());
	while (regex_search(searchStart, htmlContent.cend(), urlMatches, urlRegex)) {
		string url = urlMatches[2].str();
		if (url.find(baseDomain) != string::npos) {
			links.push_back(url);
		}
		searchStart = urlMatches.suffix().first;
	}
}

void WebAnalyzer::analizador() {
	CURL* curl;
	CURLcode res;
	output.clear(); // Asegurarnos de que output esté limpio antes de la solicitud
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallbackWrapper);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // Desactivar verificación del certificado
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // Desactivar verificación del host
		// Agrega un punto de depuración antes de la llamada
		cout << "Contenido de output antes de curl_easy_perform: " << output << endl;
		res = curl_easy_perform(curl);
		// Agrega un punto de depuración después de la llamada
		if (res != CURLE_OK) {
			cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
		} else {
			cout << "Contenido de output después de curl_easy_perform: " << output << endl;
		}
		if (res != CURLE_OK)
			cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();

	ExtractLinks(output, dominio, enlaces); // Replace with the base domain you want to match
}

/*string WebAnalyzer::ExtractDomain(const string& _url) {
	regex domainRegex(R"(^(?:https?://)?(?:www\.)?([^/]+))");
	smatch domainMatch;
	if (regex_search(_url, domainMatch, domainRegex)) {
		return domainMatch[1].str();
	}
	return "";
}*/

// Funcion para obtener el contenido HTML de una URL


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
	//dominio = ExtractDomain(url);
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