#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <fstream>
#include <iomanip>

#include "GameReview.h"           
#include "auxiliares/leitura.h"   
#include "auxiliares/ArvoreB.cpp" 

using namespace std;


// FUNÇÃO DA ETAPA 3: Análise de Desempenho
void executarAnaliseArvoreB() {
    // Configurações - enunciado
    int N_DESEJADO = 1000000; 
    int B_BUSCAS = 100;       
    int M_EXECUCOES = 3;      
    vector<int> ORDENS_TESTE = {20, 200}; // Ordens referente ao enunciado

    // Abre arquivo
    ofstream arquivoSaida("saida.txt", ios::app); 
    if (!arquivoSaida.is_open()) {
        cout << "DEBUG: Não foi possivel criar 'saida.txt'." << endl;
        return;
    }

    // Carrega dados
    string caminhoArquivo = "public/reviews.csv";
    cout << "DEBUG: Carregamento de dados (ETAPA 0)" << endl;
    auto datasetCompleto = lerCSVRecomendacoes(caminhoArquivo, true);

    if (datasetCompleto.empty()){
        cout << "DEBUG: O dataset esta vazio." << endl;
        return;
    }
  

    int N = N_DESEJADO;
/*
    NOTA: O trabalho exige N = 1.000.000 para os testes estatísticos.
    Como o dataset original possui menos registros, completamos a amostra 
    sorteando aleatoriamente registros existentes 
    do próprio dataset (amostragem com reposição).
    Isso garante que os dados de teste sejam válidos.
    Se preferir rodar apenas com o tamanho real do arquivo (sem repetições), descomente a linha abaixo:
*/
    //if ((int)datasetCompleto.size() < N) N = (int)datasetCompleto.size(); 

  auto amostra = datasetCompleto;
   if (amostra.size() < (size_t)N) {
        int faltam = N - amostra.size();
        cout << "DEBUG: Completando com " << faltam << " registros aleatorios..." << endl;
        
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distribAmostra(0, datasetCompleto.size() - 1);
        
        for (int i = 0; i < faltam; i++) {
            int idx = distribAmostra(gen);
            amostra.push_back(datasetCompleto[idx]);
        }
    }

    // Atualiza N para o tamanho real
    N = amostra.size();
    
    cout << "DEBUG: Amostra final gerada com " << amostra.size() << " registros!" << endl;
    
    
    // Cabeçalho
    arquivoSaida << "\n==============================================" << endl;
    arquivoSaida << " Relatorio da etapa 3: Estruturas Balanceadas." << endl;
    arquivoSaida << " N=" << N << ", B=" << B_BUSCAS << ", M=" << M_EXECUCOES << endl;
    arquivoSaida << "=================================================" << endl;

    random_device rd; // Semente para números aleatórios
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, N - 1); // Distribuição para buscas aleatórias

    // Loop para diferentes ordens da Árvore B
    for (int ordem : ORDENS_TESTE) {
        cout << "\nAnalisando Arvore B (m = " << ordem << ")" << endl;
        arquivoSaida << "\n[ ARVORE B | ORDEM m = " << ordem << " ]" << endl;

        double somaTempoIns = 0.0;
        double somaTempoBusca = 0.0;
        int somaCompIns = 0;
        int somaCompBusca = 0;

        for (int m = 1; m <= M_EXECUCOES; m++) {
            cout << "   Execucao " << m << "/" << M_EXECUCOES << "..." << endl;
            
            ArvoreB arvore(ordem); // Cria árvore B de ordem m
            vector<int> indicesBusca(B_BUSCAS);
            for(int i=0; i<B_BUSCAS; i++)
               indicesBusca[i] = distrib(gen); // Índices aleatórios para buscas

            // 1. Inserção
            arvore.resetComparacoes(); // Reseta contador de comparações para não acumular de medições anteriores
            clock_t inicioIns = clock(); // Início da medição de tempo
            for (int i = 0; i < N; i++) {
                arvore.inserir(datasetCompleto[i].idAplicativo, datasetCompleto[i].idAutor); // concatenação dos ids
            }
            clock_t fimIns = clock(); // Fim da medição de tempo
            double tempoIns = double(fimIns - inicioIns) / CLOCKS_PER_SEC;
            int compIns = arvore.getComparacoes(); // Obtém número de comparações na inserção 

            // 2. Busca
            arvore.resetComparacoes();
            clock_t inicioBusca = clock();
            for (int idx : indicesBusca) {
                arvore.buscar(datasetCompleto[idx].idAplicativo, datasetCompleto[idx].idAutor); // concatenação dos ids
            }
            clock_t fimBusca = clock();
            double tempoBusca = double(fimBusca - inicioBusca) / CLOCKS_PER_SEC;
            int compBusca = arvore.getComparacoes();

            somaTempoIns += tempoIns;
            somaCompIns += compIns;
            somaTempoBusca += tempoBusca; 
            somaCompBusca += compBusca;

            // Salva individual os dados da execução
            arquivoSaida << "Execucao " << m << ":" << endl;
            arquivoSaida << "   Insercao: Tempo=" << fixed << setprecision(4) << tempoIns << "s, Comps=" << compIns << endl;
            arquivoSaida << "   Busca:    Tempo=" << fixed << setprecision(6) << tempoBusca << "s, Comps=" << compBusca << endl;
            arquivoSaida << "---------------------------------------------" << endl;
        }

        // Salva média das 3 execuções
        arquivoSaida << "MEDIA FINAL (m=" << ordem << "):" << endl;
        arquivoSaida << "   Insercao: " << (somaTempoIns / M_EXECUCOES) << "s | " << (somaCompIns / M_EXECUCOES) << " comps" << endl;
        arquivoSaida << "   Busca:    " << (somaTempoBusca / M_EXECUCOES) << "s | " << (somaCompBusca / M_EXECUCOES) << " comps" << endl;
        arquivoSaida << "=============================================" << endl;
    }
    
    cout << "\n Analise de arvores concluida. Resultados em 'saida.txt'." << endl;
    arquivoSaida.close();
}

// MAIN
int main() { 
    int opcao = 0;

    do {
        cout << "\nMENU INTERATIVO:" << endl;
        cout << "1 - Estruturas balanceadas - Arvore B (Etapa 3)" << endl; 
        cout << "2 - Compressão (Etapa 4)" << endl;    
        cout << "0 - Sair" << endl;
        cout << "Opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                executarAnaliseArvoreB();
                break;
            case 2:
                break;
            case 0:
                cout << "Encerrando programa..." << endl;
                break;
            default:
                cout << "Opcao invalida!" << endl;
        }
    } while (opcao != 0);

    return 0;
}