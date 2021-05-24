#include "ArbolBinario.h"
#include <iostream>

ArbolBinario::ArbolBinario()
{
	root = nullptr;
}

ArbolBinario::~ArbolBinario()
{
	destruir_arbol();
}
void ArbolBinario::destruir_arbol(nodo* hoja)
{
    if (hoja != nullptr)
    {
        destruir_arbol(hoja->izquierda);
        destruir_arbol(hoja->derecha);
        delete hoja;
    }
}

/// <summary>
/// Inserta el valor en el arbol (recursivo)
/// </summary>
/// <param name="key"></param>
/// <param name="hoja"></param>
void ArbolBinario::insertar(int key, nodo* hoja)
{
    if (key < hoja->dato)
    {
        if (hoja->izquierda != nullptr)
            insertar(key, hoja->izquierda);
        else
        {
            hoja->izquierda = new nodo;
            hoja->izquierda->dato = key;
            hoja->izquierda->izquierda = nullptr;    //Sets the left child of the child node to null
            hoja->izquierda->derecha = nullptr;   //Sets the right child of the child node to null
        }
    }
    else if (key >= hoja->dato)
    {
        if (hoja->derecha != nullptr)
            insertar(key, hoja->derecha);
        else
        {
            hoja->derecha = new nodo;
            hoja->derecha->dato = key;
            hoja->derecha->izquierda = nullptr;  //Sets the left child of the child node to null
            hoja->derecha->derecha = nullptr; //Sets the right child of the child node to null
        }
    }
}

/// <summary>
/// busca el valor en el arbol (recursivo)
/// </summary>
/// <param name="key"></param>
/// <param name="hoja"></param>
/// <returns></returns>
nodo* ArbolBinario::buscar(int key, nodo* hoja)
{
    if (hoja != nullptr)
    {
        if (key == hoja->dato)
            return hoja;
        if (key < hoja->dato)
            return buscar(key, hoja->izquierda);
        else
            return buscar(key, hoja->derecha);
    }
    else return nullptr;
}
/// <summary>
/// Inserta el valor en el arbol, si esta vacio lo crea
/// </summary>
/// <param name="key"></param>
void ArbolBinario::insertar(int key)
{
    if (root != nullptr)
        insertar(key, root);
    else
    {
        root = new nodo;
        root->dato = key;
        root->izquierda = nullptr;
        root->derecha = nullptr;
    }
}
/// <summary>
/// Busca el valor en el arbol
/// </summary>
/// <param name="key"></param>
/// <returns></returns>
nodo* ArbolBinario::buscar(int key)
{
    return buscar(key, root);
}

/// <summary>
/// Borra el arbol
/// </summary>
void ArbolBinario::destruir_arbol()
{
    destruir_arbol(root);
}

/// <summary>
/// Imprime en la consola el arbol
/// </summary>
/// <param name="hoja"></param>
/// <param name="contador"></param>
void ArbolBinario::mostrar()
{
    int contador = 0;
    if (root == nullptr)
    {
        return;
    }
    else
    {
        mostrar(root->derecha, contador + 1);

        for (int i = 0; i < contador; i++)
            std::cout << "   ";

        std::cout << root->dato << "\n";

        mostrar(root->izquierda, contador + 1);
    }
}

/// <summary>
/// Imprime en la consola el arbol(recursivo)
/// </summary>
/// <param name="hoja"></param>
/// <param name="contador"></param>
void ArbolBinario::mostrar(nodo* hoja, int contador)
{
    if (hoja == nullptr)
    {
        return;
    }
    else
    {
        mostrar(hoja->derecha, contador + 1);

        for (int i = 0; i < contador; i++)
            std::cout << "   ";

        std::cout << hoja->dato << "\n";

        mostrar(hoja->izquierda, contador + 1);
    }
}

/// <summary>
/// Retornna el path de busqueda en un vector, para poder buscar el commonancestor
/// </summary>
/// <param name="root"></param>
/// <param name="path"></param>
/// <param name="k"></param>
/// <returns></returns>
bool ArbolBinario::findPath(nodo* root, std::vector<int>& path, int k)
{
    // base case
    if (root == nullptr) return false;

    // Store this node in path vector. The node will be removed if
    // not in path from root to k
    path.push_back(root->dato);

    // See if the k is same as root's key
    if (root->dato == k)
        return true;

    // Check if k is found in left or right sub-tree
    if ((root->izquierda && findPath(root->izquierda, path, k)) ||
        (root->derecha && findPath(root->derecha, path, k)))
        return true;

    // If not present in subtree rooted with root, remove root from
    // path[] and return false
    path.pop_back();
    return false;
}

/// <summary>
/// Retorna el CommonAncestor si valor1 y valor2 existen en el arbol
/// sino retorna -1
/// </summary>
/// <param name="n1"></param>
/// <param name="n2"></param>
/// <returns></returns>
int ArbolBinario::findLCA(int n1, int n2)
{
    
    // to store paths to n1 and n2 from the root
    std::vector<int> path1, path2;

    // Find paths from root to n1 and root to n1. If either n1 or n2
    // is not present, return -1
    if (!findPath(root, path1, n1) || !findPath(root, path2, n2))
        return -1;

    /* Compare the paths to get the first different value */
    size_t i;
    for (i = 0; i < path1.size() && i < path2.size(); i++)
        if (path1[i] != path2[i])
            break;
    return path1[i - 1];
}



