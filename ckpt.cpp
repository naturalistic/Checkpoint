#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <string.h>

#include "ckpt.h"
using namespace std;
using namespace checkpoint;

namespace checkpoint {

const char* ckpt_file = "ckpt.out";

/* Add Checkpoint: creates a new checkpoint
   with a particular day of the week, time
   and name. Appends to the bottom of a text
   file that is then polled to get day and
   week ckpts. */
void ckpt::add_ckpt(string name) {
	ofstream out;
	out.open(ckpt_file, ios_base::app);
	if(!out) {
		cerr << "Problem opening file to store checkpoint!";
	}
	out << name << endl;
	return;
}

/* show Checkpoints: reads all checkpoints
   and displays them to the user */
void ckpt::show_ckpt() {
	ifstream in(ckpt_file);
	if(!in) {
		cerr << "Problem opening file to read checkpoints!";
	}
	string line;
	while(in >> line) {
		cout << line << endl;
	}
	return;
}

}
const char* usage_str = "Usage: \nadd 'name'\nls\n";
const char* add_str = "add";
const char* ls_str = "ls";

int main(int argc, char* argv[]) {
	if (argc < 2 || argc > 3) {
		cout << "Invalid number of arguments." << endl;
		cout << usage_str;
	}
	ckpt c;	

	if(strcmp(argv[1],add_str) == 0) {
		if(argc != 3) {
			cout << "Must pass name parameter" << endl;
			cout << usage_str;
			return 0;
		}
		c.add_ckpt(argv[1]);
		return 0;
	}
	if(strcmp(argv[1],ls_str) == 0) {
		if(argc != 2) {
			cout << "Unexpected parameter" << endl;
			cout << usage_str;
			return 0;
		}
		c.show_ckpt();
		return 0;
	}
	cout << "Unknown parameter " << argv[1] << endl;
	cout << usage_str;
	return 0;	
}

