all: compile link

compile:
	g++ -c main.cpp image_manager.cpp cell.cpp board.cpp icons.cpp -I"SFML-2.5.1\include" -DSFML_STATIC

link:
	g++ main.o image_manager.o cell.o board.o icons.o -o main -L"SFML-2.5.1\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows -lsfml-main

clean:
	rm -f main *.o 