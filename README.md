# ExamenAranda
1)	Instalar Git For Windows(https://git-scm.com/download/win)
2)	Instalar Visual Studio 2019 en Ingles. Si se instala en español hay que agregar el paquete de idiomas en ingles (https://agirlamonggeeks.com/2019/03/10/how-to-change-language-in-visual-studio-2019-after-installation/) 
3)	Clonar Repositorio de vcpkg
	Desde la consola ejecutar:
		git clone https://github.com/Microsoft/vcpkg.git
4)	Instalar vcpkg, ejecutar el bat que esta dentro de la carpeta cloanda en el paso anterior
	.\vcpkg\bootstrap-vcpkg.bat
5)	Instalar paquetes necesarios
	vcpkg install --triplet x64-windows zlib openssl boost-system boost-date-time boost-regex boost-interprocess websocketpp brotli
6)	Instalar Paquetes de cpprestsdk
	vcpkg install cpprestsdk cpprestsdk:x64-windows
7)	Integrar con el VS2019
	vcpkg integrate install
8) 	Instalar Advanced Rest Client para pruebas (https://install.advancedrestclient.com/install)
9)	****** IMPORTANTE ****** El web service corre el puerto 8080




Pruebas del Web Service usando Advanced Rest Client:

- Creo 2 Arboles 

PUT http://127.0.0.1:8080/ArbolBinario
Retorna id=1

PUT http://127.0.0.1:8080/ArbolBinarioError
Retorna Error Bad Request

PUT http://127.0.0.1:8080/ArbolBinario
Retorna id=2

- Cargo de datos el arbol 1

POST http://127.0.0.1:8080/ArbolBinario?id=1&valor=10
Retorna OK

POST http://127.0.0.1:8080/ArbolBinario?id=1&valor=14
Retorna OK

POST http://127.0.0.1:8080/ArbolBinario?id=8&valor=11
Retorna Error Bad Request - Arbol Inexistente

POST http://127.0.0.1:8080/ArbolBinario?id=1&valor=6
Retorna OK

POST http://127.0.0.1:8080/ArbolBinario?id=1&valor=5
Retorna OK

POST http://127.0.0.1:8080/ArbolBinario?id=1&valor=8
Retorna OK

POST http://127.0.0.1:8080/ArbolBinario?id=1&valor=11
Retorna OK

POST http://127.0.0.1:8080/ArbolBinario?id=1&valor=18
Retorna OK

- Cargo de datos el arbol 2

POST http://127.0.0.1:8080/ArbolBinario?id=2&valor=17
Retorna OK

POST http://127.0.0.1:8080/ArbolBinario?id=2&valor=19
Retorna OK

POST http://127.0.0.1:8080/ArbolBinario?id=8&valor=11
Retorna Error Bad Request - Arbol Inexistente

POST http://127.0.0.1:8080/ArbolBinario?id=2&valor=2
Retorna OK

POST http://127.0.0.1:8080/ArbolBinario?id=2&valor=9
Retorna OK

POST http://127.0.0.1:8080/ArbolBinario?id=2&valor=3
Retorna OK

POST http://127.0.0.1:8080/ArbolBinario?id=2&valor=1
Retorna OK

POST http://127.0.0.1:8080/ArbolBinario?id=2&valor=7
Retorna OK

- Obtengo el CommonAncestor del arbol 1

GET http://127.0.0.1:8080/ArbolBinario/CommonAncestor?id=1&valor1=11&valor2=8
Retorna 10

GET http://127.0.0.1:8080/ArbolBinario/CommonAncestor?id=1&valor1=5&valor2=8
Retorna 6

GET http://127.0.0.1:8080/ArbolBinario/CommonAncestor?id=1&valor1=8&valor2=5
Retorna 6

GET http://127.0.0.1:8080/ArbolBinario/CommonAncestor?id=1&valor1=11&valor2=10
Retorna 10




