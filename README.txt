## Como usar

### No Windows (VS Code)

1. Abra a pasta no VS Code
2. Compile com **Ctrl+Shift+B** (task "Compilar C++ (g++)")
3. Execute com **Ctrl+F5** — abre console externo (cmd) e executa o programa

**Ou compile manualmente no terminal:**
```cmd
g++ main.cpp -o main.exe
main.exe
```

### No Linux / WSL

**Instalar dependências (primeira vez):**
```bash
sudo apt update
sudo apt install g++ build-essential
```

**Compilar e executar:**
```bash
# Navegue até a pasta do projeto

# Usando Makefile
make run

# Ou manualmente
g++ main.cpp -o main -std=c++11 -Wall
./main
```

## Comandos do Makefile
```bash
make        # Compila o projeto
make run    # Compila e executa
make clean  # Remove arquivos gerados
```

O Makefile é **multiplataforma** e funciona automaticamente em Windows e Linux.
