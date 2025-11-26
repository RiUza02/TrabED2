#include "GameReview.h"
#include <iostream>
#include <fstream>
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

void GameReview::createBinary(const std::string &path){
    //Abre o arquivo .csv
    std::ifstream csvFile(path);

    if (!csvFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo CSV!" << std::endl;
        return;
    }

    //Nomeia o arquivo bin com o mesmo nome do arquivo csv
    std::string binPath = path.substr(0, path.find_last_of('.')) + ".bin";

    //Cria o arquivo .bin, se já existir um arquivo com esse nome ele apaga o conteúdo do mesmo
    std::ofstream bin(binPath, std::ios::binary);

    if (!bin.is_open()) {
        std::cerr << "Erro ao abrir o arquivo Binário!" << std::endl;
        return;
    }

    std::string line;
    //Percorre as linhas do arquivo .csv
    while (std::getline(csvFile, line)){
        //Grava a linha para ler depois
        uint32_t size = line.size();
        bin.write(reinterpret_cast<const char*>(&size), sizeof(size));

        //Grava o conteudo no arquivo
        bin.write(line.c_str(), size);
    }
    csvFile.close();
    bin.close();
}

void GameReview::getReview(int i){
    //preciso da função da leitura binária ;(
}