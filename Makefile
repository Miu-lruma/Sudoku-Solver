alias gfx="g++ -std=c++14 -lX11 gfx.o"

sudoku: sudoku.o gfx.o
	g++ -std=c++14 -lX11 sudoku.o gfx.o -o sudoku

sudoku.o: sudoku.cpp 
	g++ -c -std=c++14 -lX11 sudoku.cpp -o sudoku.o

clean:
	rm sudoku.o sudoku
