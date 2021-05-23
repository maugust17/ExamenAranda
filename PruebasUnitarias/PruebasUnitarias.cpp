#include "pch.h"
#include "CppUnitTest.h"
#include "../ServicioRest/ArbolBinario.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PruebasUnitarias
{
	TEST_CLASS(PruebasUnitarias)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{

		}
		TEST_METHOD(Prueba)
		{
			ArbolBinario* arbol = new ArbolBinario();
			arbol->insertar(10);
			arbol->insertar(14);
			arbol->insertar(6);
			arbol->insertar(5);
			arbol->insertar(8);
			arbol->insertar(11);
			arbol->insertar(18);


			Assert::AreEqual(10, arbol->findLCA(11, 8));
			Assert::AreEqual(6, arbol->findLCA(5, 8));
			Assert::AreEqual(6, arbol->findLCA(8, 5));
			Assert::AreEqual(10, arbol->findLCA(11, 10));

			delete arbol;
		}
	};
}
