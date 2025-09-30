# --- Configurações do Gráfico para o Experimento 2 ---

# Define o arquivo de saída como uma imagem PNG.
set terminal pngcairo enhanced font "Verdana,10"
set output 'results/grafico_e2.png'

# Define o título do gráfico.
set title "Experimento 2: Tempo de Execução vs. Valor de P"

# Define os rótulos dos eixos X e Y.
set xlabel "Valor de P (Granularidade da Tarefa)"
set ylabel "Tempo Médio de Execução (segundos)"

# Define que o arquivo de dados usa vírgula como separador.
set datafile separator ","

# Adiciona uma grade.
set grid

# Define a posição da legenda.
set key top right

# Comando para plotar os dados do experimento 2.
# 'skip 1' pula a primeira linha do arquivo (o cabeçalho).
plot "results/experiment2_results.csv" using 1:2 with linespoints title "Paralelo (Threads)", \
     "" using 1:3 with linespoints title "Paralelo (Processos)"

# Mensagem de sucesso
print "Gráfico 'grafico_e2.png' gerado com sucesso na pasta results/."
```

### Como Gerar os Gráficos

1.  **Verifique a Instalação:** Confirme se você instalou o Gnuplot com `sudo apt-get install -y gnuplot`.
2.  **Execute os Comandos:** Após a conclusão dos experimentos, execute os seguintes comandos no seu terminal, um de cada vez:
    ```bash
    # Para gerar o gráfico do Experimento 1
    gnuplot scripts/plot_experiment1.gp

    # Para gerar o gráfico do Experimento 2
    gnuplot scripts/plot_experiment2.gp
