#include "GameReview.h"
#include <iostream>
//#include "auxiliares/hash1.cpp"
//#include "auxiliares/hash2.cpp"

using namespace std;


GameReview::GameReview() : 
    recommendationid(0), 
    appid(0), 
    author_steamid(0), 
    weighted_vote_score(0.0f) 
{}

void GameReview::GameReview::print()
{
    cout << "________________________________________" << endl;
    cout << "Recommendation ID: " << recommendationid << endl;
    cout << "App ID: " << appid << endl;
    cout << "Author Steam ID: " << author_steamid << endl;
    cout << "Weighted Vote Score: " << weighted_vote_score << endl;
    cout << "_________________________________________" << endl;
}


void GameReview::getReview(int i){
    //preciso da função da leitura binária ;(
}