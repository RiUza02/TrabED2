#include "GameReview.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <sstream>

using namespace std;

// ==================== IMPLEMENTAÇÃO BÁSICA LZW ====================

string GameReview::comprimeLZW(const string& str) {
    if (str.empty()) return "";
    
    unordered_map<string, int> dictionary;
    // Inicializa com todos os caracteres ASCII
    for (int i = 0; i < 256; i++) {
        dictionary[string(1, char(i))] = i;
    }
    
    string resultado;
    string w;
    int nextCode = 256;
    
    for (char c : str) {
        string wc = w + c;
        if (dictionary.find(wc) != dictionary.end()) {
            w = wc;
        } else {
            // Adiciona código ao resultado
            resultado += to_string(dictionary[w]) + " ";
            // Adiciona nova sequência ao dicionário
            dictionary[wc] = nextCode++;
            w = string(1, c);
        }
    }
    
    // Última sequência
    if (!w.empty()) {
        resultado += to_string(dictionary[w]);
    }
    
    return resultado;
}

string GameReview::descomprimeLZW(const string& str) {
    if (str.empty()) return "";
    
    vector<string> dictionary(256);
    for (int i = 0; i < 256; i++) {
        dictionary[i] = string(1, char(i));
    }
    
    // Converte string de códigos para vetor
    vector<int> codes;
    stringstream ss(str);
    string token;
    while (ss >> token) {
        codes.push_back(stoi(token));
    }
    
    string resultado;
    int nextCode = 256;
    
    if (codes.empty()) return "";
    
    // Primeiro código
    resultado += dictionary[codes[0]];
    string prev = dictionary[codes[0]];
    
    // Processa códigos restantes
    for (size_t i = 1; i < codes.size(); i++) {
        int code = codes[i];
        string entry;
        
        if (code < nextCode) {
            entry = dictionary[code];
        } else if (code == nextCode) {
            entry = prev + prev[0];
        } else {
            // Código inválido
            return "";
        }
        
        resultado += entry;
        
        // Adiciona ao dicionário
        if (nextCode < 4096) { // Limite do dicionário
            dictionary.push_back(prev + entry[0]);
            nextCode++;
        }
        
        prev = entry;
    }
    
    return resultado;
}

// ==================== FUNÇÕES PARA ARQUIVOS ====================

void GameReview::comprimeArquivoLZW(const string& arquivoEntrada, 
                                   const string& arquivoSaida) {
    // Lê arquivo
    ifstream entrada(arquivoEntrada);
    if (!entrada) {
        cerr << "Erro ao abrir: " << arquivoEntrada << endl;
        return;
    }
    
    string conteudo((istreambuf_iterator<char>(entrada)), 
                    istreambuf_iterator<char>());
    entrada.close();
    
    // Comprime
    string comprimido = comprimeLZW(conteudo);
    
    // Salva
    ofstream saida(arquivoSaida);
    if (!saida) {
        cerr << "Erro ao criar: " << arquivoSaida << endl;
        return;
    }
    
    saida << comprimido;
    saida.close();
    
    cout << "Arquivo comprimido: " << arquivoSaida << endl;
    cout << "Tamanho original: " << conteudo.size() << " bytes" << endl;
    cout << "Tamanho comprimido: " << comprimido.size() << " bytes" << endl;
}

void GameReview::descomprimeArquivoLZW(const string& arquivoEntrada, 
                                      const string& arquivoSaida) {
    // Lê arquivo comprimido
    ifstream entrada(arquivoEntrada);
    if (!entrada) {
        cerr << "Erro ao abrir: " << arquivoEntrada << endl;
        return;
    }
    
    string comprimido((istreambuf_iterator<char>(entrada)), 
                      istreambuf_iterator<char>());
    entrada.close();
    
    // Descomprime
    string descomprimido = descomprimeLZW(comprimido);
    
    // Salva
    ofstream saida(arquivoSaida);
    if (!saida) {
        cerr << "Erro ao criar: " << arquivoSaida << endl;
        return;
    }
    
    saida << descomprimido;
    saida.close();
    
    cout << "Arquivo descomprimido: " << arquivoSaida << endl;
}

// ==================== FUNÇÕES OBRIGATÓRIAS DO TRABALHO ====================

string GameReview::comprime(string str, int metodo) {
    if (metodo == 2) { // LZW
        return comprimeLZW(str);
    }
    cerr << "Método não implementado. Use 2 para LZW." << endl;
    return str;
}

string GameReview::descomprime(string str, int metodo) {
    if (metodo == 2) { // LZW
        return descomprimeLZW(str);
    }
    cerr << "Método não implementado. Use 2 para LZW." << endl;
    return str;
}

void GameReview::comprime(int metodo) {
    if (metodo != 2) {
        cerr << "Método não implementado. Use 2 para LZW." << endl;
        return;
    }
    
    string arquivoOrig = "reviewsOrig.txt";
    string arquivoComp = "reviewsComp.bin";
    
    comprimeArquivoLZW(arquivoOrig, arquivoComp);
}

void GameReview::descomprime(int metodo) {
    if (metodo != 2) {
        cerr << "Método não implementado. Use 2 para LZW." << endl;
        return;
    }
    
    string arquivoComp = "reviewsComp.bin";
    string arquivoDesc = "reviewsDesc.txt";
    
    descomprimeArquivoLZW(arquivoComp, arquivoDesc);
}