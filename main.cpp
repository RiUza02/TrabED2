#include <iostream>
#include "auxiliares/hash1.cpp"  // contém a tabela Hash1
#include "auxiliares/hash2.cpp"  // contém a tabela Hash2
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

    Hash1 tabela1(300); // Tamanho da tabela hash (sondagem dupla)
    Hash2 tabela2(300); // Tamanho da tabela hash (sondagem linear)
    for (int i = 0; i < 250; ++i)  //quantidade de elementos a inserir
    {
        int chave = stoi(recomendacoes[i].idAplicativo);
        tabela1.add(chave);
        tabela2.add(chave);
    }
    tabela1.display();
    std::cout << std::endl << std::endl << std::endl;
    tabela2.display();
    return 0;
}
