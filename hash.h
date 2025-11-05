#pragma once
#include <iostream>
#include <vector>
using namespace std;

/*
===========================================================
  Classe: HashTable
  Objetivo: Implementar uma tabela hash simples utilizando
            encadeamento (listas de colisão) com vetores.
===========================================================
*/
class HashTable {
public:
    // Vetor de listas (vetor de vetores), onde cada índice
    // representa um "bucket" da tabela hash.
    vector<vector<int>> table;

    // Quantidade total de posições (buckets) na tabela.
    int size;

    /*
      Função hash:
      Calcula o índice correspondente a uma chave.
      Neste caso, a função é simplesmente key % size.
    */
    int funcaoHash(int key);

public:
    /*
      Construtor:
      Recebe o tamanho desejado e inicializa a tabela.
      Cada posição da tabela contém inicialmente uma lista vazia.
    */
    HashTable(int s);

    /*
      Método: add
      Insere uma nova chave na tabela.
      - Calcula o índice usando a função hash.
      - Adiciona a chave na lista correspondente.
    */
    void add(int key);

    /*
      Método: busca
      Procura uma chave na tabela.
      - Calcula o índice pelo hash.
      - Percorre a lista correspondente e verifica se o valor existe.
      Retorna true se a chave for encontrada, false caso contrário.
    */
    bool busca(int key);

    /*
      Método: display
      Exibe toda a tabela hash no console.
      Para cada posição, mostra os valores encadeados e finaliza com "NULL".
    */
    void display();
};
