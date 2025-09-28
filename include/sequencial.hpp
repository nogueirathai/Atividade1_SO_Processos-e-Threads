#ifndef SEQUENCIAL_HPP
#define SEQUENCIAL_HPP

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

/**
 * @brief Lê uma matriz de um arquivo. Esta função é 'inline' para ser compartilhada
 * entre múltiplos arquivos .cpp sem causar erro de linkagem.
 */
inline bool lerMatriz(const std::string& nomeArquivo, std::vector<std::vector<double>>& matriz) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo " << nomeArquivo << std::endl;
        return false;
    }
    int linhas, colunas;
    arquivo >> linhas >> colunas;
    matriz.assign(linhas, std::vector<double>(colunas));
    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            arquivo >> matriz[i][j];
        }
    }
    arquivo.close();
    return true;
}

/**
 * @brief Multiplica duas matrizes. Retorna 'void' pois o resultado é passado por referência.
 */
void multiplicarSequencial(const std::vector<std::vector<double>>& m1, const std::vector<std::vector<double>>& m2, std::vector<std::vector<double>>& resultado);

/**
 * @brief Salva a matriz resultado e o tempo de execução em um arquivo.
 */
bool salvarResultado(
    const std::vector<std::vector<double>>& matriz,
    const std::string& nomeArquivo,
    long long tempoDecorrido
);

#endif // SEQUENCIAL_HPP