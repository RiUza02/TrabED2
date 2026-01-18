#include "Huffman.h"

Huffman::Huffman() : raiz(nullptr) {}

Huffman::~Huffman() { limparArvore(raiz); }

void Huffman::limparArvore(NoHuffman *no) {
  if (no) {
    limparArvore(no->esquerda);
    limparArvore(no->direita);
    delete no;
  }
}

NoHuffman *Huffman::construirArvore(const map<char, int> &frequencias) {
  priority_queue<NoHuffman *, vector<NoHuffman *>, CompararNo> pq;

  for (auto const &[key, val] : frequencias) {
    pq.push(new NoHuffman(key, val));
  }

  while (pq.size() > 1) {
    NoHuffman *esquerda = pq.top();
    pq.pop();
    NoHuffman *direita = pq.top();
    pq.pop();

    int somaFreq = esquerda->frequencia + direita->frequencia;
    NoHuffman *novoNo = new NoHuffman('\0', somaFreq);
    novoNo->esquerda = esquerda;
    novoNo->direita = direita;

    pq.push(novoNo);
  }

  return pq.top();
}

// Gera o mapa de caracteres para códigos binários (ex: 'A' -> "01")
void Huffman::gerarCodigos(NoHuffman *no, string codigo) {
  if (!no)
    return;

  if (!no->esquerda && !no->direita) {
    tabelaCodigos[no->caractere] = codigo;
    tabelaDecodificacao[codigo] = no->caractere;
  }

  gerarCodigos(no->esquerda, codigo + "0");
  gerarCodigos(no->direita, codigo + "1");
}

string Huffman::comprimeString(string texto) {
  if (texto.empty())
    return "";

  map<char, int> frequencias;
  for (char c : texto)
    frequencias[c]++;

  limparArvore(raiz);
  raiz = construirArvore(frequencias);

  tabelaCodigos.clear();
  tabelaDecodificacao.clear();
  gerarCodigos(raiz, "");
  string resultado = "";
  for (char c : texto) {
    resultado += tabelaCodigos[c];
  }

  return resultado;
}

string Huffman::descomprimeString(string textoCodificado) {
  string resultado = "";
  string codigoAtual = "";

  for (char bit : textoCodificado) {
    codigoAtual += bit;
    if (tabelaDecodificacao.find(codigoAtual) != tabelaDecodificacao.end()) {
      resultado += tabelaDecodificacao[codigoAtual];
      codigoAtual = "";
    }
  }
  return resultado;
}

void Huffman::serializarTabela(ofstream &out) {
  int tamanho = tabelaCodigos.size();
  out.write(reinterpret_cast<char *>(&tamanho), sizeof(tamanho));

  for (auto const &[caractere, codigo] : tabelaCodigos) {
    out.write(&caractere, sizeof(char)); // Escreve o char
    int freq = 0; // Salvando apenas o char e o tamanho do código, ao invés de
                  // reconstruir a mesma árvore e salvar as frequencias.
  }
}

void Huffman::comprimeArquivo(string arquivoEntrada, string arquivoSaida) {
  ifstream in(arquivoEntrada);
  if (!in.is_open()) {
    cerr << "Erro ao abrir arquivo de entrada: " << arquivoEntrada << endl;
    return;
  }

  string conteudo((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
  in.close();

  if (conteudo.empty())
    return;

  map<char, int> frequencias;
  for (char c : conteudo)
    frequencias[c]++;

  limparArvore(raiz);
  raiz = construirArvore(frequencias);
  tabelaCodigos.clear();
  gerarCodigos(raiz, "");

  ofstream out(arquivoSaida, ios::binary);

  int tamMap = frequencias.size();
  out.write(reinterpret_cast<char *>(&tamMap), sizeof(tamMap));
  for (auto par : frequencias) {
    out.write(&par.first, sizeof(char));
    out.write(reinterpret_cast<char *>(&par.second), sizeof(int));
  }

  long long totalChars = conteudo.size();
  out.write(reinterpret_cast<char *>(&totalChars), sizeof(long long));

  unsigned char buffer = 0;
  int contadorBits = 0;

  for (char c : conteudo) {
    string codigo = tabelaCodigos[c];
    for (char bit : codigo) {
      buffer = buffer << 1;
      if (bit == '1') {
        buffer = buffer | 1;
      }
      contadorBits++;

      if (contadorBits == 8) { // Buffer cheio, escreve no arquivo
        out.write(reinterpret_cast<char *>(&buffer), sizeof(buffer));
        buffer = 0;
        contadorBits = 0;
      }
    }
  }

  // Escreve os bits restantes (padding) se houver
  if (contadorBits > 0) {
    buffer = buffer << (8 - contadorBits);
    out.write(reinterpret_cast<char *>(&buffer), sizeof(buffer));
  }

  out.close();
  cout << "Arquivo compactado gerado: " << arquivoSaida << endl;
}

void Huffman::descomprimeArquivo(string arquivoEntrada, string arquivoSaida) {
  ifstream in(arquivoEntrada, ios::binary);
  if (!in.is_open()) {
    cerr << "Erro ao abrir arquivo compactado." << endl;
    return;
  }

  // Ler Cabeçalho (Frequencias)
  int tamMap = 0;
  in.read(reinterpret_cast<char *>(&tamMap), sizeof(tamMap));

  map<char, int> frequencias;
  for (int i = 0; i < tamMap; i++) {
    char c;
    int f;
    in.read(&c, sizeof(char));
    in.read(reinterpret_cast<char *>(&f), sizeof(int));
    frequencias[c] = f;
  }

  long long totalCharsOriginal;
  in.read(reinterpret_cast<char *>(&totalCharsOriginal), sizeof(long long));

  limparArvore(raiz);
  raiz = construirArvore(frequencias);

  ofstream out(arquivoSaida);
  NoHuffman *atual = raiz;
  char byteLido;
  long long charsDecodificados = 0;

  while (in.read(&byteLido, sizeof(char)) &&
         charsDecodificados < totalCharsOriginal) {
    for (int i = 7; i >= 0; i--) {
      int bit = (byteLido >> i) & 1;

      if (bit == 0)
        atual = atual->esquerda;
      else
        atual = atual->direita;

      if (!atual->esquerda && !atual->direita) {
        out << atual->caractere;
        charsDecodificados++;
        atual = raiz;

        if (charsDecodificados == totalCharsOriginal)
          break;
      }
    }
  }

  out.close();
  in.close();
  cout << "Arquivo descompactado gerado: " << arquivoSaida << endl;
}
