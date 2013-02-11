all: 	pastry

pastry:
		g++ -g -o pastry.out pastry.cpp

clean:
		rm -rf pastry.out a.out
