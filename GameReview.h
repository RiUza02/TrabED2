#ifndef GAMEREVIEW_H
#define GAMEREVIEW_H

#include <fstream>
#include <iostream>
#include <string>

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

  static int countRecords();

public:
  // Construtor
  GameReview();

  // Gets
  string getRecommendationId() const { return recommendationid; }
  int getAppId() const { return appid; }
  string getAuthorSteamId() const { return author_steamid; }
  float getWeightedVoteScore() const { return weighted_vote_score; }

  // Imprime os campos da avaliação
  void print() const;

  // Gera o arquivo binário correspondente ao arquivo CSV
  void createBinary(const std::string &path);

  // Acessa e imprime o i-ésimo registro do arquivo binário
  static bool getReview(int index, GameReview &review);

  // Método auxiliar para preencher os dados (necessário para o print funcionar
  // após ler do binário)
  bool analisalinhaCSV(const string &line);

  // Importa N reviews aleatórias do arquivo binário
  static GameReview *import(int n, const string &caminho);
};
#endif // GAMEREVIEW_H
