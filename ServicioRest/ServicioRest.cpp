#include <stdio.h>
#include <iostream>
#include <cpprest/uri.h>
#include <cpprest/http_listener.h>
#include <cpprest/asyncrt_utils.h>
#include "ArbolBinario.h"
#include "sqlite3.h"



using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

std::vector<ArbolBinario*> listaArboles;
int contador;
sqlite3* DB = nullptr;
void crear_db()
{

	std::string sql = "CREATE TABLE arbol("
		"ID INT PRIMARY KEY     NOT NULL, "
		"ORDEN          TEXT    NOT NULL);";
	int exit = 0;
	exit = sqlite3_open("aranda.db", &DB);
	char* messaggeError;
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

	if (exit != SQLITE_OK)
	{
		//Puede ser porque existe, revisar el error
		// 
		//std::cerr << "Error Create Table" << std::endl;
		//sqlite3_free(messaggeError);
	}
	else
		std::cout << "Tabla creada exitosamente" << std::endl;
	//
}
void cerrar_db()
{
	sqlite3_close(DB);
}
void insertar_o_actualizar(int id, std::string texto)
{

	char* messaggeError;
	
	/*std::string select("SELECT 1 FROM arbol");
	int exit = sqlite3_exec(DB, select.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK) {
		std::cerr << "Error Insert" << std::endl;
		sqlite3_free(messaggeError);
	}
	else
		std::cout << "Records created Successfully!" << std::endl;
		*/
	std::string sql("INSERT INTO arbol VALUES(" + std::to_string(id) + " ,'" + texto + "');");
	int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK) {
		std::cerr << "Error Insert" << std::endl;
		sqlite3_free(messaggeError);

		std::string update("UPDATE arbol SET orden = '" + texto + "' WHERE ID = " + std::to_string(id) + ";");
		int exit = sqlite3_exec(DB, update.c_str(), NULL, 0, &messaggeError);
		if (exit != SQLITE_OK) {
			std::cerr << "Error Update" << std::endl;
			sqlite3_free(messaggeError);


		}
		else
		{
			std::cout << "Records updated Successfully!" << std::endl;

		}

	}
	else
		std::cout << "Records created Successfully!" << std::endl;

}

static int callback(void* data, int argc, char** argv, char** azColName)
{
	
	
	
	if (argc == 2)
	{

		ArbolBinario* arbol = ArbolBinario::Deserialize(argv[1]);

		listaArboles.push_back(arbol);
		contador++;

	}

	
	return 0;
}
void recuperar_arboles()
{ 
	std::string data("CALLBACK FUNCTION");
	std::string sql("SELECT * FROM arbol;");
	
	int rc = sqlite3_exec(DB, sql.c_str(), callback, (void*)data.c_str(), NULL);

	if (rc != SQLITE_OK)
		ucerr << "Error SELECT" << std::endl;
	else {
		ucout << "Operation OK!" << std::endl;
	}
}



class CommandHandler
{
public:
	CommandHandler() { contador = 0; }
	CommandHandler(utility::string_t url);
	~CommandHandler();
	pplx::task<void> open() { return m_listener.open(); }
	pplx::task<void> close() { return m_listener.close(); }
private:
	void handle_get(http_request message);
	void handle_post(http_request message);
	void handle_put(http_request message);
	http_listener m_listener;
	
};
CommandHandler::~CommandHandler()
{
	listaArboles.clear();
}

CommandHandler::CommandHandler(utility::string_t url) : m_listener(url)
{
	//contador = 0;

	recuperar_arboles();
	m_listener.support(methods::GET, std::bind(&CommandHandler::handle_get, this, std::placeholders::_1));
	m_listener.support(methods::POST, std::bind(&CommandHandler::handle_post, this, std::placeholders::_1));
	m_listener.support(methods::PUT, std::bind(&CommandHandler::handle_put, this, std::placeholders::_1));

}
/// <summary>
/// Handle para procesar los pedidos GET
/// </summary>
/// <param name="message"></param>
void CommandHandler::handle_get(http_request message)
{
	string_t URI = http::uri::decode(message.relative_uri().path());
	ucout << "Metodo: " << message.method() << std::endl;
	ucout << "URI: " << http::uri::decode(message.relative_uri().path()) << std::endl;
	ucout << "Query: " << http::uri::decode(message.relative_uri().query()) << std::endl << std::endl;
	
	//Solo acepto pedidos a la ruta /ArbolBinario
	auto path = message.relative_uri().path();
	if (0 == path.find(U("/ArbolBinario/CommonAncestor")))
	{
		auto http_get_vars = uri::split_query(message.request_uri().query());
		auto found_id = http_get_vars.find(U("id"));
		auto found_valor1 = http_get_vars.find(U("valor1"));
		auto found_valor2 = http_get_vars.find(U("valor2"));


		//Busco si existe el parametro ID
		if (found_id == end(http_get_vars))
		{

			message.reply(status_codes::BadRequest, "Falta el ID del arbol");
			return;

		}
		else if (found_valor1 == end(http_get_vars))
		{

			message.reply(status_codes::BadRequest, "Falta el valor1");
			return;

		}
		else if (found_valor2 == end(http_get_vars))
		{

			message.reply(status_codes::BadRequest, "Falta el valor");
			return;

		}
		else
		{
			//Obtengo el ID
			int id = std::stoi(found_id->second);
			int valor1 = std::stoi(found_valor1->second);
			int valor2 = std::stoi(found_valor2->second);
			//Reviso si el ID es mayor a 0 y menor o igual al tama?o del vecto
			if (listaArboles.size() >= id && id > 0)
			{
				ArbolBinario* arbol = listaArboles[id-1];
				//Para debug
				//arbol->mostrar();

				message.reply(status_codes::OK, arbol->findLCA(valor1, valor2));

				
			}
			else
			{
				message.reply(status_codes::BadRequest, "Arbol inexistente");
				return;
			}
		}



	}
	else
	{
		message.reply(status_codes::NotFound);
	}

		
};
/// <summary>
/// Handle para procesar los pedidos POST
/// </summary>
/// <param name="message"></param>
void CommandHandler::handle_post(http_request message)
{
	string_t URI = http::uri::decode(message.relative_uri().path());
	ucout << "Metodo: " << message.method() << std::endl;
	ucout << "URI: " << http::uri::decode(message.relative_uri().path()) << std::endl;
	ucout << "Query: " << http::uri::decode(message.relative_uri().query()) << std::endl << std::endl;

	//Solo acepto pedidos a la ruta /ArbolBinario
	auto path = message.relative_uri().path();
	if (0 == path.find(U("/ArbolBinario")))
	{
		auto http_get_vars = uri::split_query(message.request_uri().query());
		auto found_valor = http_get_vars.find(U("valor"));
		auto found_id = http_get_vars.find(U("id"));


		if (found_valor == end(http_get_vars)) 
		{

			message.reply(status_codes::BadRequest, "Falta el Valor");
			return;

		}
		else if (found_id == end(http_get_vars))
		{

			message.reply(status_codes::BadRequest, "Falta el ID del arbol");
			return;

		}
		else
		{
			int valor = std::stoi(found_valor->second);
			int id = std::stoi(found_id->second);

			if (listaArboles.size() >= id && id>0)
			{
				ArbolBinario* arbol = listaArboles[id-1];
				arbol->insertar(valor);

				message.reply(status_codes::OK);

				insertar_o_actualizar(id, arbol->Serialize());
			}
			else
			{
				message.reply(status_codes::BadRequest, "Arbol inexistente");
				return;
			}
		}
	}
	else
	{
		message.reply(status_codes::NotFound);
	}
};
/// <summary>
/// Handle para procesar los pedidos PUT
/// </summary>
/// <param name="message"></param>
void CommandHandler::handle_put(http_request message)
{
	string_t URI = http::uri::decode(message.relative_uri().path());
	ucout << "Metodo: " << message.method() << std::endl;
	ucout << "URI: " << http::uri::decode(message.relative_uri().path()) << std::endl;
	ucout << "Query: " << http::uri::decode(message.relative_uri().query()) << std::endl << std::endl;

	//Solo acepto pedidos a la ruta /ArbolBinario
	auto path = message.relative_uri().path();
	if (0 == path.compare(U("/ArbolBinario")))
	{
		ArbolBinario *arbol=new ArbolBinario();
		if (arbol != nullptr)
		{
			contador++;
			listaArboles.push_back(arbol);
		}
		
		//Genero un nuevo arbol y env?o ID de vuelta
		message.reply(status_codes::OK, contador);
		//Lo grabo en la DB
		insertar_o_actualizar(contador, "");
	}
	else
	{
		message.reply(status_codes::NotFound);
	}
};



int main(int argc, char argv[])
{
	try
	{
		crear_db();

		//recuperar_arboles();

		ucout << "Recupero Arboles" << std::endl;
		//Para crear el servicio REST uso CPPRESTSDK, instalado desde vcpkg(en el Readme se indica como instalar)
		//Escucho en el puerto 8080
		utility::string_t address = U("http://*:8080/");
		uri_builder uri(address);
		auto addr = uri.to_uri().to_string();
		CommandHandler handler(addr);
		handler.open().wait();
		ucout << utility::string_t(U("Escuchando solicitudes en: ")) << addr << std::endl;


		std::string line;
		ucout << U("Presiones ENTER para salir...") << std::endl;
		std::getline(std::cin, line);
		handler.close().wait();

		
	}
	catch (std::exception& ex)
	{
		ucout << U("Excepcion: ") << ex.what() << std::endl;
		ucout << U("Presiones ENTER para salir...") << std::endl;
		std::string line;
		std::getline(std::cin, line);
	}

	cerrar_db();
	return 0;
}