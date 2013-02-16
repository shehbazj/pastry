all: 	pastry 

pastry: 
		g++ -g -o pastry.out pastry.cpp md5.cpp

clean:
		rm -rf pastry.out a.out
