#pragma once
#include<iostream>
#include<vector>
#include<chrono>
#include<sstream>
#include <functional>
#include <thread>
#include "Logging.hpp"

class TIME { // class for determining execution time, performance statistics and mostly debugging the whole program
private:
	std::chrono::steady_clock::time_point program_started_time = std::chrono::high_resolution_clock::now(); // time at which we started the program
	std::vector<std::chrono::steady_clock::time_point> time_points; // currently used for debugging
	std::vector<std::string> function_name_tp;						// currently used for debugging
public:
	void execute_function(std::thread fn_thread, const char* function_name, LOG log_obj,   // using this function, we pass a thread with our function and it will save the
				    LOG_TYPE log_type, void (*add_log)(LOG, LOG_TYPE, int, const char*))   // execution time as a log
	{
		auto start_time = std::chrono::high_resolution_clock::now();
		fn_thread.join();
		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
		this->time_points.push_back(end_time);
		this->function_name_tp.push_back(function_name);
		std::cout << duration.count() << std::endl;
		std::string log = "";
		log += function_name;
		log += " - time (ms) - ";
		std::string duration_to_str = std::to_string(duration.count());
		log += duration_to_str;
		log += '\n';
		add_log(log_obj, log_type, 7, log.c_str()); // same add_log pointer we pass to execute_function, really easy to use as mentioned
	}
};