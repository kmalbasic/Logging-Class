#pragma once
#include<iostream>
#include <fstream>
#include <windows.h>
#include <ctime>

enum LOG_TYPE { // log types, to keep the code understandable, neat and easy to use
	SUCCESS_LOG = 0,
	WARNING_LOG = 1,
	ERROR_LOG = 2,
	UNDEFINED_LOG = 3
};

enum LOG_COLOR { // log colors (used in SetConsoleTextAttribute)
	WARNING_COL = 14,
	ERROR_COL = 4,
	SUCCESS_COL = 10,
	DEFAULT_COL = 7
};

namespace CONSOLE { // added for fun
	void printl(int color = 7,const char* prefix = "", const char* msg = "") { // console outputting with colored prefix and a message
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
		std::cout << "[" << prefix << "] ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		std::cout << msg << std::endl;
	}
}

class LOG {
private:
	std::string log_name = "";
	static int warning_cnt;
	static int error_cnt;
	static int success_cnt;
	static int undefined_cnt;
public:
	const char* enum_to_log(LOG_TYPE log_type) { // converting enum -> string, nothing fancy
		switch (log_type) {
		case SUCCESS_LOG:
			this->increment_cnt(log_type);
			return "[SUCCESS] ";
			break;
		case WARNING_LOG:
			this->increment_cnt(log_type);
			return "[WARNING] ";
			break;
		case ERROR_LOG:
			this->increment_cnt(log_type);
			return "[ERROR] ";
			break;
		default:
			this->increment_cnt(log_type);
			return "[UNDEFINED] ";
			break;
		}
	}

	void increment_cnt(LOG_TYPE log_type) { // incrementation of log counters in the class, it's a neat addition if you want a one line resume of your work.
		switch (log_type) {
		case SUCCESS_LOG:
			this->success_cnt++;
			break;
		case WARNING_LOG:
			this->warning_cnt++;
			break;
		case ERROR_LOG:
			this->error_cnt++;
			break;
		default:
			this->undefined_cnt++;
			break;
		}
	}
	const char* log_print(LOG_TYPE log_type, int color = 7, const char* msg = "") { // printing log in console aswell as to the current active log file.
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
		char buffer[256];
		strncpy(buffer, enum_to_log(log_type), sizeof(buffer));
		strncat(buffer, msg, sizeof(buffer));
		std::ofstream append_log_file;
		append_log_file.open(this->log_name + ".log", std::ios::ios_base::app);
		std::time_t curr_date_time = std::time(nullptr);
		std::string buffer_str = "";
		for (int i = 0; i < sizeof(buffer) / sizeof(char); i++) {
			buffer_str = buffer_str + buffer[i];
		}
		std::string time_string(std::asctime(std::localtime(&curr_date_time)));
		for (int i = 0; i < time_string.length(); i++) {
			if (i == time_string.length() - 1) {
				time_string[i] = '~'; // asctime adds a '\n' at the end of the string so we replace it with '~'
			}
		}
		std::string output_to_file = '[' + time_string + ']' + buffer_str;
		append_log_file << output_to_file << std::endl;
		append_log_file.close();
		std::cout << buffer << std::endl;
		return buffer;
	}

	void custom_log(int color = 7, const char* prefix = "", const char* msg = "") {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
		std::cout << "[" << prefix << "] ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		std::cout << msg << std::endl;
		std::time_t curr_date_time = std::time(nullptr);
		std::string time_string(std::asctime(std::localtime(&curr_date_time)));
		for (int i = 0; i < time_string.length(); i++) {
			if (i == time_string.length() - 1) {
				time_string[i] = '~'; // asctime adds a '\n' at the end of the string so we replace it with '~'
			}
		}
		std::string output_to_file = '[' + time_string + ']' + '[' + prefix + "] " + msg + '\n';
		std::ofstream append_log_file;
		append_log_file.open(this->log_name + ".log", std::ios::ios_base::app);
		append_log_file << output_to_file;
	}

	LOG() {
		time_t raw_time;
		struct tm* time_info;
		char buffer[80];
		time(&raw_time);
		time_info = localtime(&raw_time);
		strftime(buffer, sizeof(buffer), "%d-%m-%Y-%H-%M-%S", time_info);
		std::string str(buffer);
		this->log_name = str;
		std::ofstream log_file(this->log_name + ".log");
	}

	void end_logging() {
		std::ofstream append_log_file;
		append_log_file.open(this->log_name + ".log", std::ios::ios_base::app);
		// diving by two due to some weird incrementing issue, it doubles for some reason and I am blind I guess  --  EDIT: fixed, I am retarded.
		append_log_file << "success_cnt -> " << this->success_cnt << std::endl; 
		append_log_file << "warning_cnt -> " << this->warning_cnt << std::endl;
		append_log_file << "error_cnt -> " << this->error_cnt << std::endl;
		append_log_file << "undefined_cnt -> " << this->undefined_cnt << std::endl;
		append_log_file.close();
	}
};

// definitions (static int)
int LOG::success_cnt = 0;
int LOG::error_cnt = 0;
int LOG::warning_cnt = 0;
int LOG::undefined_cnt = 0;

// function that we can use globally anywhere when assigned to the pointer under this definition
void add_log_ptr(LOG log_obj, LOG_TYPE log_type, int color, const char* message) {
	log_obj.log_print(log_type, color, message);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

// this one :)
static void (*add_log)(LOG, LOG_TYPE, int, const char*);

// initializing the log and properly pointing the pointer to add_log_ptr function.
LOG initialize_log() {
	LOG new_log;
	add_log = add_log_ptr;
	return new_log;
}

