#pragma once
#include <vector>
#include <iostream>
#include <string>
struct nodo
{
	int dato;
	nodo* izquierda;
	nodo* derecha;
};

class ArbolBinario
{
public:
    ArbolBinario();
    ~ArbolBinario();

    void insertar(int key);
    nodo* buscar(int key);
    void destruir_arbol();
    void mostrar();
    int findLCA(int n1, int n2);
 
    std::string Serialize();
    static ArbolBinario* Deserialize(std::string serialize);
private:
    void destruir_arbol(nodo* leaf);
    void insertar(int key, nodo* leaf);
    nodo* buscar(int key, nodo* leaf);
    void mostrar(nodo* leaf, int contador);
    bool findPath(nodo* root, std::vector<int>& path, int k);
    nodo* root;
    std::vector<int> orden_ingreso;
};

