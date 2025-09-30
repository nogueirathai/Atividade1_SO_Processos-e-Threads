#!/bin/bash

# --- Script Modificado para Retomar o Experimento 2 ---
# Objetivo: Ler os resultados do Experimento 1, encontrar o tamanho da matriz
#           e continuar a execução do Experimento 2 a partir de onde parou.
# CORREÇÃO: Usa uma função robusta para extrair o tempo de execução.

# --- Variáveis de Configuração ---
REPETICOES=10
ARQUIVO_SAIDA="results/experiment2_results.csv"
ARQUIVO_RESULTADOS_E1="results/experiment1_results.csv"
LIMITE_TEMPO_S=120 # 2 minutos em segundos

# --- Função Auxiliar Robusta para Obter o Tempo ---
# Esta função itera sobre os arquivos de resultado parcial e encontra o maior
# tempo de execução, que corresponde ao tempo total da etapa paralela.
get_parallel_time() {
    local base_name="$1"
    local max_time=0
    # Itera sobre todos os arquivos que correspondem ao padrão (ex: resultado_parcial_thread_*.txt)
    for file in results/${base_name}*.txt; do
        # Verifica se o arquivo realmente existe
        if [ -f "$file" ]; then
            # Pega a última linha do arquivo, que contém o tempo
            local current_time=$(tail -n 1 "$file")
            # Compara com o tempo máximo encontrado até agora
            if (( current_time > max_time )); then
                max_time=$current_time
            fi
        fi
    done
    echo $max_time
}


# --- Script Principal ---

if [ ! -f "$ARQUIVO_RESULTADOS_E1" ]; then
    echo "Erro: O arquivo '$ARQUIVO_RESULTADOS_E1' não foi encontrado."
    exit 1
fi

echo "Analisando resultados do Experimento 1 para encontrar o tamanho da matriz..."
TAMANHO_MATRIZ=$(awk -F',' -v limite="$LIMITE_TEMPO_S" 'NR>1 && $2 > limite {print $1; exit}' "$ARQUIVO_RESULTADOS_E1")

if [ -z "$TAMANHO_MATRIZ" ]; then
    echo "Erro: Nenhum teste no Experimento 1 atingiu o tempo de ${LIMITE_TEMPO_S}s."
    exit 1
fi

echo "Tamanho de matriz encontrado: ${TAMANHO_MATRIZ}x${TAMANHO_MATRIZ}"
TOTAL_ELEMENTOS=$((TAMANHO_MATRIZ * TAMANHO_MATRIZ))

# A lista de divisores foi encurtada para começar a partir da 4ª iteração (divisor 12).
DIVISORES=(2)

mkdir -p results data

# Não apaga o arquivo de saída para continuar de onde parou.
# rm -f "$ARQUIVO_SAIDA"

if [ ! -f "$ARQUIVO_SAIDA" ]; then
    echo "P_Value,Avg_Time_Threads_Seconds,Avg_Time_Procs_Seconds" > "$ARQUIVO_SAIDA"
fi

echo "Gerando matrizes de teste..."
./bin/auxiliar $TAMANHO_MATRIZ $TAMANHO_MATRIZ $TAMANHO_MATRIZ $TAMANHO_MATRIZ > /dev/null

for divisor in "${DIVISORES[@]}"; do
    P_VALUE=$((TOTAL_ELEMENTOS / divisor))
    echo "----------------------------------------------------"
    echo "Testando com P = $P_VALUE (aprox. N*N/${divisor})..."

    total_time_threads=0
    total_time_procs=0

    for i in $(seq 1 $REPETICOES); do
        echo "  Repeticao $i/$REPETICOES..."
        
        # Threads
        rm -f results/resultado_parcial_thread_*.txt
        ./bin/paralelo_threads data/matriz1.txt data/matriz2.txt $P_VALUE > /dev/null
        time_threads=$(get_parallel_time "resultado_parcial_thread_")
        total_time_threads=$((total_time_threads + time_threads))

        # Processos
        rm -f results/resultado_parcial_proc_*.txt
        ./bin/paralelo_processos data/matriz1.txt data/matriz2.txt $P_VALUE > /dev/null
        time_procs=$(get_parallel_time "resultado_parcial_proc_")
        total_time_procs=$((total_time_procs + time_procs))
    done

    avg_time_threads=$((total_time_threads / REPETICOES))
    avg_time_procs=$((total_time_procs / REPETICOES))
    
    avg_time_threads_sec=$(echo "scale=6; $avg_time_threads / 1000000" | bc)
    avg_time_procs_sec=$(echo "scale=6; $avg_time_procs / 1000000" | bc)

    echo "  -> Tempo medio (Threads):    ${avg_time_threads_sec}s"
    echo "  -> Tempo medio (Processos):  ${avg_time_procs_sec}s"
    
    echo "$P_VALUE,$avg_time_threads_sec,$avg_time_procs_sec" >> "$ARQUIVO_SAIDA"
done

echo "----------------------------------------------------"
echo "Experimento 2 concluído. Resultados em: $ARQUIVO_SAIDA"
rm -f data/matriz1.txt data/matriz2.txt