#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <bitset>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

using namespace std;

// Estrutura do Nó da Árvore de Huffman
struct NoHuffman {
  char caractere;
  int frequencia;
  NoHuffman *esquerda;
  NoHuffman *direita;

  NoHuffman(char c, int f)
      : caractere(c), frequencia(f), esquerda(nullptr), direita(nullptr) {}
};

// Comparador para a fila de prioridade (Min-Heap)
struct CompararNo {
  bool operator()(NoHuffman *l, NoHuffman *r) {
    return l->frequencia > r->frequencia;
  }
};

class Huffman {
private:
  NoHuffman *raiz;
  map<char, string> tabelaCodigos;
  map<string, char> tabelaDecodificacao;

  void limparArvore(NoHuffman *no);
  void gerarCodigos(NoHuffman *no, string codigo);
  void serializarTabela(ofstream &out);
  void deserializarTabela(ifstream &in);
  NoHuffman *construirArvore(const map<char, int> &frequencias);

  void escreverBit(char bit, unsigned char &buffer, int &contador,
                   ofstream &out);

public:
  Huffman();
  ~Huffman();

  string comprimeString(string texto);
  string descomprimeString(string textoCodificado);

  void comprimeArquivo(string arquivoEntrada, string arquivoSaida);
  void descomprimeArquivo(string arquivoEntrada, string arquivoSaida);
};

#endif
