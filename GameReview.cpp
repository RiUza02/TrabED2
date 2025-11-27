#include "GameReview.h"
// #include "auxiliares/leitura.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

using namespace std;

GameReview::GameReview()
    : recommendationid(""), appid(0), author_steamid(""),
      weighted_vote_score(0.0f) {}

void GameReview::print() const {
  cout << "________________________________________" << endl;
  cout << "Registro ID   : " << recommendationid << endl;
  cout << "App ID (Jogo) : " << appid << endl;
  cout << "Autor SteamID : " << author_steamid << endl;
  cout << "Nota          : " << weighted_vote_score << endl;
  cout << "__________________________________________" << endl;
}

void GameReview::createBinary(const std::string &path) {
  // Abre o arquivo .csv
  std::ifstream csvFile(path);

  if (!csvFile.is_open()) {
    std::cerr << "Erro ao abrir o arquivo CSV!" << std::endl;
    return;
  }

  // Nomeia o arquivo bin com o mesmo nome do arquivo csv
  std::string binPath = path.substr(0, path.find_last_of('.')) + ".bin";

  // Cria o arquivo .bin, se já existir um arquivo com esse nome ele apaga o
  // conteúdo do mesmo
  std::ofstream bin(binPath, std::ios::binary);

  if (!bin.is_open()) {
    std::cerr << "Erro ao abrir o arquivo Binário!" << std::endl;
    return;
  }

  std::string line;
  // Percorre as linhas do arquivo .csv
  while (std::getline(csvFile, line)) {
    // Grava a linha para ler depois
    uint32_t size = line.size();
    bin.write(reinterpret_cast<const char *>(&size), sizeof(size));

    // Grava o conteudo no arquivo
    bin.write(line.c_str(), size);
  }
  csvFile.close();
  bin.close();
  cout << "Arquivo binário criado com sucesso em: " << binPath << endl;
}

// Analisa uma linha CSV e preenche os campos do objeto GameReview
bool GameReview::analisalinhaCSV(const string &line) {

  // vai até o fim do recommendationid
  size_t pos1 = line.find(',');
  if (pos1 == string::npos)
    return false; // Se não achar vírgula, erro

  // fim do appid
  size_t pos2 = line.find(',', pos1 + 1);
  if (pos2 == string::npos)
    return false;

  // fim do author_steamid
  size_t pos3 = line.find(',', pos2 + 1);
  if (pos3 == string::npos)
    return false;

  // extraindo os dados do início até a primeira vírgula
  recommendationid = line.substr(0, pos1);

  // extraindo os dados da primeira até a segunda vírgula
  string appid_str = line.substr(pos1 + 1, pos2 - pos1 - 1);
  try {
    appid = stoi(appid_str);
  } catch (...) {
    appid = 0;
  }

  // extraindo os dados do segunda até a terceira vírgula
  author_steamid = line.substr(pos2 + 1, pos3 - pos2 - 1);

  // extraindo os dados do terceira virgula até o fim da linha
  string score_str = line.substr(pos3 + 1);
  try {
    weighted_vote_score = stof(score_str);
  } catch (...) {
    weighted_vote_score = 0.0f;
  } // transforma pra float

  return true;
}

// Função auxiliar para contar quantos registros existem no arquivo para
// validação de índice
int GameReview::countRecords() {
  string binPath = "public/reviews.bin";
  ifstream binFile(binPath, ios::binary);
  if (!binFile.is_open()) {
    return -1; // indica erro ao abrir o arquivo
  }

  int total = 0;
  uint32_t size = 0;

  // conta os registros pulando o conteúdo
  while (binFile.read(reinterpret_cast<char *>(&size), sizeof(size))) {
    // valida o tamanho para evitar loop infinito em arquivos corrompidos
    if (size == 0 || size > 1000000) {
      break;
    }

    // Pula o conteúdo do registro
    binFile.seekg(size, ios::cur);
    if (binFile.fail()) {
      break;
    }

    total++;
  }

  binFile.close();
  return total;
}

bool GameReview::getReview(int index, GameReview &review) {
  // validações do índice antes de abrir o arquivo:
  if (index < 0) {
    cout << "Erro: indice inválido" << endl;
    return false;
  }
  // pra contar quantos registros existem no arquivo e verificar se o índice
  // existe
  int totalRegistros = countRecords();

  if (totalRegistros == -1) {
    cout << "Erro: arquivo reviews.bin nao encontrado" << endl;
    return false;
  }

  if (totalRegistros == 0) {
    cout << "Erro: arquivo binário está vazio" << endl;
    return false;
  }

  // Verifica se o índice existe
  if (index >= totalRegistros) {
    cout << "Erro: indice " << index << " não existe! ";
    cout << "O arquivo possui apenas " << totalRegistros << " registros ";
    cout << "(índices válidos: 0 a " << (totalRegistros - 1) << ")." << endl;
    return false;
  }
  // fim da validação do índice

  // abre o arquivo binário para buscar o registro
  string binPath = "public/reviews.bin";
  ifstream binFile(binPath, ios::binary);
  if (!binFile.is_open()) {
    cout << "Erro: Arquivo " << binPath << " nao encontrado!" << endl;
    return false;
  }

  int contador = 0;
  uint32_t size = 0;

  // percorre os registros até encontrar o índice
  while (binFile.read(reinterpret_cast<char *>(&size), sizeof(size))) {
    // verifica se o tamanho é válido
    if (size == 0 || size > 1000000) {
      cout << "Erro: Tamanho de registro inválido detectado!" << endl;
      break;
    }

    if (contador == index) {
      vector<char> buffer(size);
      // tenta ler o conteúdo
      if (binFile.read(buffer.data(), size)) {
        string linhaLida(buffer.begin(), buffer.end());
        bool sucesso = review.analisalinhaCSV(linhaLida);
        binFile.close();
        return sucesso;
      } else {
        cout << "Erro ao ler dados do registro " << index << endl;
        break;
      }
    } else {
      // passo pro próximo registro
      binFile.seekg(size, ios::cur);
      if (binFile.fail()) {
        cout << "Erro ao navegar pelo arquivo binário!" << endl;
        break;
      }
    }

    contador++;

    // verifica se já passou do índice desejado
    if (contador > index) {
      break;
    }
  }

  binFile.close();
  return false;
}

#include <algorithm> // for sort, shuffle
#include <fstream>
#include <iostream>
#include <numeric> // for iota
#include <random>  // for default_random_engine
#include <vector>

GameReview *GameReview::import(int n, const string &caminho) {
  ifstream binFile(caminho, ios::binary);
  if (!binFile) {
    cerr << "Erro ao abrir arquivo binário." << endl;
    return nullptr;
  }

  int total = GameReview::countRecords();
  if (total <= 0 || n > total) {
    cerr << "Erro: Quantidade solicitada (" << n
         << ") inválida ou maior que o total (" << total << ")." << endl;
    return nullptr;
  }

  // gera indices aleatorios
  vector<int> allIndices(total);
  iota(allIndices.begin(), allIndices.end(), 0);
  shuffle(allIndices.begin(), allIndices.end(),
          default_random_engine(time(nullptr)));

  // pegar os 'n' primeiros
  vector<int> targetIndices(allIndices.begin(), allIndices.begin() + n);
  sort(targetIndices.begin(), targetIndices.end());

  GameReview *vetor = new GameReview[n];

  int atual = 0;
  int pos = 0;       // posicao nos indices
  int vectorPos = 0; // onde salvar no vetor de retorno

  while (binFile && pos < n) {
    uint32_t size;

    if (!binFile.read(reinterpret_cast<char *>(&size), sizeof(size)))
      break;

    // Validação básica de sanidade
    if (size == 0 || size > 1000000) {
      cerr << "Registro corrompido ou inválido." << endl;
      delete[] vetor;
      return nullptr;
    }

    // Checa se o índice atual é um dos que queremos importar
    if (atual == targetIndices[pos]) {
      // É um alvo: LER os dados
      vector<char> buffer(size);
      binFile.read(buffer.data(), size);

      string linha(buffer.begin(), buffer.end());
      vetor[vectorPos].analisalinhaCSV(linha);

      pos++;
      vectorPos++;
    } else {
      // Não é um alvo: PULAR os dados (seek forward)
      binFile.seekg(size, ios::cur);
    }

    atual++;
  }

  // Embaralhar o vetor final para manter a aleatoriedade original
  shuffle(vetor, vetor + n, default_random_engine(time(nullptr)));

  return vetor;
}
