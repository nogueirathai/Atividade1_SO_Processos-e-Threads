#!/bin/bash

# --- Configurações do Experimento E1 ---
# Objetivo: Comparar os tempos de execução (Sequencial, Threads, Processos)
#           aumentando o tamanho da matriz até que o sequencial demore
#           pelo menos 2 minutos (120,000,000 microssegundos).

# Executáveis
AUXILIAR="./bin/auxiliar"
SEQUENCIAL="./bin/sequencial"
THREADS="./bin/paralelo_threads"
PROCESSOS="./bin/paralelo_processos"

# Parâmetros
REPETICOES=10
TAMANHO_INICIAL=100
LIMITE_TEMPO_SEQUENCIAL_US=120000000 # 120 segundos em microssegundos
ARQUIVO_SAIDA="results/experiment1_results.csv"

# --- Funções Auxiliares ---

# Função para extrair o tempo do arquivo de resultado sequencial
get_sequential_time() {
    tail -n 1 "results/resultado_sequencial.txt"
}

# Função para extrair o MAIOR tempo de todos os arquivos de resultado parciais
# Isso é crucial, pois o tempo total de uma execução paralela é determinado pela
# thread/processo mais lento.
get_parallel_time() {
    local base_name="$1"
    local max_time=0

    # Itera sobre todos os arquivos que correspondem ao padrão (ex: resultado_parcial_thread_*.txt)
    for file in results/${base_name}*.txt; do
        # Pega o tempo (última linha) do arquivo atual
        local current_time=$(tail -n 1 "$file")
        # Compara com o máximo encontrado até agora
        if (( current_time > max_time )); then
            max_time=$current_time
        fi
    done
    echo $max_time
}

# --- Script Principal ---

# Garante que os diretórios existam
mkdir -p data results

# Remove o arquivo de saída antigo, se existir, e escreve o cabeçalho do CSV
rm -f "$ARQUIVO_SAIDA"
echo "tamanho_matriz,tempo_sequencial_medio_s,tempo_threads_medio_s,tempo_processos_medio_s" > "$ARQUIVO_SAIDA"

echo "Iniciando Experimento 1..."
echo "Os resultados serao salvos em: $ARQUIVO_SAIDA"
echo "----------------------------------------------------"

TAMANHO_ATUAL=$TAMANHO_INICIAL

while true; do
    echo "Testando com matrizes de tamanho ${TAMANHO_ATUAL}x${TAMANHO_ATUAL}..."

    # Zera os acumuladores de tempo para as 10 repetições
    total_time_seq=0
    total_time_threads=0
    total_time_procs=0

    # Calcula o valor de P para esta dimensão de matriz
    # P = ceil((N*N)/8). Em aritmética inteira, isso é (N*N + 7) / 8.
    P=$(( (TAMANHO_ATUAL * TAMANHO_ATUAL + 7) / 8 ))

    for i in $(seq 1 $REPETICOES); do
        echo "  Repeticao $i/$REPETICOES..."

        # Limpa os resultados da execução anterior
        rm -f results/*.txt

        # 1. Gera as matrizes
        $AUXILIAR $TAMANHO_ATUAL $TAMANHO_ATUAL $TAMANHO_ATUAL $TAMANHO_ATUAL > /dev/null

        # 2. Executa e cronometra a versão Sequencial
        $SEQUENCIAL data/matriz1.txt data/matriz2.txt > /dev/null
        time_seq=$(get_sequential_time)
        total_time_seq=$((total_time_seq + time_seq))

        # 3. Executa e cronometra a versão com Threads
        $THREADS data/matriz1.txt data/matriz2.txt $P > /dev/null
        time_threads=$(get_parallel_time "resultado_parcial_thread_")
        total_time_threads=$((total_time_threads + time_threads))

        # 4. Executa e cronometra a versão com Processos
        $PROCESSOS data/matriz1.txt data/matriz2.txt $P > /dev/null
        time_procs=$(get_parallel_time "resultado_parcial_proc_")
        total_time_procs=$((total_time_procs + time_procs))
    done

    # Calcula a média dos tempos (em microssegundos)
    avg_time_seq=$((total_time_seq / REPETICOES))
    avg_time_threads=$((total_time_threads / REPETICOES))
    avg_time_procs=$((total_time_procs / REPETICOES))

    # Converte as médias para segundos (ponto flutuante) para o CSV
    avg_time_seq_s=$(echo "scale=6; $avg_time_seq / 1000000" | bc)
    avg_time_threads_s=$(echo "scale=6; $avg_time_threads / 1000000" | bc)
    avg_time_procs_s=$(echo "scale=6; $avg_time_procs / 1000000" | bc)

    # Salva os resultados no arquivo CSV
    echo "$TAMANHO_ATUAL,$avg_time_seq_s,$avg_time_threads_s,$avg_time_procs_s" >> "$ARQUIVO_SAIDA"

    echo "  -> Tempo medio (Sequencial): ${avg_time_seq_s}s"
    echo "  -> Tempo medio (Threads):    ${avg_time_threads_s}s"
    echo "  -> Tempo medio (Processos):  ${avg_time_procs_s}s"
    echo "----------------------------------------------------"

    # Condição de parada: se o tempo do sequencial passou do limite
    if (( avg_time_seq > LIMITE_TEMPO_SEQUENCIAL_US )); then
        echo "Tempo sequencial excedeu o limite de 120s. Experimento 1 concluido."
        # Salva o tamanho da matriz para usar no próximo experimento
        echo $TAMANHO_ATUAL > results/tamanho_matriz_e2.txt
        break
    fi

    # Dobra o tamanho da matriz para a próxima iteração
    TAMANHO_ATUAL=$((TAMANHO_ATUAL * 2))
done

echo "Experimento 1 finalizado!"