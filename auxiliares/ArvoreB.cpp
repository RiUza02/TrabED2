#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <random>
#include <ctime>
#include <iomanip>
#include <fstream>

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
    int numComparacao; 

    // Busca Recursiva
    bool buscarRecursivo(NoB *atual, string k)
    {
        int i = 0;
        while (i < atual->chaves.size())
        {
            numComparacao++;
            if (k > atual->chaves[i])
                i++;
            else
                break;
        }
        
        numComparacao++; // Conta a comparação de igualdade/falha logo abaixo
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

            while (i >= 0)
            {
                numComparacao++;
                if (no->chaves[i] > k)
                {
                    no->chaves[i + 1] = no->chaves[i];
                    i--;
                }
                else
                {
                    break;
                }
            }

            no->chaves[i + 1] = k;
        }
        else
        {
            while (i >= 0)
            {
                numComparacao++;
                if (no->chaves[i] > k)
                    i--;
                else
                    break;
            }
            i++;

            if (no->filhos[i]->chaves.size() == M - 1)
            {
                dividirFilho(no, i, no->filhos[i]);
                
                numComparacao++;
                if (k > no->chaves[i])
                    i++;
            }
            inserirNaoCheio(no->filhos[i], k);
        }
    }

    void deletarArvore(NoB* no) {
        if (!no) return;
        if (!no->folha) {
            for (auto filho : no->filhos) {
                deletarArvore(filho);
            }
        }
        delete no;
    }

    // Impressão In-Ordem (Recursivo)
    void imprimirInOrdemRecursivo(NoB *no)
    {
        if (!no) return;
        int i;
        for (i = 0; i < no->chaves.size(); i++)
        {
            if (!no->folha) imprimirInOrdemRecursivo(no->filhos[i]);
            cout << no->chaves[i] << endl;
        }
        if (!no->folha) imprimirInOrdemRecursivo(no->filhos[i]);
    }

    // Impressão Visual Hierárquica
    void imprimirVisualRecursivo(NoB *no, string prefixo, bool ehUltimo)
    {
        if (!no) return;
        cout << prefixo;
        if (ehUltimo) {
            cout << "L__ ";
            prefixo += "    ";
        } else {
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
        numComparacao = 0;
    }
    
    ~ArvoreB()
    {
        deletarArvore(raiz);
    }

    void inserir(string a, string b)
    {
        string k = a + b;

        // Se a chave já existe, não insere (buscar já incrementa comparacoes)
        if (raiz != nullptr && buscar(a, b))
        {
            return;
        }

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

    void resetComparacoes() {
        numComparacao = 0;
    }
    
 
    int getComparacoes() const {
        return numComparacao;
    }


    struct ResultadoTeste {
        double tempoExecucao; 
        int comparacoesInsercao;
        double tempoBusca; 
        int comparacoesBusca;
    }; 
    
    void imprimir()
    {
        if (!raiz)
        {
            cout << "Arvore vazia." << endl;
            return;
        }

        cout << "=== ESTRUTURA VISUAL DA ARVORE ===" << endl;
        imprimirVisualRecursivo(raiz, "", true);
        cout << endl;

        cout << "=== LISTAGEM ORDENADA (IN-ORDER) ===" << endl;
        imprimirInOrdemRecursivo(raiz);
        cout << "====================================" << endl;

        cout << "=== TOTAL DE ELEMENTOS: " << totalElementos << " chaves ===" << endl;
        cout << "====================================" << endl << endl << endl;
    }
};