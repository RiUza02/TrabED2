#include <iostream>
#include "auxiliares/hash.cpp"  // contém a tabela HashTable
#include "auxiliares/leitura.h" // contém funções de leitura de CSV

using namespace std;

int main()
{
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

    std::cout << "Leitura concluida com sucesso!" << std::endl;
    std::cout << "Total de registros: " << recomendacoes.size() << std::endl
              << std::endl;

    HashTable tabela(300); // Tamanho da tabela hash
    for (int i = 0; i < 250; ++i)  //quantidade de elementos a inserir
    {
        int chave = stoi(recomendacoes[i].idAplicativo);
        tabela.add(chave);
    }
    tabela.display();
    return 0;
}
