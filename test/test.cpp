#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <cmath>
#include <algorithm>

// Inclui os cabeçalhos das funções que vamos testar ou usar.
#include "auxiliar.hpp"
#include "sequencial.hpp"

// --- Funções Auxiliares para os Testes ---

// Função para verificar se duas matrizes são aproximadamente iguais.
bool matrizesSaoIguais(const std::vector<std::vector<double>>& m1, const std::vector<std::vector<double>>& m2, double tolerancia = 1e-9) {
    if (m1.size() != m2.size() || (m1.empty() && !m2.empty()) || (!m1.empty() && m2.empty())) return false;
    if (m1.empty() && m2.empty()) return true;
    if (m1[0].size() != m2[0].size()) return false;
    for (size_t i = 0; i < m1.size(); ++i) {
        for (size_t j = 0; j < m1[i].size(); ++j) {
            if (std::abs(m1[i][j] - m2[i][j]) > tolerancia) return false;
        }
    }
    return true;
}

// Função para juntar os resultados parciais dos arquivos paralelos.
bool juntarResultadosParciais(int numArquivos, const std::string& baseNome, int linhas, int colunas, int p, std::vector<std::vector<double>>& resultadoFinal) {
    resultadoFinal.assign(linhas, std::vector<double>(colunas, 0.0));
    int elementosLidos = 0;
    int totalElementos = linhas * colunas;

    for (int i = 0; i < numArquivos; ++i) {
        std::string nomeArquivo = "results/" + baseNome + std::to_string(i) + ".txt";
        std::ifstream arquivo(nomeArquivo);
        if (!arquivo.is_open()) {
            std::cerr << "Falha ao abrir o arquivo de resultado parcial: " << nomeArquivo << std::endl;
            return false;
        }

        int tempLinhas, tempColunas;
        arquivo >> tempLinhas >> tempColunas;

        int startIndex = i * p;
        int elementosEsperadosNesteBloco = std::min(p, totalElementos - startIndex);

        for (int j = 0; j < elementosEsperadosNesteBloco; ++j) {
            double valor;
            if (!(arquivo >> valor)) {
                std::cerr << "Erro de leitura no arquivo " << nomeArquivo << std::endl;
                return false;
            }

            if (elementosLidos < totalElementos) {
                int linhaAtual = elementosLidos / colunas;
                int colunaAtual = elementosLidos % colunas;
                resultadoFinal[linhaAtual][colunaAtual] = valor;
                elementosLidos++;
            }
        }
    }
    return elementosLidos == totalElementos;
}

// --- Bateria de Testes ---

void teste_logica_sequencial() {
    std::cout << "TESTE 1: Logica da multiplicacao sequencial ... ";
    std::vector<std::vector<double>> a = {{1, 2}, {3, 4}};
    std::vector<std::vector<double>> b = {{5, 6}, {7, 8}};
    std::vector<std::vector<double>> esperado = {{19, 22}, {43, 50}};
    std::vector<std::vector<double>> resultado;
    multiplicarSequencial(a, b, resultado);
    if (matrizesSaoIguais(esperado, resultado)) {
        std::cout << "[PASSOU]" << std::endl;
    } else {
        std::cout << "[FALHOU]" << std::endl;
        exit(1);
    }
}

void teste_integrado_paralelo(const std::string& nomeTeste, const std::string& executavel, const std::string& baseArquivoRes) {
    std::cout << nomeTeste;
    std::vector<std::vector<double>> a = {{1, 2, 3}, {4, 5, 6}};
    std::vector<std::vector<double>> b = {{7, 8}, {9, 10}, {11, 12}};
    std::vector<std::vector<double>> esperado = {{58, 64}, {139, 154}};

    salvarMatriz("data/test_mat1.txt", a);
    salvarMatriz("data/test_mat2.txt", b);

    int p = 3;
    std::string comando = "./" + executavel + " data/test_mat1.txt data/test_mat2.txt " + std::to_string(p) + " > /dev/null 2>&1";
    int ret = system(comando.c_str());

    if (ret != 0) {
        std::cout << "[FALHOU - Erro na execucao do programa]" << std::endl;
        exit(1);
    }

    std::vector<std::vector<double>> resultado;
    int totalElementos = esperado.size() * esperado[0].size();
    int numArquivos = (totalElementos + p - 1) / p;

    if (juntarResultadosParciais(numArquivos, baseArquivoRes, esperado.size(), esperado[0].size(), p, resultado)) {
        if (matrizesSaoIguais(esperado, resultado)) {
            std::cout << "[PASSOU]" << std::endl;
        } else {
            std::cout << "[FALHOU - Resultado da matriz incorreto]" << std::endl;
            exit(1);
        }
    } else {
        std::cout << "[FALHOU - Erro ao juntar arquivos de resultado]" << std::endl;
        exit(1);
    }
}

int main() {
    std::cout << "--- Iniciando Bateria de Testes ---" << std::endl;

    teste_logica_sequencial();
    
    teste_integrado_paralelo("TESTE 2: Integracao Paralelo com Threads ... ", "bin/paralelo_threads", "resultado_parcial_thread_");
    
    // CORREÇÃO: Alterado de "processo" para "proc" para corresponder ao nome do arquivo gerado.
    teste_integrado_paralelo("TESTE 3: Integracao Paralelo com Processos ...", "bin/paralelo_processos", "resultado_parcial_proc_");

    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Resultado Final: 3 de 3 testes passaram." << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    return 0;
}