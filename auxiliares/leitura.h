#ifndef LEITURA_H
#define LEITURA_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

/*
===========================================================
  Estrutura: Recomendacao
  Objetivo: Representar uma linha válida do arquivo CSV com:
            - ID da recomendação
            - ID do aplicativo
            - ID do autor (SteamID)
            - Pontuação ponderada
===========================================================
*/
struct Recomendacao
{
    std::string idRecomendacao;
    std::string idAplicativo;
    std::string idAutor;
    std::string pontuacao;
};

/*
===========================================================
  Função auxiliar: dividirLinhaCSV
  Objetivo: Separar corretamente os campos de uma linha CSV,
            considerando aspas e vírgulas internas.
===========================================================
*/
static inline std::vector<std::string> dividirLinhaCSV(const std::string &linha)
{
    std::vector<std::string> campos;
    std::string campoAtual;
    bool dentroDeAspas = false;

    for (size_t i = 0; i < linha.size(); ++i)
    {
        char c = linha[i];

        if (c == '"')
        {
            // Trata aspas duplas ("") dentro de um campo
            if (dentroDeAspas && i + 1 < linha.size() && linha[i + 1] == '"')
            {
                campoAtual.push_back('"');
                ++i;
            }
            else
            {
                dentroDeAspas = !dentroDeAspas;
            }
        }
        else if (c == ',' && !dentroDeAspas)
        {
            campos.push_back(campoAtual);
            campoAtual.clear();
        }
        else
        {
            campoAtual.push_back(c);
        }
    }

    campos.push_back(campoAtual);
    return campos;
}

/*
===========================================================
  Função auxiliar: removerEspacos
  Objetivo: Remove espaços, tabulações e quebras de linha
            do início e do final de uma string.
===========================================================
*/
static inline std::string removerEspacos(const std::string &texto)
{
    const char *espacos = " \t\n\r\f\v";
    size_t inicio = texto.find_first_not_of(espacos);
    if (inicio == std::string::npos)
        return "";

    size_t fim = texto.find_last_not_of(espacos);
    return texto.substr(inicio, fim - inicio + 1);
}

/*
===========================================================
  Função: lerCSVRecomendacoes
  Objetivo:
    Ler um arquivo CSV contendo exatamente quatro colunas:
      1) recommendationid
      2) appid
      3) author_steamid
      4) weighted_vote_score

  Detalhes:
    - Linhas com mais ou menos que 4 campos são ignoradas.
    - Pode pular o cabeçalho, se especificado.
    - Retorna um vetor com todas as recomendações válidas.
===========================================================
*/
static inline std::vector<Recomendacao> lerCSVRecomendacoes(const std::string &caminhoArquivo, bool possuiCabecalho = true)
{
    std::vector<Recomendacao> recomendacoes;
    std::ifstream arquivo(caminhoArquivo);

    if (!arquivo.is_open())
    {
        std::cerr << "Erro: não foi possível abrir o arquivo '" << caminhoArquivo << "'." << std::endl;
        return recomendacoes;
    }

    std::string linha;

    // Ignora a primeira linha se for cabeçalho
    if (possuiCabecalho)
        std::getline(arquivo, linha);

    while (std::getline(arquivo, linha))
    {
        // Remove '\r' de arquivos Windows
        if (!linha.empty() && linha.back() == '\r')
            linha.pop_back();

        auto campos = dividirLinhaCSV(linha);

        // Ignora linhas com número incorreto de campos
        if (campos.size() != 4)
            continue;

        Recomendacao rec;
        rec.idRecomendacao = removerEspacos(campos[0]);
        rec.idAplicativo = removerEspacos(campos[1]);
        rec.idAutor = removerEspacos(campos[2]);
        rec.pontuacao = removerEspacos(campos[3]);

        recomendacoes.push_back(rec);
    }

    return recomendacoes;
}

#endif // LEITURA_H