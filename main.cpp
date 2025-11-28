#include "GameReview.cpp"
#include "auxiliares/hash1.cpp" // contém a tabela Hash1
#include "auxiliares/hash2.cpp" // contém a tabela Hash2
#include "auxiliares/leitura.h" // contém funções de leitura de CSV
#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <vector>

using namespace std;

// Função para contar repetições de jogos usando os dados já lidos
void contarJogosRepetidos(int n, const vector<Recomendacao> &recomendacoes)
{
    cout << ">>>> Selecionando " << n << " registros aleatorios..." << endl;

    if (n > recomendacoes.size())
    {
        cout << "Erro: Numero solicitado (" << n << ") maior que total disponivel ("
             << recomendacoes.size() << ")" << endl;
        return;
    }

    // Cria uma cópia das recomendações para embaralhar
    vector<Recomendacao> copiaRecomendacoes = recomendacoes;

    // Embaralha aleatoriamente
    random_device rd;
    mt19937 g(rd());
    shuffle(copiaRecomendacoes.begin(), copiaRecomendacoes.end(), g);

    // Pega os primeiros N elementos
    vector<Recomendacao> amostra(copiaRecomendacoes.begin(),
                                 copiaRecomendacoes.begin() + n);

    // Mapa para contar as repetições de cada appid
    map<int, int> contadorJogos;

    // Contar as ocorrências de cada jogo
    for (const auto &recomendacao : amostra)
    {
        int appid = stoi(recomendacao.idAplicativo);
        contadorJogos[appid]++;
    }

    // Converter o mapa para um vetor para ordenação
    vector<pair<int, int>> jogosOrdenados(contadorJogos.begin(),
                                          contadorJogos.end());

    // Ordenar por quantidade de repetições (decrescente)
    sort(jogosOrdenados.begin(), jogosOrdenados.end(),
         [](const pair<int, int> &a, const pair<int, int> &b)
         {
             return a.second > b.second;
         });

    // Exibir resultados
    cout << "\n=============================================" << endl;
    cout << "    CONTAGEM DE JOGOS REPETIDOS" << endl;
    cout << "=============================================" << endl;
    cout << "Total de registros analisados: " << n << endl;
    cout << "Total de jogos distintos: " << contadorJogos.size() << endl;
    cout << "=============================================" << endl;

    int jogosComRepeticao = 0;
    for (const auto &par : jogosOrdenados)
    {
        if (par.second > 1)
        {
            jogosComRepeticao++;
        }
    }

    cout << "Jogos que se repetem: " << jogosComRepeticao << endl;
    cout << "=============================================" << endl;

    // Exibir os top 10 jogos mais repetidos
    if (!jogosOrdenados.empty())
    {
        cout << "\nTOP 10 JOGOS MAIS REPETIDOS:" << endl;
        cout << "-----------------------------" << endl;
        int limite = min(10, (int)jogosOrdenados.size());

        for (int i = 0; i < limite; i++)
        {
            cout << i + 1 << ". Jogo ID: " << jogosOrdenados[i].first
                 << " - Repetições: " << jogosOrdenados[i].second << endl;
        }
    }

    // Exibir alguns exemplos de jogos que aparecem apenas uma vez
    int jogosUnicos = 0;
    for (const auto &par : contadorJogos)
    {
        if (par.second == 1)
        {
            jogosUnicos++;
        }
    }

    cout << "\nJogos que aparecem apenas uma vez: " << jogosUnicos << endl;
    cout << "Taxa de repetição: "
         << (jogosComRepeticao * 100.0 / contadorJogos.size()) << "%" << endl;
}

int main()
{
    // Caminho do arquivo CSV
    std::string caminho = "public/reviews.csv";

    GameReview gameManager;

    // Lê todas a lista de recomendações do arquivo
    auto recomendacoes = lerCSVRecomendacoes(caminho, true);

    // Verifica se leu algo
    if (recomendacoes.empty())
    {
        std::cout << "Nenhum dado foi lido." << std::endl;
        return 1;
    }

    std::cout << "Gerando arquivo binario..." << endl;
    gameManager.createBinary(caminho);

    std::cout << "Leitura concluida com sucesso!" << std::endl;
    std::cout << "Total de registros: " << recomendacoes.size() << std::endl
              << std::endl;

    // Menu interativo
    int opcao;
    do
    {
        cout << "\n_________________" << endl;
        cout << "   MENU PRINCIPAL" << endl;
        cout << "_________________" << endl;
        cout << "1 - Gerar tabela hash" << endl;
        cout << "2 - Exibir X jogos mais avaliados" << endl;
        cout << "3 - Sair" << endl;
        cout << "________________" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        if (opcao < 1 || opcao > 3)
        {
            cout << "Opcao invalida!" << endl;
            continue;
        }
        cout << endl;

        switch (opcao)
        {
        case 1:
        {
            cout << ">>>> Opcao 1: Gerar tabela hash" << endl;
            int chave;
            cout << "Digite o numero de elementos para gerar a tabela hash: ";
            cin >> chave;

            // Gera números aleatório
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(1, recomendacoes.size());

            Hash1 tabela1(chave * 1.1); // Tamanho da tabela hash (sondagem dupla)
            Hash2 tabela2(chave * 1.1); // Tamanho da tabela hash (sondagem linear
            for (int i = 0; i < chave; i++)
            {
                int numero_aleatorio = distrib(gen);
                int chave = stoi(recomendacoes[numero_aleatorio].idAplicativo);
                tabela1.add(chave);
                tabela2.add(chave);
            }
            tabela1.display();
            cout << endl;
            tabela2.display();
            break;
        }

        case 2:
        {
            cout << ">>>> Opcao 2: Exibir X jogos mais avaliados" << endl;
            cout << ">>>> Contar jogos repetidos em registros aleatorios" << endl;
            int numReg;
            cout << "Digite o numero de registros aleatorios para analisar: ";
            cin >> numReg;

            if (numReg <= 0)
            {
                cout << "Numero invalido! Deve ser maior que 0." << endl;
                break;
            }

            contarJogosRepetidos(numReg, recomendacoes);
            break;
        }

        case 3:
        {
            cout << ">>>> Encerrando programa" << endl;
            break;
        }

        case 4:
        {
            cout << ">>>> Opcao extra: Testar getReview" << endl;
            GameReview review;
            int index;
            cout << "Digite o indice do registro a ser exibido: ";
            cin >> index;
            if (GameReview::getReview(index, review))
            {
                review.print();
            }
            else
            {
                cout << "Verifique se o indice é valido e tente novamente." << endl;
            }
            break;
        }

        case 5:
        {
            cout << ">>> Import" << endl;
            int numReg;
            cout << "Digite o numero de registros aleatorios que deseja: ";
            cin >> numReg;
            GameReview *importado = GameReview::import(numReg, caminho);
            if (importado)
            {
                for (int i = 0; i < numReg; i++)
                {
                    importado[i].print();
                }
                delete[] importado;
            }
            break;
        }

        default:
        {
            cout << ">>>> Opcao invalida! Tente novamente." << endl;
            break;
        }
        }

    } while (opcao != 3);

    return 0;
}
