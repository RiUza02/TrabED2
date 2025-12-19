#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <random>

using namespace std;

// ----------------------------------------------------------------------
// Estrutura do nó da Árvore B
// ----------------------------------------------------------------------
struct NoB
{
    vector<string> chaves;
    vector<NoB *> filhos;
    bool folha;

    NoB(bool ehFolha) { folha = ehFolha; }
};

// ----------------------------------------------------------------------
// Classe ArvoreB
// ----------------------------------------------------------------------
class ArvoreB
{
private:
    NoB *raiz;
    int M;
    int totalElementos;

    // Busca Recursiva
    bool buscarRecursivo(NoB *atual, string k)
    {
        int i = 0;
        while (i < atual->chaves.size() && k > atual->chaves[i])
            i++;

        if (i < atual->chaves.size() && atual->chaves[i] == k)
            return true;

        if (atual->folha)
            return false;

        return buscarRecursivo(atual->filhos[i], k);
    }

    // Dividir Filho
    void dividirFilho(NoB *pai, int indice, NoB *cheio)
    {
        int meio = cheio->chaves.size() / 2;
        NoB *novo = new NoB(cheio->folha);

        string chaveMediana = cheio->chaves[meio];

        auto it_chaves = cheio->chaves.begin() + meio + 1;
        novo->chaves.assign(it_chaves, cheio->chaves.end());

        if (!cheio->folha)
        {
            auto it_filhos = cheio->filhos.begin() + (meio + 1);
            novo->filhos.assign(it_filhos, cheio->filhos.end());
            cheio->filhos.erase(it_filhos, cheio->filhos.end());
        }

        cheio->chaves.erase(cheio->chaves.begin() + meio, cheio->chaves.end());

        pai->filhos.insert(pai->filhos.begin() + indice + 1, novo);
        pai->chaves.insert(pai->chaves.begin() + indice, chaveMediana);
    }

    // Inserir Não Cheio
    void inserirNaoCheio(NoB *no, string k)
    {
        int i = no->chaves.size() - 1;

        if (no->folha)
        {
            no->chaves.push_back("");
            while (i >= 0 && no->chaves[i] > k)
            {
                no->chaves[i + 1] = no->chaves[i];
                i--;
            }
            no->chaves[i + 1] = k;
        }
        else
        {
            while (i >= 0 && no->chaves[i] > k)
                i--;
            i++;

            if (no->filhos[i]->chaves.size() == M - 1)
            {
                dividirFilho(no, i, no->filhos[i]);
                if (k > no->chaves[i])
                    i++;
            }
            inserirNaoCheio(no->filhos[i], k);
        }
    }

    // ------------------------------------------------------------------
    // NOVO: Método de Impressão In-Ordem (Recursivo)
    // Percorre a árvore da esquerda para a direita, gerando a lista ordenada.
    // ------------------------------------------------------------------
    void imprimirInOrdemRecursivo(NoB *no)
    {
        if (!no)
            return;

        int i;
        // Percorre todas as chaves do nó
        for (i = 0; i < no->chaves.size(); i++)
        {
            // Se não for folha, visita o filho à esquerda da chave[i] antes de imprimir
            if (!no->folha)
            {
                imprimirInOrdemRecursivo(no->filhos[i]);
            }

            // Imprime a chave atual
            cout << no->chaves[i] << endl;
        }

        // Importante: Visitar o último filho (à direita da última chave)
        if (!no->folha)
        {
            imprimirInOrdemRecursivo(no->filhos[i]);
        }
    }

    // Impressão Visual Hierárquica
    void imprimirVisualRecursivo(NoB *no, string prefixo, bool ehUltimo)
    {
        if (!no)
            return;

        cout << prefixo;
        if (ehUltimo)
        {
            cout << "L__ ";
            prefixo += "    ";
        }
        else
        {
            cout << "|__ ";
            prefixo += "|   ";
        }

        cout << "[ ";
        for (const string &val : no->chaves)
            cout << "\"" << val << "\" ";
        cout << "]" << endl;

        for (size_t i = 0; i < no->filhos.size(); i++)
        {
            bool ultimo = (i == no->filhos.size() - 1);
            imprimirVisualRecursivo(no->filhos[i], prefixo, ultimo);
        }
    }

public:
    ArvoreB(int ordem) : raiz(nullptr), M(ordem)
    {
        totalElementos = 0;
    }

    void inserir(string a, string b)
    {
        string k = a + b;

        // --------------------------------------------------
        // BLOQUEIO DE DUPLICATAS
        // --------------------------------------------------
        if (raiz != nullptr && buscar(a, b))
        {
            // Chave já existe → não insere
            return;
        }

        // Inserção normal
        totalElementos++;
        if (!raiz)
        {
            raiz = new NoB(true);
            raiz->chaves.push_back(k);
        }
        else
        {
            if (raiz->chaves.size() == M - 1)
            {
                NoB *novaRaiz = new NoB(false);
                novaRaiz->filhos.push_back(raiz);
                dividirFilho(novaRaiz, 0, raiz);
                raiz = novaRaiz;
                inserirNaoCheio(raiz, k);
            }
            else
            {
                inserirNaoCheio(raiz, k);
            }
        }
    }

    bool buscar(string a, string b)
    {
        string k = a + b;
        return (raiz == nullptr) ? false : buscarRecursivo(raiz, k);
    }

    // ------------------------------------------------------------------
    // Método imprimir (Interface Pública Atualizada)
    // Agora chama as duas formas de impressão.
    // ------------------------------------------------------------------
    void imprimir()
    {
        if (!raiz)
        {
            cout << "Arvore vazia." << endl;
            return;
        }

        // 1. Impressão Visual
        cout << "=== ESTRUTURA VISUAL DA ARVORE ===" << endl;
        imprimirVisualRecursivo(raiz, "", true);
        cout << endl;

        // 2. Impressão Ordenada
        cout << "=== LISTAGEM ORDENADA (IN-ORDER) ===" << endl;
        imprimirInOrdemRecursivo(raiz);
        cout << "====================================" << endl;

        cout << "=== TOTAL DE ELEMENTOS: " << totalElementos << " chaves ===" << endl;
        cout << "====================================" << endl
             << endl
             << endl;
    }
};