#include "hash.h"

// Construtor
HashTable::HashTable(int s) : size(s), table(s) {}

// Função hash
int HashTable::funcaoHash(int key)
{
    return key % size;
}

// Inserção de uma chave
void HashTable::add(int key)
{
    int index = funcaoHash(key);
    table[index].push_back(key);
}

// Busca de uma chave
bool HashTable::busca(int key)
{
    int index = funcaoHash(key);
    for (int val : table[index])
    {
        if (val == key)
            return true;
    }
    return false;
}

// Exibição da tabela hash
void HashTable::display()
{
    for (int i = 0; i < size; i++)
    {
        cout << i << ": ";
        for (int val : table[i])
            cout << val << " -> ";
        cout << "NULL" << endl;
    }
}
