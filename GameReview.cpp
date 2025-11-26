#include "GameReview.h"
#include "auxiliares/leitura.h" 
#include <iostream>
#include <cstdlib> 
#include <ctime>   

using namespace std;

GameReview::GameReview() : 
    recommendationid(""), appid(0), author_steamid(""), weighted_vote_score(0.0f) {}

void GameReview::print() const {
    cout << "----------------------------------------" << endl;
    cout << "Registro ID   : " << recommendationid << endl;
    cout << "App ID (Jogo) : " << appid << endl;
    cout << "Autor SteamID : " << author_steamid << endl;
    cout << "Nota          : " << weighted_vote_score << endl;
    cout << "----------------------------------------" << endl;
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
    cout << "Arquivo binário criado com sucesso em: " << binPath << endl;
}

// Auxiliar para separar a string e preencher a classe
bool GameReview::parseFromCSVLine(const string& line) {
    vector<string> tokens = dividirLinhaCSV(line);
    
    if (tokens.size() < 4) {
        return false;
    }
    
    recommendationid = tokens[0];
    
    try {
        appid = stoi(tokens[1]);
    } catch (...) {
        appid = 0;
    }
    
    author_steamid = tokens[2];
    
    try {
        weighted_vote_score = stof(tokens[3]);
    } catch (...) {
        weighted_vote_score = 0.0f;
    }
    
    return true;
}


bool GameReview::getReview(int index, GameReview& review) {
    const string binPath = "public/reviews.bin";
    ifstream binFile(binPath, ios::binary);

    if (!binFile.is_open()) {
        cout << "ERRO: Arquivo binario nao encontrado!" << endl;
        cout<< "Caminho: " << binPath << endl;
        cout << "Execute a opcao 'Gerar arquivo binario' primeiro." << endl;
        return false;
    }

    if (index < 0) {
        cout << "ERRO: Indice invalido (deve ser >= 0)." << endl;
        binFile.close();
        return false;
    }

    int currentIndex = 0;
    
    while (binFile.good() && binFile.peek() != EOF) {
        uint32_t size;
        
        // Lê o tamanho
        binFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        
        if (binFile.fail()) {
            break;
        }
        

        // Lê o conteúdo
        vector<char> buffer(size + 1);
        binFile.read(buffer.data(), size);
        buffer[size] = '\0';
        
        if (binFile.fail()) {
            break;
        }
        
        // Verifica se encontrou o índice desejado
        if (currentIndex == index) {
            string line(buffer.data());
            bool success = review.parseFromCSVLine(line);
            binFile.close();
            
            if (success) {
                return true;
            } else {
                cerr << "ERRO: Falha ao processar o registro." << endl;
                return false;
            }
        }
        
        currentIndex++;
    }

    cout << "ERRO: Registro com indice " << index << " nao encontrado." << endl;
    cout << "Total de registros disponiveis: " << currentIndex << endl;
    binFile.close();
    return false;
}