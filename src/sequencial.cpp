#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include "sequencial.hpp" // Inclusão do nosso arquivo de cabeçalho atualizado

/**
 * Implementação da função de multiplicação sequencial.
 * Corresponde à declaração 'void' no arquivo .hpp.
 */
void multiplicarSequencial(
    const std::vector<std::vector<double>>& matriz1,
    const std::vector<std::vector<double>>& matriz2,
    std::vector<std::vector<double>>& resultado
) {
    int n1 = matriz1.size();
    int m1 = matriz1[0].size();
    int m2 = matriz2[0].size();

    resultado.assign(n1, std::vector<double>(m2, 0.0));

    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < m2; ++j) {
            for (int k = 0; k < m1; ++k) {
                resultado[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }
}

/**
 * Implementação da função para salvar o resultado e o tempo.
 * Corresponde à declaração no arquivo .hpp.
 */
bool salvarResultado(
    const std::vector<std::vector<double>>& matriz,
    const std::string& nomeArquivo,
    long long tempoDecorrido
) {
    std::ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo para escrita: " << nomeArquivo << std::endl;
        return false;
    }

    if (matriz.empty()) {
        arquivo << "0 0\n";
    } else {
        int linhas = matriz.size();
        int colunas = matriz[0].size();
        arquivo << linhas << " " << colunas << "\n";
        for (int i = 0; i < linhas; ++i) {
            for (int j = 0; j < colunas; ++j) {
                arquivo << matriz[i][j] << (j == colunas - 1 ? "" : " ");
            }
            arquivo << "\n";
        }
    }

    arquivo << tempoDecorrido << "\n";
    arquivo.close();
    return true;
}

/**
 * Função principal do executável 'sequencial'.
 * NOTA: A implementação de 'lerMatriz' foi removida deste arquivo
 * porque agora ela reside no 'sequencial.hpp' como uma função inline.
 */
#ifndef TEST_BUILD // Esta flag previne que a 'main' seja compilada durante os testes
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_matriz1> <arquivo_matriz2>" << std::endl;
        return 1;
    }

    std::vector<std::vector<double>> matriz1, matriz2;

    if (!lerMatriz(argv[1], matriz1) || !lerMatriz(argv[2], matriz2)) {
        return 1;
    }

    if (matriz1.empty() || matriz2.empty() || matriz1[0].size() != matriz2.size()) {
        std::cerr << "Erro: Dimensoes das matrizes sao incompativeis para multiplicacao." << std::endl;
        return 1;
    }

    std::vector<std::vector<double>> resultado;
    
    auto inicio = std::chrono::high_resolution_clock::now();
    multiplicarSequencial(matriz1, matriz2, resultado);
    auto fim = std::chrono::high_resolution_clock::now();

    auto duracao = std::chrono::duration_cast<std::chrono::microseconds>(fim - inicio).count();
    
    const std::string arquivoResultado = "results/resultado_sequencial.txt";
    if (!salvarResultado(resultado, arquivoResultado, duracao)) {
        return 1;
    }

    std::cout << "Programa sequencial finalizado com sucesso. Resultado em " << arquivoResultado << std::endl;
    return 0;
}
#endif // TEST_BUILD

