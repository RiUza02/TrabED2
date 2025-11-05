all:
	g++ main.cpp -o main.exe

run: all
	./main.exe

clean:
	del main.exe
