#include "GameReview.cpp"
#include "auxiliares/hash1.cpp" // contém a tabela Hash1
#include "auxiliares/hash2.cpp" // contém a tabela Hash2
#include "auxiliares/leitura.h" // contém funções de leitura de CSV
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

// Função para contar repetições de jogos
void contarJogosRepetidos(int n, const string& caminho) {
    cout << ">>>> Importando " << n << " registros aleatorios..." << endl;
    
    GameReview* reviews = GameReview::import(n, caminho);
    if (!reviews) {
        cout << "Erro ao importar registros!" << endl;
        return;
    }

    // Mapa para contar as repetições de cada appid
    map<int, int> contadorJogos;
    
    // Contar as ocorrências de cada jogo
    for (int i = 0; i < n; i++) {
        int appid = reviews[i].getAppId();
        contadorJogos[appid]++;
    }

    // Converter o mapa para um vetor para ordenação
    vector<pair<int, int>> jogosOrdenados(contadorJogos.begin(), contadorJogos.end());
    
    // Ordenar por quantidade de repetições (decrescente)
    sort(jogosOrdenados.begin(), jogosOrdenados.end(), 
         [](const pair<int, int>& a, const pair<int, int>& b) {
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
    for (const auto& par : jogosOrdenados) {
        if (par.second > 1) {
            jogosComRepeticao++;
        }
    }
    
    cout << "Jogos que se repetem: " << jogosComRepeticao << endl;
    cout << "=============================================" << endl;
    
    // Exibir os top 10 jogos mais repetidos
    cout << "\nTOP 10 JOGOS MAIS REPETIDOS:" << endl;
    cout << "-----------------------------" << endl;
    int limite = min(10, (int)jogosOrdenados.size());
    
    for (int i = 0; i < limite; i++) {
        cout << i + 1 << ". Jogo ID: " << jogosOrdenados[i].first 
             << " - Repetições: " << jogosOrdenados[i].second << endl;
    }

    // Exibir alguns exemplos de jogos que aparecem apenas uma vez
    int jogosUnicos = 0;
    for (const auto& par : contadorJogos) {
        if (par.second == 1) {
            jogosUnicos++;
        }
    }
    
    cout << "\nJogos que aparecem apenas uma vez: " << jogosUnicos << endl;
    
    // Limpar memória
    delete[] reviews;
}

int main() {
    // Caminho do arquivo CSV
    std::string caminho = "public/reviews.csv";

    GameReview gameManager;

    // Lê todas a lista de recomendações do arquivo
    auto recomendacoes = lerCSVRecomendacoes(caminho, true);

    // Verifica se leu algo
    if (recomendacoes.empty()) {
        std::cout << "Nenhum dado foi lido." << std::endl;
        return 1;
    }

    std::cout << "Gerando arquivo binario..." << endl;
    gameManager.createBinary(caminho);

    std::cout << "Leitura concluida com sucesso!" << std::endl;
    std::cout << "Total de registros: " << recomendacoes.size() << std::endl
              << std::endl;

    Hash1 tabela1(300);           // Tamanho da tabela hash (sondagem dupla)
    Hash2 tabela2(300);           // Tamanho da tabela hash (sondagem linear)
    for (int i = 0; i < 250; ++i) // quantidade de elementos a inserir
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
    do {
        cout << "\n_________________" << endl;
        cout << "           MENU PRINCIPAL" << endl;
        cout << "_________________" << endl;
        cout << "1 - Gerar tabela hash" << endl;
        cout << "2 - Exibir X jogos mais avaliados" << endl;
        cout << "3 - Sair" << endl;
        cout << "4 - Opcao extra para testes" << endl;
        cout << "5 - Import" << endl;
        cout << "6 - Contar jogos repetidos" << endl;  // NOVA OPÇÃO
        cout << "________________" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;
        
        if (opcao < 1 || opcao > 6) {
            cout << "Opcao invalida!" << endl;
            continue;
        }
        cout << endl;

        switch (opcao) {
        case 1: {
            cout << ">>>> Opcao 1: Gerar tabela hash" << endl;
            int chave;
            cout << "Digite a chave para gerar a tabela hash: ";
            cin >> chave;
            // Aqui você pode chamar a função para gerar a tabela hash com a chave
            // fornecida RegistroHash* createTable(int n):
            break;
        }

        case 2: {
            cout << ">>>> Opcao 2: Exibir X jogos mais avaliados" << endl;
            // Pega a tabela gerada na Opção 1, ordena e exibe.
            break;
        }

        case 3: {
            cout << ">>>> Encerrando programa" << endl;
            break;
        }

        case 4: {
            cout << ">>>> Opcao extra: Testar getReview" << endl;
            GameReview review;
            int index;
            cout << "Digite o indice do registro a ser exibido: ";
            cin >> index;
            if (GameReview::getReview(index, review)) {
                review.print();
            } else {
                cout << "Verifique se o indice é valido e tente novamente." << endl;
            }
            break;
        }

        case 5: {
            cout << ">>> Import" << endl;
            GameReview review;
            int numReg;
            cout << "Digite o numero de registros aleatorios que deseja: ";
            cin >> numReg;
            GameReview* importado = GameReview::import(numReg, caminho);
            if (importado) {
                for (int i = 0; i < numReg; i++) {
                    importado[i].print();
                }
                delete[] importado;
            }
            break;
        }

        case 6: {  // NOVA OPÇÃO
            cout << ">>>> Contar jogos repetidos em registros aleatorios" << endl;
            int numReg;
            cout << "Digite o numero de registros aleatorios para analisar: ";
            cin >> numReg;
            
            if (numReg <= 0) {
                cout << "Numero invalido! Deve ser maior que 0." << endl;
                break;
            }
            
            contarJogosRepetidos(numReg, caminho);
            break;
        }

        default: {
            cout << ">>>> Opcao invalida! Tente novamente." << endl;
            break;
        }
        }

    } while (opcao != 3);

    return 0;
}