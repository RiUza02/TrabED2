#include "LZ77.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

static const int WINDOW_SIZE = 64;    // dicionário
static const int LOOKAHEAD_SIZE = 32; // buffer

void LZ77::comprimeArquivo(const string &arqOrigem, const string &arqSaida) {
  ifstream in(arqOrigem);
  ofstream out(arqSaida, ios::binary);

  if (!in.is_open() || !out.is_open())
    return;

  // Lê todo o arquivo texto
  string texto((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());

  string comprimido = comprimeLZ77(texto);

  // Escreve como binário (string serializada)
  out.write(comprimido.data(), comprimido.size());

  in.close();
  out.close();
  cout << "   [LZ77] Compressao concluida!" << endl;
}

void LZ77::descomprimeArquivo(const string &arqOrigem, const string &arqSaida) {
  ifstream in(arqOrigem, ios::binary);
  ofstream out(arqSaida);

  if (!in.is_open() || !out.is_open())
    return;

  string comprimido((istreambuf_iterator<char>(in)),
                    istreambuf_iterator<char>());

  string texto = descomprimeLZ77(comprimido);

  out << texto;

  in.close();
  out.close();
  cout << "   [LZ77] Descompressao concluida!" << endl;
}

string LZ77::comprimeLZ77(const string &input) {
  vector<LZ77Token> tokens;
  int n = input.size();
  int pos = 0;

  while (pos < n) {
    int bestOffset = 0, bestLength = 0;
    int windowStart = max(0, pos - WINDOW_SIZE);

    for (int j = windowStart; j < pos; ++j) {
      int length = 0;
      while (length < LOOKAHEAD_SIZE && pos + length < n &&
             input[j + length] == input[pos + length]) {
        ++length;
      }
      if (length > bestLength) {
        bestLength = length;
        bestOffset = pos - j;
      }
    }

    char nextChar = (pos + bestLength < n) ? input[pos + bestLength] : '\0';
    tokens.push_back({bestOffset, bestLength, nextChar});
    pos += (bestLength > 0) ? bestLength + 1 : 1;
  }

  string output;
  for (const auto &t : tokens) {
    output += "<" + to_string(t.offset) + "," + to_string(t.length) + "," +
              (t.next == '\0' ? "#" : string(1, t.next)) + ">";
  }
  return output;
}

string LZ77::descomprimeLZ77(const string &input) {
  string output;
  int i = 0;
  int n = input.size();

  while (i < n) {
    if (input[i] != '<') {
      ++i;
      continue;
    }
    ++i; // '<'

    int offset = 0;
    while (i < n && input[i] != ',')
      offset = offset * 10 + (input[i++] - '0');
    ++i; // ','

    int length = 0;
    while (i < n && input[i] != ',')
      length = length * 10 + (input[i++] - '0');
    ++i; // ','

    char nextChar = (i < n) ? input[i++] : '#';
    if (i < n && input[i] == '>')
      ++i;

    int start = static_cast<int>(output.size()) - offset;
    for (int j = 0; j < length; ++j) {
      if (start + j >= 0 && start + j < static_cast<int>(output.size()))
        output += output[start + j];
    }
    if (nextChar != '#')
      output += nextChar;
  }
  return output;
}
