#include<iostream>

#include "md5.h"
#include<string>


using namespace std;

int 
main(int argc, const char *argv[]){
	string a;
	std::cin >> a;
	std::cout << md5(a).substr(0,8) << "\n";
	return 0;
}
