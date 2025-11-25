# HashingTrab - Projeto de Hashing em C++

## Descrição
Implementação de tabelas hash em C++ com dois métodos de resolução de colisões:
- **Hash1**: Sondagem Dupla (Double Hashing)
- **Hash2**: Sondagem Linear (Linear Probing)

## Estrutura do Projeto
```
HashingTrab/
├── main.cpp           : Programa principal com menu interativo
├── Hash1.cpp          : Implementação da tabela hash com sondagem dupla
├── Hash2.cpp          : Implementação da tabela hash com sondagem linear
├── Makefile           : Comandos de compilação multiplataforma
├── .vscode/           : Configurações do VS Code (tasks e launch)
└── README.md          : Este arquivo
```

## Como usar

### No Windows (VS Code)

1. Abra a pasta `HashingTrab` no VS Code
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
cd /mnt/c/Users/SeuUsuario/HashingTrab

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
