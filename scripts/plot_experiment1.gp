# --- Configurações do Gráfico para o Experimento 1 ---

# Define o arquivo de saída como uma imagem PNG.
set terminal pngcairo enhanced font "Verdana,10"
set output 'results/grafico_e1.png'

# Define o título do gráfico.
set title "Experimento 1: Tempo de Execução vs. Tamanho da Matriz"

# Define os rótulos dos eixos X e Y.
set xlabel "Tamanho da Matriz (N x N)"
set ylabel "Tempo Médio de Execução (segundos)"

# Define que o arquivo de dados usa vírgula como separador.
set datafile separator ","

# Adiciona uma grade ao gráfico para facilitar a leitura.
set grid

# Define a posição da legenda.
set key top left

# Comando para plotar os dados:
# 'using 1:2' significa: use a coluna 1 para o eixo X e a coluna 2 para o eixo Y.
# 'with linespoints' desenha uma linha conectando pontos em cada dado.
# 'title' define o nome que aparecerá na legenda.
# 'skip 1' pula a primeira linha do arquivo (o cabeçalho).
plot "results/experiment1_results.csv" using 1:2 with linespoints title "Sequencial", \
     "" using 1:3 with linespoints title "Paralelo (Threads)", \
     "" using 1:4 with linespoints title "Paralelo (Processos)"

# Mensagem de sucesso
print "Gráfico 'grafico_e1.png' gerado com sucesso na pasta results/."