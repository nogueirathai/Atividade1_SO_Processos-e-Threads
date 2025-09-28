#ifndef PARALELO_THREADS_HPP
#define PARALELO_THREADS_HPP

#include <vector>
#include <string>

/**
 * @struct ThreadData
 * @brief Agrupa todos os dados necessários para uma thread realizar seu trabalho.
 * * Esta estrutura é passada como um ponteiro para cada thread. Ela contém ponteiros
 * para as matrizes originais (para evitar cópias pesadas), informações sobre
 * qual "fatia" do trabalho a thread deve fazer, e um local para armazenar
 * seu resultado e tempo de execução.
 */
struct ThreadData {
    int startIndex;      // Índice do primeiro elemento a ser calculado por esta thread.
    int numElementos;    // Número de elementos que esta thread deve calcular (o valor de P).
    int totalElementos;  // Total de elementos na matriz resultado.
    int m2;              // Número de colunas da segunda matriz (necessário para calcular índices).
    
    // Ponteiros para as matrizes de entrada para evitar cópias de dados.
    const std::vector<std::vector<double>>* matriz1;
    const std::vector<std::vector<double>>* matriz2;
    
    // Ponteiro para um vetor onde esta thread armazenará seus resultados.
    std::vector<double>* resultadoParcial;
    // Variável para armazenar o tempo de execução individual da thread.
    long long tempoDecorrido;
};

/**
 * @brief Salva o resultado parcial de uma thread em um arquivo de texto.
 */
bool salvarResultadoParcial(const std::string& nomeArquivo, const std::pair<int, int>& dimensoes, const std::vector<double>& resultado, long long tempo);

#endif // PARALELO_THREADS_HPP