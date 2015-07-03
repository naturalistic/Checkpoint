#include <iostream>
#include <fstream>
#include <string>
using namespace std;

namespace checkpoint {

class ckpt {
	public:
		// adds a new chkpts
		static void add_ckpt(string name, int dow, int hour, int minute);

		// Shows all chpts for a particular day of the week
		// not passing a day prints the current days checkpoints
		static void show_ckpt(int day_filter); 

		// marks a ckpt complete for a particular date and time
		static void complete_ckpt(string name);

		// prints out percentage ckpt completion
		static void stat(int day, int month, int year);
};

}

