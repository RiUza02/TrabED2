CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Detecta o sistema operacional
ifeq ($(OS),Windows_NT)
    TARGET = main.exe
    RM = del /Q
    RUN = main.exe
else
    TARGET = main
    RM = rm -f
    RUN = ./main
endif

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

run: all
	$(RUN)

clean:
	$(RM) $(TARGET)