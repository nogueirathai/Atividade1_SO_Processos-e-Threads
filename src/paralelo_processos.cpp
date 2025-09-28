#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <cmath>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream>
#include <algorithm> // Para std::min

#include "sequencial.hpp" // Para usar a função lerMatriz

/**
 * @brief Função auxiliar para salvar o resultado parcial de um processo.
 * Usada apenas dentro deste arquivo.
 */
static bool salvarResultadoParcial(const std::string& nomeArquivo, const std::pair<int, int>& dimensoes, const std::vector<double>& resultado, long long tempo) {
    std::ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo para escrita: " << nomeArquivo << std::endl;
        return false;
    }
    arquivo << dimensoes.first << " " << dimensoes.second << "\n";
    for (size_t i = 0; i < resultado.size(); ++i) {
        arquivo << resultado[i] << "\n";
    }
    arquivo << tempo << "\n";
    arquivo.close();
    return true;
}

#ifndef TEST_BUILD
int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Uso: " << argv[0] << " <matriz1> <matriz2> <P>" << std::endl;
        return 1;
    }

    std::vector<std::vector<double>> matriz1, matriz2;
    if (!lerMatriz(argv[1], matriz1) || !lerMatriz(argv[2], matriz2)) {
        return 1;
    }

    int p = std::stoi(argv[3]);
    if (matriz1.empty() || matriz2.empty() || matriz1[0].size() != matriz2.size()) {
        std::cerr << "Erro: Dimensoes das matrizes incompativeis." << std::endl;
        return 1;
    }

    int n1 = matriz1.size();
    int m1 = matriz1[0].size();
    int m2 = matriz2[0].size();
    int totalElementos = n1 * m2;
    int numProcessos = std::ceil((double)totalElementos / p);

    for (int i = 0; i < numProcessos; ++i) {
        pid_t pid = fork();

        if (pid == -1) {
            std::cerr << "Erro: Falha ao criar processo filho (fork)." << std::endl;
            return 1;
        }

        if (pid == 0) { // Código do processo filho
            auto inicio = std::chrono::high_resolution_clock::now();
            
            int startIndex = i * p;
            int elementosNesteBloco = std::min(p, totalElementos - startIndex);
            std::vector<double> resultadoParcial;
            resultadoParcial.reserve(elementosNesteBloco);

            for (int j = 0; j < elementosNesteBloco; ++j) {
                int indexGlobal = startIndex + j;
                int linha = indexGlobal / m2;
                int coluna = indexGlobal % m2;
                double soma = 0.0;
                for (int k = 0; k < m1; ++k) {
                    soma += matriz1[linha][k] * matriz2[k][coluna];
                }
                resultadoParcial.push_back(soma);
            }

            auto fim = std::chrono::high_resolution_clock::now();
            long long tempoDecorrido = std::chrono::duration_cast<std::chrono::microseconds>(fim - inicio).count();

            std::string nomeArquivo = "results/resultado_parcial_proc_" + std::to_string(i) + ".txt";
            salvarResultadoParcial(nomeArquivo, std::make_pair(n1, m2), resultadoParcial, tempoDecorrido);
            exit(0); // Termina o processo filho
        }
    }

    // Código do processo pai: esperar todos os filhos terminarem
    for (int i = 0; i < numProcessos; ++i) {
        wait(NULL);
    }

    return 0;
}
#endif


