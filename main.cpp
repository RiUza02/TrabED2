#include <iostream>
#include "auxiliares/hash1.cpp"  // contém a tabela Hash1
#include "auxiliares/hash2.cpp"  // contém a tabela Hash2
#include "auxiliares/leitura.h" // contém funções de leitura de CSV
#include "GameReview.cpp"

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

    
    // Menu interativo
    int opcao;
    do
    {
        cout << "\n============================================" << endl;
        cout << "           MENU PRINCIPAL" << endl;
        cout << "============================================" << endl;
        cout << "1 - Gerar tabela hash" << endl;
        cout << "2 - Exibir X jogos mais avaliados" << endl;
        cout << "3 - Sair" << endl;
        cout << "============================================" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;
        if(opcao < 1 || opcao > 3){
            cout << "Opcao invalida!" << endl;
            cin >> opcao;
        }
        cout << endl;
        
        switch (opcao)
        {
            case 1:
            {
                cout << ">>> Opcao 1: Gerar tabela hash" << endl;
                int chave;
                cout << "Digite a chave para gerar a tabela hash: ";
                cin >> chave;
                // Aqui você pode chamar a função para gerar a tabela hash com a chave fornecida 
                // RegistroHash* createTable(int n):
                break;
            }
            
            case 2:
            {
                cout << ">>> Opcao 2: Exibir X jogos mais avaliados" << endl;
                 int indice;
                cout << "Digite o indice de acesso no registro do arquivo: ";
                cin >> indice;
                getReview(indice);
                break;
            }
            
            case 3:
            {
                cout << ">>> Encerrando programa..." << endl;
                cout << "Ate logo!" << endl;
                break;
            }
            
            default:
            {
                cout << ">>> Opcao invalida! Tente novamente." << endl;
                break;
            }
        }
        
    } while (opcao != 3);
    


    return 0;
}
