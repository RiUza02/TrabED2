#ifndef GAMEREVIEW_H
#define GAMEREVIEW_H

#include <string>
#include <iostream>
#include <fstream>

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
class GameReview
{
private:
    string recommendationid;
    int appid;
    string author_steamid;
    float weighted_vote_score;

public:
    // Construtor
    GameReview();

    // Acessa e imprime o i-ésimo registro do arquivo binário
    void getReview(int i);
};
#endif // GAMEREVIEW_H