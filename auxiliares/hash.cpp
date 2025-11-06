#pragma once
#include <iostream>
#include <vector>
using namespace std;

/*
===========================================================
  Classe: HashTable
  Objetivo: Implementar uma tabela hash com endereçamento
            aberto utilizando sondagem dupla (double hashing).

  Descrição geral:
  - Cada posição da tabela armazena uma única chave inteira.
  - Colisões são resolvidas através da sondagem dupla:
      índice = (h1(key) + i * h2(key)) % size
  - h1(key) = key % size
  - h2(key) = 1 + (key % (size - 1))
  - Uma posição vazia é marcada com o valor EMPTY (-1).
===========================================================
*/
class HashTable
{
private:
    vector<int> table;
    vector<bool> ocupado;
    int size;
    const int EMPTY = -1;

    int hash1(int key) { return key % size; }
    int hash2(int key) { return 1 + (key % (size - 1)); }

public:
    // construtor que inicializa a tabela hash com tamanho 's'
    HashTable(int s) : size(s)
    {
        table.resize(size, EMPTY);
        ocupado.resize(size, false);
    }

    // adiciona uma chave à tabela hash
    void add(int key)
    {
        int index = hash1(key);
        int step = hash2(key);
        int i = 0;
        while (ocupado[(index + i * step) % size])
        {
            i++;
            if (i == size)
            {
                cout << "Erro: tabela cheia, nao foi possivel inserir " << key << endl;
                return;
            }
        }
        int pos = (index + i * step) % size;
        table[pos] = key;
        ocupado[pos] = true;
    }

    // busca uma chave na tabela hash
    bool busca(int key)
    {
        int index = hash1(key);
        int step = hash2(key);
        int i = 0;
        while (ocupado[(index + i * step) % size])
        {
            int pos = (index + i * step) % size;
            if (table[pos] == key)
                return true;
            i++;
            if (i == size)
                break;
        }
        return false;
    }

    // exibe o conteúdo da tabela hash
    void display()
    {
        cout << "==============================" << endl;
        cout << "   TABELA HASH (Sondagem Dupla)" << endl;
        cout << "==============================" << endl;
        for (int i = 0; i < size; i++)
        {
            cout << i + 1 << " --> ";
            if (ocupado[i])
                cout << table[i];
            else
                cout << "NULL";
            cout << endl;
        }
    }
};
