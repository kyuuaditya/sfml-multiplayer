all: compile link run

compile:
	g++ -I"C:\SFML-2.6.2\include" -I"C:\SFML-2.6.2\include" -c server.cpp -o server.o

link: 
	g++ -I"C:\SFML-2.6.2\include" .\server.o -o server.exe -L"C:\SFML-2.6.2\lib" -lmingw32 -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-main 

run:
	./server.exe