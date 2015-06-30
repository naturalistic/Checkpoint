#include <iostream>
#include <fstream>
#include <string>
using namespace std;

namespace checkpoint {

class ckpt {
	public:
		static void add_ckpt(string name);
		static void show_ckpt(); 
};

}

