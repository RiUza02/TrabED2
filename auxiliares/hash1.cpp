#include <iostream>
#include <vector>
using namespace std;

/*
===========================================================
  Classe: Hash1
  Descrição geral:
  - Cada posição da tabela armazena uma única chave inteira.
  - Colisões são resolvidas através da sondagem dupla:
      índice = (h1(key) + i * h2(key)) % size
  - h1(key) = key % size
  - h2(key) deve ser coprimo com size para explorar toda tabela
  - Uma posição vazia é marcada com o valor EMPTY (-1).
  - Chaves duplicadas NÃO são inseridas novamente.
===========================================================
*/
class Hash1
{
private:
    vector<int> table;    // Armazena as chaves
    vector<bool> ocupado; // Indica se a posição está ocupada
    int size;             // Tamanho da tabela hash
    int colisoes;         // Contador de colisões
    int elementos;        // Contador de elementos inseridos
    const int EMPTY = -1; // Valor que indica posição vazia

    // Calcula MDC para verificar se dois números são coprimos
    int mdc(int a, int b)
    {
        while (b != 0)
        {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    // Primeira função hash para sondagem dupla
    int hash1(int key) { return key % size; }

    // Segunda função hash para sondagem dupla
    int hash2(int key)
    {
        int h2 = 1 + (key % (size - 1));
        while (mdc(h2, size) != 1)
        {
            h2++;
            if (h2 >= size)
                h2 = 1;
        }
        return h2;
    }

public:
    // construtor que inicializa a tabela hash com tamanho 's'
    Hash1(int s) : size(s), colisoes(0), elementos(0)
    {
        table.resize(size, EMPTY);
        ocupado.resize(size, false);
    }

    // adiciona uma chave à tabela hash
    void add(int key)
    {
        // Verifica se tabela já está cheia
        if (elementos >= size)
        {
            cout << "Erro: tabela cheia, nao foi possivel inserir " << key << endl;
            return;
        }
        int index = hash1(key);
        int step = hash2(key);

        // Sondagem dupla
        for (int i = 0; i < size; i++)
        {
            int pos = (index + i * step) % size;

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

    // busca uma chave na tabela hash
    bool busca(int key)
    {
        int index = hash1(key);
        int step = hash2(key);

        // Sondagem dupla
        for (int i = 0; i < size; i++)
        {
            int pos = (index + i * step) % size;

            // Se encontrou posição vazia, a chave não existe
            if (!ocupado[pos])
                return false;

            // Se encontrou a chave
            if (table[pos] == key)
                return true;
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