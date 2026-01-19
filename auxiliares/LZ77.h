#ifndef LZ77_H
#define LZ77_H

#include <string>

struct LZ77Token {
    int offset;
    int length;
    char next;
};

class LZ77 {
public:
    void comprimeArquivo(const std::string& arqOrigem, const std::string& arqSaida);
    void descomprimeArquivo(const std::string& arqOrigem, const std::string& arqSaida);

private:
    std::string comprimeLZ77(const std::string& input);
    std::string descomprimeLZ77(const std::string& input);
};

#endif // LZ77_H