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

$(TARGET): main.o GameReview.o Huffman.o LZ77.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o GameReview.o Huffman.o LZ77.o

# 2. Regra para o main.o (mantenha igual)
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

# 3. Regra para o GameReview.o (mantenha igual)
GameReview.o: GameReview.cpp GameReview.h
	$(CXX) $(CXXFLAGS) -c GameReview.cpp

# 4. Regra para o Huffman.o (mantenha igual)
Huffman.o: auxiliares/Huffman.cpp auxiliares/Huffman.h
	$(CXX) $(CXXFLAGS) -c auxiliares/Huffman.cpp -o Huffman.o

# 5. NOVA REGRA: Compilar o LZ77
LZ77.o: auxiliares/LZ77.cpp auxiliares/LZ77.h
	$(CXX) $(CXXFLAGS) -c auxiliares/LZ77.cpp -o LZ77.o

run: all
	$(RUN)

clean:
	$(RM) $(TARGET)
	$(CLEAN_OBJS)
	$(RM) reviewsOrig.txt reviewsComp.bin reviewsDesc.txt
