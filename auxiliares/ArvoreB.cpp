#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct NoB
{
    vector<int> chaves;   // Chaves do nó
    vector<NoB *> filhos; // Ponteiros para filhos
    bool folha;           // Flag identificadora de folha

    NoB(bool ehFolha) { folha = ehFolha; }
};

class ArvoreB
{
private:
    NoB *raiz;
    int M; // Ordem da árvore

    // ----------------------------------------------------------------------
    // Método buscarRecursivo
    // Realiza a busca de uma chave k a partir de um nó atual.
    // A busca é feita de forma recursiva, utilizando busca binária
    // interna no nó para decidir se a chave está presente ou
    // para qual filho a busca deve continuar.
    // ----------------------------------------------------------------------
    bool buscarRecursivo(NoB *atual, int k)
    {
        int i = 0;

        // Avança no vetor de chaves até encontrar
        // a primeira chave maior ou igual a k
        while (i < atual->chaves.size() && k > atual->chaves[i])
            i++;

        // Se a chave foi encontrada no nó atual
        if (i < atual->chaves.size() && atual->chaves[i] == k)
            return true;

        // Se não encontrou e o nó é folha,
        // a chave não existe na árvore
        if (atual->folha)
            return false;

        // Caso contrário, continua a busca
        // no filho correspondente
        return buscarRecursivo(atual->filhos[i], k);
    }

    // ----------------------------------------------------------------------
    // Método dividirFilho
    // Divide um nó filho que está cheio e promove a chave mediana
    // para o nó pai. Essa operação mantém as propriedades da Árvore B,
    // garantindo balanceamento e limite máximo de chaves por nó.
    // ----------------------------------------------------------------------
    void dividirFilho(NoB *pai, int indice, NoB *cheio)
    {
        // Calcula o índice da chave mediana
        int meio = cheio->chaves.size() / 2;

        // Cria um novo nó com a mesma característica de folha
        NoB *novo = new NoB(cheio->folha);

        // Armazena a chave mediana que será promovida ao pai
        int chaveMediana = cheio->chaves[meio];

        // Move as chaves da metade direita para o novo nó
        auto it_chaves = cheio->chaves.begin() + meio + 1;
        novo->chaves.assign(it_chaves, cheio->chaves.end());

        // Se o nó não for folha, move também os filhos correspondentes
        if (!cheio->folha)
        {
            auto it_filhos = cheio->filhos.begin() + (meio + 1);
            novo->filhos.assign(it_filhos, cheio->filhos.end());

            // Remove os filhos movidos do nó original
            cheio->filhos.erase(it_filhos, cheio->filhos.end());
        }

        // Remove do nó original as chaves que foram movidas
        // e a própria chave mediana
        cheio->chaves.erase(cheio->chaves.begin() + meio,
                            cheio->chaves.end());

        // Insere o novo nó como filho do pai, logo após o nó dividido
        pai->filhos.insert(pai->filhos.begin() + indice + 1, novo);

        // Insere a chave mediana no nó pai, na posição correta
        pai->chaves.insert(pai->chaves.begin() + indice, chaveMediana);
    }

    // ----------------------------------------------------------------------
    // Método inserirNaoCheio
    // Insere uma chave k em um nó que sabemos previamente
    // não estar cheio. Caso o nó seja interno, a função
    // garante que a descida nunca ocorra para um nó cheio
    // (split proativo).
    // ----------------------------------------------------------------------
    void inserirNaoCheio(NoB *no, int k)
    {
        // Índice da última chave do nó
        int i = no->chaves.size() - 1;

        // Caso o nó seja folha, a inserção é direta
        if (no->folha)
        {
            // Cria espaço para a nova chave
            no->chaves.push_back(0);

            // Desloca as chaves maiores que k para a direita
            while (i >= 0 && no->chaves[i] > k)
            {
                no->chaves[i + 1] = no->chaves[i];
                i--;
            }

            // Insere a chave na posição correta
            no->chaves[i + 1] = k;
        }
        else
        {
            // Encontra o filho correto para descer
            while (i >= 0 && no->chaves[i] > k)
                i--;
            i++;

            // Se o filho escolhido estiver cheio,
            // realiza a divisão antes de continuar a inserção
            if (no->filhos[i]->chaves.size() == M - 1)
            {
                dividirFilho(no, i, no->filhos[i]);

                // Após a divisão, decide se deve descer
                // para o filho da direita
                if (k > no->chaves[i])
                    i++;
            }

            // Continua a inserção recursivamente
            inserirNaoCheio(no->filhos[i], k);
        }
    }

    // ----------------------------------------------------------------------
    // Método imprimirVisualRecursivo
    // Realiza a impressão hierárquica da Árvore B,
    // exibindo visualmente os níveis, ramificações
    // e chaves de cada nó.
    // ----------------------------------------------------------------------
    void imprimirVisualRecursivo(NoB *no, string prefixo, bool ehUltimo)
    {
        // Caso base: nó inexistente
        if (!no)
            return;

        // Imprime o prefixo da árvore
        cout << prefixo;

        // Define o formato visual do ramo
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

        // Imprime as chaves do nó atual
        cout << "[ ";
        for (int val : no->chaves)
            cout << val << " ";
        cout << "]" << endl;

        // Chama recursivamente a impressão dos filhos
        for (size_t i = 0; i < no->filhos.size(); i++)
        {
            bool ultimo = (i == no->filhos.size() - 1);
            imprimirVisualRecursivo(no->filhos[i], prefixo, ultimo);
        }
    }

public:
    // ----------------------------------------------------------------------
    // Construtor da Árvore B
    // Inicializa a árvore sem raiz e define a ordem M.
    // A ordem M determina o número máximo de filhos por nó (M)
    // e o número máximo de chaves (M - 1).
    // ----------------------------------------------------------------------
    ArvoreB(int ordem) : raiz(nullptr), M(ordem) {}

    // ----------------------------------------------------------------------
    // Método inserir
    // Responsável por inserir uma chave k na Árvore B.
    // Garante que as propriedades da Árvore B sejam mantidas,
    // realizando divisões de nós quando necessário.
    // ----------------------------------------------------------------------
    void inserir(int k)
    {
        // Caso a árvore esteja vazia, cria a raiz como nó folha
        if (!raiz)
        {
            raiz = new NoB(true); // Nó folha
            raiz->chaves.push_back(k);
        }
        else
        {
            // Se a raiz estiver cheia (M - 1 chaves),
            // é necessário criar uma nova raiz
            if (raiz->chaves.size() == M - 1)
            {
                // Cria uma nova raiz que não é folha
                NoB *novaRaiz = new NoB(false);

                // A antiga raiz passa a ser filha da nova raiz
                novaRaiz->filhos.push_back(raiz);

                // Divide o nó filho que está cheio
                dividirFilho(novaRaiz, 0, raiz);

                // Atualiza a raiz da árvore
                raiz = novaRaiz;

                // Insere a chave no nó apropriado
                inserirNaoCheio(raiz, k);
            }
            else
            {
                // Caso a raiz não esteja cheia,
                // a inserção pode ser feita diretamente
                inserirNaoCheio(raiz, k);
            }
        }
    }

    // ----------------------------------------------------------------------
    // Método buscar
    // Verifica se a chave k existe na Árvore B.
    // Retorna true se a chave for encontrada,
    // ou false caso contrário.
    // ----------------------------------------------------------------------
    bool buscar(int k)
    {
        // Se a árvore estiver vazia, não há o que buscar
        // Caso contrário, chama a busca recursiva
        return (raiz == nullptr) ? false : buscarRecursivo(raiz, k);
    }

    // ----------------------------------------------------------------------
    // Método imprimir
    // Exibe a estrutura da Árvore B de forma visual e hierárquica,
    // facilitando a compreensão da organização dos nós e chaves.
    // ----------------------------------------------------------------------
    void imprimir()
    {
        // Caso a árvore esteja vazia, informa ao usuário
        if (!raiz)
        {
            cout << "Arvore vazia." << endl;
            return;
        }

        // Imprime o título da visualização
        cout << "\nEstrutura da Arvore B:\n"
             << endl;

        // Chama a função recursiva responsável pela impressão visual
        imprimirVisualRecursivo(raiz, "", true);

        cout << endl;
    }
};