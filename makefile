# Define o compilador C++
CXX := g++

# Define as flags de compilação.
# -Iinclude: Procura por arquivos de cabeçalho (.hpp) na pasta 'include'.
# -std=c++17: Usa o padrão C++17.
# -Wall: Mostra todos os warnings (boas práticas).
# -lpthread: Linka com a biblioteca Pthreads, necessária para o programa de threads.
CXXFLAGS := -Iinclude -std=c++17 -Wall -lpthread

# Define o diretório dos executáveis
BIN_DIR := bin

# Lista de todos os executáveis que queremos criar.
TARGET_AUXILIAR := $(BIN_DIR)/auxiliar
TARGET_SEQUENCIAL := $(BIN_DIR)/sequencial
TARGET_THREADS := $(BIN_DIR)/paralelo_threads
TARGET_PROCESSOS := $(BIN_DIR)/paralelo_processos
TARGET_TEST := run_tests

# Agrupa todos os executáveis principais em uma variável
EXECUTABLES := $(TARGET_AUXILIAR) $(TARGET_SEQUENCIAL) $(TARGET_THREADS) $(TARGET_PROCESSOS)

# A regra 'all' é a regra padrão, executada quando você digita apenas 'make'.
# Ela depende de todos os nossos executáveis.
all: $(EXECUTABLES)

# Regra para criar o executável 'auxiliar'
$(TARGET_AUXILIAR): src/auxiliar.cpp include/auxiliar.hpp include/sequencial.hpp
	@mkdir -p $(BIN_DIR) # Garante que o diretório bin/ exista.
	$(CXX) $(CXXFLAGS) src/auxiliar.cpp -o $@

# Regra para criar o executável 'sequencial'
$(TARGET_SEQUENCIAL): src/sequencial.cpp include/sequencial.hpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) src/sequencial.cpp -o $@

# Regra para criar o executável 'paralelo_threads'
$(TARGET_THREADS): src/paralelo_threads.cpp include/paralelo_threads.hpp include/sequencial.hpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) src/paralelo_threads.cpp -o $@

# Regra para criar o executável 'paralelo_processos'
$(TARGET_PROCESSOS): src/paralelo_processos.cpp include/paralelo_processos.hpp include/sequencial.hpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) src/paralelo_processos.cpp -o $@

# Regra para compilar e executar os testes.
# Depende dos executáveis principais para os testes de integração.
test: $(EXECUTABLES)
	@echo "--- Compilando os Testes ---"
	# Compila o test.cpp com os .cpp que contêm as funções auxiliares que ele usa diretamente.
	# Adicionamos -DTEST_BUILD para "desligar" as 'main' dos outros arquivos.
	$(CXX) $(CXXFLAGS) -DTEST_BUILD test/test.cpp src/sequencial.cpp src/auxiliar.cpp -o $(TARGET_TEST)
	@echo "--- Executando Testes de Integracao ---"
	./$(TARGET_TEST)
	@echo "--- Fim dos Testes ---"

# A regra 'clean' remove todos os arquivos gerados.
# O .PHONY garante que ela sempre execute, mesmo que exista um arquivo chamado 'clean'.
.PHONY: clean all test

clean:
	@echo "Limpando arquivos compilados e diretorios de trabalho..."
	rm -rf $(BIN_DIR) $(TARGET_TEST)
	# Apaga e recria os diretórios para garantir que fiquem vazios mas existam.
	rm -rf results data
	mkdir -p results data