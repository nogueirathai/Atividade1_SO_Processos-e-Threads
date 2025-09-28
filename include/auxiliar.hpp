#ifndef AUXILIAR_HPP
#define AUXILIAR_HPP

#include <vector>
#include <string>

/**
 * @brief Salva uma matriz em um arquivo de texto, com o formato:
 * linhas colunas
 * val val val ...
 */
bool salvarMatriz(const std::string& nomeArquivo, const std::vector<std::vector<double>>& matriz);

#endif // AUXILIAR_HPP