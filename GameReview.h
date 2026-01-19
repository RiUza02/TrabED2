#ifndef GAMEREVIEW_H
#define GAMEREVIEW_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <cstdint>

using namespace std;

/*
_____________________________________________________________
  Classe: GameReview
  Objetivo: Representar uma avaliação de jogo com os campos:
            - recommendationid
            - appid
            - author_steamid
            - weighted_vote_score
_____________________________________________________________
*/
class GameReview {
private:
  string recommendationid;
  int appid;
  string author_steamid;
  float weighted_vote_score;
  long filePosition; // Posição do registro no arquivo binário (nova)

  static int countRecords();

  // Métodos auxiliares para compressão (privados)
  static string comprimeLZW(const string& str);
  static string descomprimeLZW(const string& str);
  static void comprimeArquivoLZW(const string& arquivoEntrada, 
                                 const string& arquivoSaida);
  static void descomprimeArquivoLZW(const string& arquivoEntrada, 
                                    const string& arquivoSaida);

public:
  // Construtor
  GameReview();

  // Gets
  string getRecommendationId() const { return recommendationid; }
  int getAppId() const { return appid; }
  string getAuthorSteamId() const { return author_steamid; }
  float getWeightedVoteScore() const { return weighted_vote_score; }
  long getFilePosition() const { return filePosition; } // Nova

  // Sets
  void setFilePosition(long pos) { filePosition = pos; } // Nova

  // Imprime os campos da avaliação
  void print() const;

  // Gera o arquivo binário correspondente ao arquivo CSV
  void createBinary(const std::string &path);

  // Acessa e imprime o i-ésimo registro do arquivo binário
  static bool getReview(int index, GameReview &review);

  // Acessa registro pela posição no arquivo (nova)
  static bool getReviewByPosition(long position, GameReview &review);

  // Método auxiliar para preencher os dados (necessário para o print funcionar
  // após ler do binário)
  bool analisalinhaCSV(const string &line);

  // Importa N reviews aleatórias do arquivo binário
  static GameReview *import(int n, const string &caminho);

  // ==================== FUNÇÕES DE COMPRESSÃO OBRIGATÓRIAS ====================
  
  // Comprime uma string usando o método especificado
  // 0 = Huffman, 1 = LZ77, 2 = LZW
  static string comprime(string str, int metodo);
  
  // Descomprime uma string usando o método especificado
  // 0 = Huffman, 1 = LZ77, 2 = LZW
  static string descomprime(string str, int metodo);
  
  // Comprime o arquivo texto reviewsOrig.txt
  // Salva o resultado em reviewsComp.bin
  static void comprime(int metodo);
  
  // Descomprime o arquivo binário reviewsComp.bin
  // Salva o resultado em reviewsDesc.txt
  static void descomprime(int metodo);

  // ==================== FUNÇÕES PARA ANÁLISE ====================
  
  // Gera string concatenada de N registros (para compressão)
  static string gerarStringParaCompressao(int n, const string& caminho);
  
  // Calcula taxa de compressão
  static double calcularTaxaCompressao(const string& original, const string& comprimido);
  
  // Executa análise de compressão (Etapa 4)
  static void analisarCompressao(int metodo, int M, const string& caminho);

  // ==================== FUNÇÕES PARA ÁRVORE B ====================
  
  // Gera ID único para a árvore B (appid + author_steamid)
  string gerarIdUnico() const;
  
  // Comparador para ordenação na árvore B
  static bool compararIds(const GameReview& a, const GameReview& b);
};

#endif // GAMEREVIEW_H