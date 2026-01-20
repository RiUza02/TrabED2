#include "GameReview.h"
#include "auxiliares/ArvoreB.cpp"
#include "auxiliares/Huffman.h"
#include "auxiliares/LZ77.h"
#include "auxiliares/leitura.h"
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

void gerarArquivoImport(int n) {
  GameReview *GR = new GameReview;

  GR->createBinary("public/reviews.csv");
  delete GR;

  GameReview *registros = GameReview::import(n, "public/reviews.bin");

  if (!registros) {
    cerr << "Erro fatal: Nao foi possivel importar registros." << endl;
    return;
  }

  ofstream arquivoTxt("reviewsOrig.txt");
  for (int i = 0; i < n; i++) {
    arquivoTxt << registros[i].getRecommendationId() << ","
               << registros[i].getAppId() << ","
               << registros[i].getAuthorSteamId() << ","
               << registros[i].getWeightedVoteScore() << "\n";
  }
  arquivoTxt.close();
  delete[] registros;
}

double executarCompressao(int metodo) {
  string arqOrigem = "reviewsOrig.txt";
  string arqSaida = "reviewsComp.bin";

  if (metodo == 0) {
    Huffman huff;
    huff.comprimeArquivo(arqOrigem, arqSaida);

    ifstream orig(arqOrigem, ios::binary | ios::ate);
    ifstream comp(arqSaida, ios::binary | ios::ate);

    if (!orig.is_open() || !comp.is_open())
      return 0.0;

    long tamOrig = orig.tellg();
    long tamComp = comp.tellg();

    orig.close();
    comp.close();

    double taxa = (1.0 - (double)tamComp / tamOrig) * 100.0;

    cout << "   [Relatorio] Original: " << tamOrig
         << " bytes -> Comprimido: " << tamComp << " bytes" << endl;
    cout << "   [Relatorio] Taxa de compressao: " << fixed << setprecision(2)
         << taxa << "%" << endl;

    return taxa;
  } else if (metodo == 1) {
    LZ77 lz77;
    lz77.comprimeArquivo(arqOrigem, arqSaida);

    ifstream orig(arqOrigem, ios::binary | ios::ate);
    ifstream comp(arqSaida, ios::binary | ios::ate);

    if (!orig.is_open() || !comp.is_open())
      return 0.0;

    long tamOrig = orig.tellg();
    long tamComp = comp.tellg();

    orig.close();
    comp.close();

    double taxa = (1.0 - (double)tamComp / tamOrig) * 100.0;

    cout << "   [Relatorio] Original: " << tamOrig
         << " bytes -> Comprimido: " << tamComp << " bytes" << endl;
    cout << "   [Relatorio] Taxa de compressao: " << fixed << setprecision(2)
         << taxa << "%" << endl;

    return taxa;
  } else if (metodo == 2) {
    // LZW - USANDO FUNÇÃO PÚBLICA comprime(int metodo)
    GameReview lzw;
    
    // Primeiro, precisamos ler o arquivo para passar para a função comprime(string, int)
    ifstream entrada(arqOrigem);
    if (!entrada) {
      cerr << "Erro ao abrir: " << arqOrigem << endl;
      return 0.0;
    }
    
    string conteudo((istreambuf_iterator<char>(entrada)), 
                    istreambuf_iterator<char>());
    entrada.close();
    
    // Comprime usando a função pública
    string comprimido = lzw.comprime(conteudo, 2);
    
    // Salva o resultado comprimido
    ofstream saida(arqSaida);
    if (!saida) {
      cerr << "Erro ao criar: " << arqSaida << endl;
      return 0.0;
    }
    
    saida << comprimido;
    saida.close();
    
    // Cálculo da taxa de compressão
    ifstream orig(arqOrigem, ios::binary | ios::ate);
    ifstream comp(arqSaida, ios::binary | ios::ate);

    if (!orig.is_open() || !comp.is_open())
      return 0.0;

    long tamOrig = orig.tellg();
    long tamComp = comp.tellg();

    orig.close();
    comp.close();

    double taxa = (1.0 - (double)tamComp / tamOrig) * 100.0;

    cout << "   [LZW] Original: " << tamOrig
         << " bytes -> Comprimido: " << tamComp << " bytes" << endl;
    cout << "   [LZW] Taxa de compressao: " << fixed << setprecision(2)
         << taxa << "%" << endl;

    return taxa;
  }
  return 0.0;
}

void executarDescompressao(int metodo) {
  string arqComp = "reviewsComp.bin";
  string arqSaida = "reviewsDesc.txt";

  if (metodo == 0) {
    Huffman huff;
    huff.descomprimeArquivo(arqComp, arqSaida);
  } else if (metodo == 1) {
    LZ77 lz77;
    lz77.descomprimeArquivo(arqComp, arqSaida);
  } else if (metodo == 2) {
    // LZW - USANDO FUNÇÃO PÚBLICA descomprime(string, int)
    GameReview lzw;
    
    // Lê o arquivo comprimido
    ifstream entrada(arqComp);
    if (!entrada) {
      cerr << "Erro ao abrir: " << arqComp << endl;
      return;
    }
    
    string comprimido((istreambuf_iterator<char>(entrada)), 
                      istreambuf_iterator<char>());
    entrada.close();
    
    // Descomprime usando a função pública
    string descomprimido = lzw.descomprime(comprimido, 2);
    
    // Salva o resultado descomprimido
    ofstream saida(arqSaida);
    if (!saida) {
      cerr << "Erro ao criar: " << arqSaida << endl;
      return;
    }
    
    saida << descomprimido;
    saida.close();
    
    cout << "   [LZW] Descompressao concluida!" << endl;
  }
}

//-------------------------------------------------

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

  if (datasetCompleto.empty()) {
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
      Se preferir rodar apenas com o tamanho real do arquivo (sem repetições),
     descomente a linha abaixo:
  */
  // if ((int)datasetCompleto.size() < N) N = (int)datasetCompleto.size();

  auto amostra = datasetCompleto;
  if (amostra.size() < (size_t)N) {
    int faltam = N - amostra.size();
    cout << "DEBUG: Completando com " << faltam << " registros aleatorios..."
         << endl;

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

  cout << "DEBUG: Amostra final gerada com " << amostra.size() << " registros!"
       << endl;

  // Cabeçalho
  arquivoSaida << "\n==============================================" << endl;
  arquivoSaida << " Relatorio da etapa 3: Estruturas Balanceadas." << endl;
  arquivoSaida << " N=" << N << ", B=" << B_BUSCAS << ", M=" << M_EXECUCOES
               << endl;
  arquivoSaida << "=================================================" << endl;

  random_device rd; // Semente para números aleatórios
  mt19937 gen(rd());
  uniform_int_distribution<> distrib(
      0, N - 1); // Distribuição para buscas aleatórias

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
      for (int i = 0; i < B_BUSCAS; i++)
        indicesBusca[i] = distrib(gen); // Índices aleatórios para buscas

      // 1. Inserção
      arvore.resetComparacoes();   // Reseta contador de comparações para não
                                   // acumular de medições anteriores
      clock_t inicioIns = clock(); // Início da medição de tempo
      for (int i = 0; i < N; i++) {
        arvore.inserir(datasetCompleto[i].idAplicativo,
                       datasetCompleto[i].idAutor); // concatenação dos ids
      }
      clock_t fimIns = clock(); // Fim da medição de tempo
      double tempoIns = double(fimIns - inicioIns) / CLOCKS_PER_SEC;
      int compIns =
          arvore.getComparacoes(); // Obtém número de comparações na inserção

      // 2. Busca
      arvore.resetComparacoes();
      clock_t inicioBusca = clock();
      for (int idx : indicesBusca) {
        arvore.buscar(datasetCompleto[idx].idAplicativo,
                      datasetCompleto[idx].idAutor); // concatenação dos ids
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
      arquivoSaida << "   Insercao: Tempo=" << fixed << setprecision(4)
                   << tempoIns << "s, Comps=" << compIns << endl;
      arquivoSaida << "   Busca:    Tempo=" << fixed << setprecision(6)
                   << tempoBusca << "s, Comps=" << compBusca << endl;
      arquivoSaida << "---------------------------------------------" << endl;
    }

    // Salva média das 3 execuções
    arquivoSaida << "MEDIA FINAL (m=" << ordem << "):" << endl;
    arquivoSaida << "   Insercao: " << (somaTempoIns / M_EXECUCOES) << "s | "
                 << (somaCompIns / M_EXECUCOES) << " comps" << endl;
    arquivoSaida << "   Busca:    " << (somaTempoBusca / M_EXECUCOES) << "s | "
                 << (somaCompBusca / M_EXECUCOES) << " comps" << endl;
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
    case 2: {
      int n_registros;
      int M = 3; // Quantidade de execuções para média pro relatorio
      cout << "Digite o numero de registros a importar: ";
      cin >> n_registros;

      cout << "\nEscolha o metodo (0=Huffman, 1=LZ77, 2=LZW): ";
      int met;
      cin >> met;

      double somaTaxas = 0.0;

      // Loop das M execuções
      for (int i = 0; i < M; i++) {
        cout << "\n--- Execucao " << (i + 1) << " de " << M << " ---" << endl;
        gerarArquivoImport(n_registros);
        cout << "Gerando arquivo de texto com " << n_registros
             << " registros..." << endl;
        somaTaxas += executarCompressao(met);
        executarDescompressao(met);
      }

      double media = somaTaxas / M;
      cout << "\n==============================================" << endl;
      cout << "MEDIA DA TAXA DE COMPRESSAO: " << fixed << setprecision(2)
           << media << "%" << endl;
      cout << "==============================================" << endl;

      // Salvar no saida.txt para comparacao
      ofstream saida("saida.txt", ios::app);
      saida << "Metodo " << met << " - Media Taxa: " << media << "%" << endl;
      saida.close();

      break;
    }
    case 0:
      cout << "Encerrando programa..." << endl;
      break;
    default:
      cout << "Opcao invalida!" << endl;
    }
  } while (opcao != 0);

  return 0;
}
