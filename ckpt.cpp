#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdexcept>
#include <time.h>

#include "ckpt.h"
using namespace std;
using namespace checkpoint;

namespace checkpoint {

const char* ckpt_file = "./ckpt.out"; // contains recurring ckpts
const char* log_file = "./ckptlog.out"; // contains log of completions

/* Add Checkpoint: creates a new checkpoint
   with a particular day of the week, time
   and name. Appends to the bottom of a text
   file that is then polled to get day and
   week ckpts. */
// mo tu we th fr sa su 
void ckpt::add_ckpt(string name, int dow, int hour, int minute) {
	ofstream out;
	out.open(ckpt_file, ios_base::app);
	if(!out) {
		cerr << "Problem opening file to store checkpoint!";
	}
	out << name << " " << dow << " " << hour << " " << minute << endl;
	out.close();
	return;
}

/* show Checkpoints: reads all checkpoints
   and displays them to the user*/
void ckpt::show_ckpt(int day_filter) {

	if(day_filter == 0) {
		time_t theTime = time(NULL);
		struct tm *aTime = localtime(&theTime);
		day_filter = aTime->tm_wday -1; // mo 1 to su 7
	}
	
	ifstream in(ckpt_file);
	if(!in) {
		cerr << "Problem opening file to read checkpoints!";
	}
	string name;
	int day;
	int hour;
	int minute; 
	while(in >> name >> day >> hour >> minute) {
		if(day_filter == -1 || day == day_filter) {
		cout << "Title: " << name << " DOW: " << day << " Time: " << hour << ":" << minute << endl;
		}
	}
	in.close();
	return;
}

void ckpt::complete_ckpt(string name) {

	time_t theTime = time(NULL);
	struct tm *aTime = localtime(&theTime);
	
	int min = aTime->tm_min;
	int hour = aTime->tm_hour;
	int day = aTime->tm_mday;
	int month = aTime->tm_mon + 1;
	int year = aTime->tm_year + 1900;
	int dow = aTime->tm_wday-1; // mo 1 to su 7

	ifstream in(ckpt_file);
	if(!in) {
		cerr << "Problem opening file to read checkpoints!";
	}
	string cname;
	int cdow;
	int chour;
	int cmin;

	while(in >> cname >> cdow >> chour >> cmin) {
		if(cname != name || cdow != dow || chour < hour || (chour == hour && cmin < min)) {
			continue;
		} 		
		ofstream out;
		out.open(log_file, ios_base::app);
		if(!out) {
			cerr << "Problem opening file to log completion";
		}
		out << name << " " << dow << " " << day << " " << month << " " << year << endl;
		out.close();
		return;
	}		
	cout << "Couldn't find checkpoint with name: '" << name << endl;
}

void ckpt::stat(int day, int month, int year) {
	ifstream in(log_file);
	if(!in) {
		cerr << "Problem opening log file";
	}
	string cname;
	int cdow;
	int cday;
	int cmonth;
	int cyear;
	
	int completed_count = 0; 
	while(in >> cname >> cdow >> cday >> cmonth >> cyear) {	
		if(cday == day && cmonth == month && cyear == year) {
			++completed_count;	
		} 	
	}
	
	in.close();
	int chour;
	int cmin;
	int dow = cdow; // store from prev. as avoid calc. dow
	int todo_count = 0;
	in.open(ckpt_file);
	if(!in) {
		cerr << "Problem opening ckpt file";
	}
	while(in >> cname >> cdow >> chour >> cmin) {
		if(cdow == dow) {
			++todo_count;
		}	
	}
	cout << "Completed: " << completed_count << "/" << todo_count << endl;
}


}


const char* usage_str = "Usage: \n\tadd 'name' 'day as no. e.g 1, 2' and time as hour then minute e.g 4 23\n\tls\n";
const char* add_str = "add";
const char* ls_str = "ls";
const char* log_str = "log";
const char* stat_str = "stat";

int main(int argc, char* argv[]) {
	if(argc < 2) {
		cout << "Must pass a parameter" << endl;
		cout << usage_str;
		return 0;
	}
	ckpt c;	

	if(strcmp(argv[1],add_str) == 0) {
		if(argc != 6) {
			cout << "Must pass name, day, hour and minute parameters" << endl;
			cout << usage_str;
			return 0;
		}
		try {
			int day = std::stoi(argv[3]);
			int hour = std::stoi(argv[4]);
			int minute = std::stoi(argv[5]);
			if(day > -2 && day < 8 && hour >= 0 && hour < 24 
				&& minute >= 0 && minute < 60) {
				c.add_ckpt(argv[2], day, hour, minute);
				return 0;
			}
		} catch (invalid_argument & ex) {
			// Do nothing
		}
		cout << "Invalid parameters." << endl;
		cout << usage_str;	
		return 0;
	}
	if(strcmp(argv[1],ls_str) == 0) {
		if(argc == 2) {
			c.show_ckpt(0);
		} else if (argc == 3) {
			try {
				int day = std::stoi(argv[2]);
				c.show_ckpt(day);
			} catch (invalid_argument & ex) {
				cout << "Invalid day parameter" << endl;
				cout << usage_str;
			}
		} else {
			cout << "Unexpected parameter" << endl;
			cout << usage_str;
		}
		return 0;
	}
	if(strcmp(argv[1], log_str) == 0) {
		if(argc == 3) {
			c.complete_ckpt(argv[2]);
		} else {
			cout << "Invalid parameters for log command" << endl;
			cout << usage_str;
		}
		return 0;
	}
	if(strcmp(argv[1], stat_str) == 0) {
		if(argc == 5) {
			try {
				int day = std::stoi(argv[2]);
				int month = std::stoi(argv[3]);
				int year = std::stoi(argv[4]);
				c.stat(day, month, year);
			} catch (invalid_argument & ex) {
				cout << "Invalid day, month or year parameter" << endl;
				cout << usage_str;
			}
		} else if(argc == 2) {
				
			time_t theTime = time(NULL);
			struct tm *aTime = localtime(&theTime);
		
			int day = aTime->tm_mday;
			int month = aTime->tm_mon + 1;
			int year = aTime->tm_year + 1900;
			c.stat(day, month, year);
		
		} else {
			cout << "Invalid parameters for stat command" << endl;
			cout << usage_str;
		}
		return 0;
	}
	cout << "Unknown parameters: ";
	for(int i=0; i<argc; i++) {
		cout << argv[i];
		if(i != argc-1) {
			cout << ", ";	
		}
	}
	cout << usage_str;
	return 0;	
}

