#include <stdio.h>
#include <cpprest/uri.h>
#include <cpprest/http_listener.h>
#include <cpprest/asyncrt_utils.h>
#include "ArbolBinario.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

class CommandHandler
{
public:
	CommandHandler() {}
	CommandHandler(utility::string_t url);
	pplx::task<void> open() { return m_listener.open(); }
	pplx::task<void> close() { return m_listener.close(); }
private:
	void handle_get_or_post(http_request message);
	http_listener m_listener;
};

CommandHandler::CommandHandler(utility::string_t url) : m_listener(url)
{
	m_listener.support(methods::GET, std::bind(&CommandHandler::handle_get_or_post, this, std::placeholders::_1));
	m_listener.support(methods::POST, std::bind(&CommandHandler::handle_get_or_post, this, std::placeholders::_1));
}

void CommandHandler::handle_get_or_post(http_request message)
{
	ucout << "Metodo: " << message.method() << std::endl;
	ucout << "URI: " << http::uri::decode(message.relative_uri().path()) << std::endl;
	ucout << "Query: " << http::uri::decode(message.relative_uri().query()) << std::endl << std::endl;



	message.reply(status_codes::OK, "SERVICIO OK");
};

int main(int argc, char argv[])
{
	try
	{
		//Para crear el servicio REST uso CPPRESTSDK, instalado desde vcpkg(en el Readme se indica como instalar)
		//Escucho en el puerto 8080
		utility::string_t address = U("http://*:8080/");
		uri_builder uri(address);
		auto addr = uri.to_uri().to_string();
		CommandHandler handler(addr);
		handler.open().wait();
		ucout << utility::string_t(U("Escuchando solicitudes en: ")) << addr << std::endl;
		
		std::string line;
		
		//std::cout << "Hello World!\n";
		ArbolBinario* arbol = new ArbolBinario();
		int valor = 0;
		do
		{
			std::cin >> valor;
			if (valor > 0)
			{

				arbol->insertar(valor);

			}

		} while (valor != 0);

		arbol->mostrar();

		int valor1, valor2;
		std::cout << "Ingresar 2 numeros" << std::endl;
		std::cin >> valor1;
		std::cin >> valor2;
		int c = arbol->findLCA(valor1, valor2);

		std::cout << c;
		
		
		ucout << U("Presiones ENTER para salir...") << std::endl;
		std::getline(std::cin, line);




		handler.close().wait();

		delete arbol;
	}
	catch (std::exception& ex)
	{
		ucout << U("Excepcion: ") << ex.what() << std::endl;
		ucout << U("Presiones ENTER para salir...") << std::endl;
		std::string line;
		std::getline(std::cin, line);
	}
	return 0;
}