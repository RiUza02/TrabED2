#include <iostream>
#include <vector>
using namespace std;

/*
===========================================================
  Classe: RegistroHash2
  Objetivo: Implementar uma tabela hash com endereçamento
            aberto utilizando sondagem linear (linear probing).

  Descrição geral:
  - Cada posição da tabela armazena uma única chave inteira.
  - Colisões são resolvidas através da sondagem linear:
      índice = (h(key) + i) % size
  - h(key) = key % size
  - Uma posição vazia é marcada com o valor EMPTY (-1).
  - Chaves duplicadas NÃO são inseridas novamente.
===========================================================
*/
class RegistroHash2
{
private:
    vector<int> table;    // Armazena as chaves
    vector<bool> ocupado; // Indica se a posição está ocupada
    int size;             // Tamanho da tabela hash
    int colisoes;         // Contador de colisões
    int elementos;        // Contador de elementos inseridos
    const int EMPTY = -1; // Valor que indica posição vazia

    // Função hash para sondagem linear
    int hash(int key) { return key % size; }

public:
    // Construtor que inicializa a tabela hash com tamanho 's'
    RegistroHash2(int s) : size(s), colisoes(0), elementos(0)
    {
        table.resize(size, EMPTY);
        ocupado.resize(size, false);
    }

    // Adiciona uma chave à tabela hash
    void add(int key)
    {
        // Verifica se tabela já está cheia
        if (elementos >= size)
        {
            cout << "Erro: tabela cheia, nao foi possivel inserir " << key << endl;
            return;
        }

        int index = hash(key);

        // Sondagem linear
        for (int i = 0; i < size; i++)
        {
            int pos = (index + i) % size;

            // Encontrou posição vazia
            if (!ocupado[pos])
            {
                table[pos] = key;
                ocupado[pos] = true;
                elementos++;
                if (i > 0)
                    colisoes += i; // Conta colisões
                return;
            }

            // Verifica se a chave já existe (evita duplicatas)
            if (table[pos] == key)
            {
                return; // Chave duplicada, não insere
            }
        }

        // Se chegou aqui, percorreu toda a tabela e não achou espaço
        cout << "Erro: tabela cheia, nao foi possivel inserir " << key << endl;
    }

    // Busca uma chave na tabela hash
    bool busca(int key)
    {
        int index = hash(key);

        // Sondagem linear
        for (int i = 0; i < size; i++)
        {
            int pos = (index + i) % size;

            // Se encontrou posição vazia, a chave não existe
            if (!ocupado[pos])
                return false;

            // Se encontrou a chave
            if (table[pos] == key)
                return true;
        }
        return false;
    }

    // Exibe o conteúdo da tabela hash
    void display()
    {
        cout << "==============================" << endl;
        cout << "   TABELA HASH (Sondagem Linear)" << endl;
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
        cout << "==============================" << endl;
        cout << "Total de colisoes: " << colisoes << endl;
        cout << "Elementos inseridos: " << elementos << " / " << size << endl;
        cout << "Fator de carga: " << (elementos * 100.0 / size) << "%" << endl;
    }

    // Retorna o número de colisões
    int getColisoes() const { return colisoes; }

    // Retorna o número de elementos
    int getElementos() const { return elementos; }

    // Retorna o fator de carga (porcentagem de ocupação)
    double getFatorCarga() const { return (double)elementos / size * 100.0; }
};