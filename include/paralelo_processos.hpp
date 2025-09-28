#ifndef PARALELO_PROCESSOS_HPP
#define PARALELO_PROCESSOS_HPP

#include <vector>
#include <string>
#include <tuple>

/**
 * @brief Salva o resultado parcial de um processo filho em um arquivo.
 *
 * Cada processo gera um arquivo contendo os elementos que calculou e seu tempo de execução.
 * O nome do arquivo será "results/resultado_processo_ID.txt".
 *
 * @param processo_id O ID do processo (um número de 0 em diante), para nomear o arquivo.
 * @param dimensoes_resultado As dimensões da matriz de resultado final (n1 x m2).
 * @param resultados_parciais Um vetor de tuplas (linha, coluna, valor) dos elementos calculados.
 * @param tempoDecorrido O tempo que o processo levou para executar seus cálculos.
 * @return true se o arquivo foi salvo com sucesso, false caso contrário.
 */
bool salvarResultadoProcesso(
    int processo_id,
    const std::pair<int, int>& dimensoes_resultado,
    const std::vector<std::tuple<int, int, double>>& resultados_parciais,
    long long tempoDecorrido
);

#endif // PARALELO_PROCESSOS_HPP