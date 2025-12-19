#include "auxiliares/ArvoreB.cpp"
#include "auxiliares/leitura.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <vector>

int main()
{
    // ----------------------------------------------
    // Leitura do arquivo de recomendações
    // ----------------------------------------------
    // Caminho do arquivo CSV
    std::string caminho = "public/reviews.csv";

    // Lê todas a lista de recomendações do arquivo
    auto recomendacoes = lerCSVRecomendacoes(caminho, true);

    // Verifica se leu algo
    if (recomendacoes.empty())
    {
        std::cout << "Nenhum dado foi lido." << std::endl;
        return 1;
    }

    // ----------------------------------------------
    // Criação e uso da Árvore B
    // ----------------------------------------------

    cout << "Tamanho do registro " << recomendacoes.size() << "..." << endl;

    // Definindo a ordem M na inicialização
    int ordemM = 4;
    ArvoreB arvore(ordemM);
    cout << "Inicializando Arvore B de ordem " << ordemM << "..." << endl;

    // gera numeros aleatórios para popular a árvore
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, recomendacoes.size());

    // adiciona valores na arvore
    for (int i = 0; i < 100; i++)
    {
        // gera um numero aleatório baseado no tamanho do vetor de recomendações
        int numero_aleatorio = distrib(gen);

        // insere o id do aplicativo na arvore
        string idApp = recomendacoes[numero_aleatorio].idAplicativo;
        string idAutor = recomendacoes[numero_aleatorio].idAutor;
        arvore.inserir(idApp, idAutor);
    }

    // ------------------------------
    // Imprime os valores da árvore em ordem crescente
    // ------------------------------
    cout << "Valores na arvore (em ordem crescente): ";
    arvore.imprimir();

    // Teste de busca
    string idApp = recomendacoes[0].idAplicativo;
    string idAutor = recomendacoes[0].idAutor;
    cout << "Buscando o valor " << idApp + idApp << "... " << endl;
    if (arvore.buscar(idApp, idAutor))
    {
        cout << "Resultado: O valor foi encontrado!" << endl;
    }
    else
    {
        cout << "Resultado: Valor nao presente na arvore." << endl;
    }

    return 0;
}