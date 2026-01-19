CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Detecta o sistema operacional (Mantendo sua lógica original)
ifeq ($(OS),Windows_NT)
    TARGET = main.exe
    RM = del /Q
    RUN = main.exe
    # No Windows o comando de limpar *.o é um pouco diferente, ajustado abaixo
    CLEAN_OBJS = del /Q *.o
else
    TARGET = main
    RM = rm -f
    RUN = ./main
    CLEAN_OBJS = rm -f *.o
endif

# Lista de objetos
OBJ = main.o GameReview.o LZW.o

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

run: all
	$(RUN)

clean:
	$(RM) $(TARGET)