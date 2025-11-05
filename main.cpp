#include <iostream>
#include "hash.cpp"  // ou o nome do arquivo que contém sua classe
using namespace std;

int main() {
    HashTable tabela(10);

    tabela.add(5);
    tabela.add(15);
    tabela.add(25);

    tabela.display();

    cout << (tabela.busca(15) ? "Encontrado" : "Nao encontrado") << endl;
    cout << (tabela.busca(99) ? "Encontrado" : "Nao encontrado") << endl;
    cout << (tabela.size) << endl;


    return 0;
}
