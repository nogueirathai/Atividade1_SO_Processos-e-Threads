#include "auxiliar.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>

/**
 * Implementação da função para salvar uma matriz simples.
 */
bool salvarMatriz(const std::string& nomeArquivo, const std::vector<std::vector<double>>& matriz) {
    std::ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro: Nao foi possivel criar o arquivo " << nomeArquivo << std::endl;
        return false;
    }

    if (matriz.empty()) {
        arquivo << "0 0" << std::endl;
    } else {
        int linhas = matriz.size();
        int colunas = matriz[0].size();
        arquivo << linhas << " " << colunas << std::endl;
        for (int i = 0; i < linhas; ++i) {
            for (int j = 0; j < colunas; ++j) {
                arquivo << matriz[i][j] << (j == colunas - 1 ? "" : " ");
            }
            arquivo << std::endl;
        }
    }
    arquivo.close();
    return true;
}

#ifndef TEST_BUILD
int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Uso: " << argv[0] << " <n1> <m1> <n2> <m2>" << std::endl;
        return 1;
    }

    int n1 = std::stoi(argv[1]);
    int m1 = std::stoi(argv[2]);
    int n2 = std::stoi(argv[3]);
    int m2 = std::stoi(argv[4]);

    if (m1 != n2) {
        std::cerr << "Erro: O numero de colunas da Matriz 1 deve ser igual ao numero de linhas da Matriz 2." << std::endl;
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(1.0, 100.0);

    std::vector<std::vector<double>> matriz1(n1, std::vector<double>(m1));
    for (int i = 0; i < n1; ++i) for (int j = 0; j < m1; ++j) matriz1[i][j] = dis(gen);

    std::vector<std::vector<double>> matriz2(n2, std::vector<double>(m2));
    for (int i = 0; i < n2; ++i) for (int j = 0; j < m2; ++j) matriz2[i][j] = dis(gen);
    
    if (!salvarMatriz("data/matriz1.txt", matriz1)) return 1;
    std::cout << "Matriz 1 salva em data/matriz1.txt" << std::endl;

    if (!salvarMatriz("data/matriz2.txt", matriz2)) return 1;
    std::cout << "Matriz 2 salva em data/matriz2.txt" << std::endl;

    return 0;
}
#endif // TEST_BUILD

