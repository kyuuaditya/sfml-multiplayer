all: compile link run

compile:
	g++ -I"C:\SFML-2.6.2\include" -I"C:\SFML-2.6.2\include" -c client.cpp -o client.o

link: 
	g++ -I"C:\SFML-2.6.2\include" .\client.o -o client.exe -L"C:\SFML-2.6.2\lib" -lmingw32 -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-main 

run:
	./client.exe