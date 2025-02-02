// Logica/WebAnalyzer.cpp
#include "WebAnalyzer.h"
#include <iostream>

using namespace std;

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




/*

// Callback para escribir datos recibidos por curl
size_t WebAnalyzer::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
	s->append((char*)contents, size * nmemb);
	return size * nmemb;
}

// Descargar el contenido HTML de una URL
std::string WebAnalyzer::downloadHtml(const std::string& url) {
	CURL* curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();
	if(curl) {
		// Configurar la URL a descargar
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		// Configurar la funcion de callback para escribir los datos recibidos
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		// Configurar los datos donde se almacenara el contenido recibido
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		// Ejecutar la solicitud HTTP
		res = curl_easy_perform(curl);
		// Limpiar la sesion de curl
		curl_easy_cleanup(curl);
	}
	return readBuffer;
}

// Extraer enlaces de un documento HTML
std::vector<std::string> WebAnalyzer::extractLinks(const std::string& html, const std::string& baseUrl) {
	std::vector<std::string> links;
	// Analizar el contenido HTML en memoria
	htmlDocPtr doc = htmlReadMemory(html.c_str(), html.size(), baseUrl.c_str(), NULL, HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);

	if (doc) {
		xmlNode* root = xmlDocGetRootElement(doc);
		xmlNode* currentNode = root;

		// Recorrer los nodos del documento HTML
		while (currentNode) {
			// Si el nodo es un elemento <a>
			if (currentNode->type == XML_ELEMENT_NODE && xmlStrcasecmp(currentNode->name, (const xmlChar *)"a") == 0) {
				// Obtener el atributo href
				xmlChar* href = xmlGetProp(currentNode, (const xmlChar *)"href");
				if (href) {
					// Agregar el enlace a la lista de enlaces
					links.push_back((char*)href);
					xmlFree(href);
				}
			}
			currentNode = currentNode->next;
		}

		// Liberar el documento HTML
		xmlFreeDoc(doc);
	}
	return links;
}

// Verificar si un enlace es interno
bool WebAnalyzer::isInternalLink(const std::string& link, const std::string& baseUrl) {
	// Implementar la logica para verificar si el enlace pertenece al mismo dominio
	// Por simplicidad, retornamos true para todos los enlaces (logica a implementar)
	return true;
}

// Encontrar enlaces que contienen una palabra clave en la URL
std::vector<std::string> WebAnalyzer::findKeywordLinks(const std::vector<std::string>& links, const std::string& keyword) {
	std::vector<std::string> keywordLinks;
	// Copiar enlaces que contienen la palabra clave
	std::copy_if(links.begin(), links.end(), std::back_inserter(keywordLinks),
	[&keyword](const std::string& link) {
		return link.find(keyword) != std::string::npos;
	});
	return keywordLinks;
}
*/