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

all: $(TARGET)

# 1. LINKAGEM: Junta main.o + GameReview.o + Huffman.o para criar o executável
$(TARGET): main.o GameReview.o Huffman.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o GameReview.o Huffman.o

# 2. Compila o main (cria main.o)
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

# 3. Compila o GameReview (cria GameReview.o)
GameReview.o: GameReview.cpp GameReview.h
	$(CXX) $(CXXFLAGS) -c GameReview.cpp

# 4. Compila o Huffman que está na pasta auxiliares (cria Huffman.o)
Huffman.o: auxiliares/Huffman.cpp auxiliares/Huffman.h
	$(CXX) $(CXXFLAGS) -c auxiliares/Huffman.cpp -o Huffman.o

run: all
	$(RUN)

clean:
	$(RM) $(TARGET)
	$(CLEAN_OBJS)
	$(RM) reviewsOrig.txt reviewsComp.bin reviewsDesc.txt
