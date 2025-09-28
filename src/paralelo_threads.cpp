#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <cmath>
#include <fstream>
#include "paralelo_threads.hpp"
#include "sequencial.hpp" // Para usar a função lerMatriz

/**
 * @brief Salva o resultado parcial de uma thread em um arquivo.
 */
bool salvarResultadoParcial(const std::string& nomeArquivo, const std::pair<int, int>& dimensoes, const std::vector<double>& resultado, long long tempo) {
    std::ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo para escrita: " << nomeArquivo << std::endl;
        return false;
    }
    arquivo << dimensoes.first << " " << dimensoes.second << "\n";
    for (size_t i = 0; i < resultado.size(); ++i) {
        arquivo << resultado[i] << (i == resultado.size() - 1 ? "" : "\n");
    }
    arquivo << "\n" << tempo << "\n";
    arquivo.close();
    return true;
}

/**
 * @brief Função executada por cada thread para calcular uma porção da matriz resultado.
 */
void calcularBloco(ThreadData* data) {
    auto inicio = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < data->numElementos; ++i) {
        int indexGlobal = data->startIndex + i;
        if (indexGlobal >= data->totalElementos) break;

        int linha = indexGlobal / data->m2;
        int coluna = indexGlobal % data->m2;
        double soma = 0.0;

        for (size_t k = 0; k < data->matriz1->at(0).size(); ++k) {
            soma += (*data->matriz1)[linha][k] * (*data->matriz2)[k][coluna];
        }
        (*data->resultadoParcial)[i] = soma;
    }

    auto fim = std::chrono::high_resolution_clock::now();
    data->tempoDecorrido = std::chrono::duration_cast<std::chrono::microseconds>(fim - inicio).count();
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
    int m2 = matriz2[0].size();
    int totalElementos = n1 * m2;
    int numThreads = std::ceil((double)totalElementos / p);

    std::vector<std::thread> threads;
    std::vector<ThreadData> dados(numThreads);

    for (int i = 0; i < numThreads; ++i) {
        dados[i].startIndex = i * p;
        dados[i].numElementos = p;
        dados[i].totalElementos = totalElementos;
        dados[i].matriz1 = &matriz1;
        dados[i].matriz2 = &matriz2;
        dados[i].m2 = m2;
        
        int elementosNesteBloco = std::min(p, totalElementos - dados[i].startIndex);
        dados[i].resultadoParcial = new std::vector<double>(elementosNesteBloco);

        threads.emplace_back(calcularBloco, &dados[i]);
    }

    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
        
        std::string nomeArquivoParcial = "results/resultado_parcial_thread_" + std::to_string(i) + ".txt";
        salvarResultadoParcial(
            nomeArquivoParcial,
            std::make_pair(n1, m2),
            *dados[i].resultadoParcial,
            dados[i].tempoDecorrido
        );
        delete dados[i].resultadoParcial; // Libera a memória alocada
    }

    return 0;
}
#endif