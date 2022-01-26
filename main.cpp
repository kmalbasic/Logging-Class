#include <iostream>
#include "Timer.hpp"
#include "Console.hpp"

void test_function(int n) { // testing function for TIME object -> execute_function
    while (n > 0) {
        std::cout << "TESTING" << std::endl;
        n--;
    }
}

int main()
{
#if NDEBUG
    create_console(); // If you are not running a console application, this will allocate a separate console. As of now,
                      // it will only allocate a separate console window if it detects NDEBUG argument in preprocessor (debugging).
#endif
    LOG test_obj = initialize_log(); // initialize log class
    TIME time_obj; // initialize time class 
    int n = 6;
    while (n > 0) { // add some logs just for testing purposes
        add_log(test_obj, SUCCESS_LOG, SUCCESS_COL, "fn pointer works.");
        add_log(test_obj, ERROR_LOG, ERROR_COL, "fn pointer works.");
        add_log(test_obj, WARNING_LOG, WARNING_COL, "fn pointer works.");
        add_log(test_obj, UNDEFINED_LOG, DEFAULT_COL, "fn pointer works.");
        n--;
    }
    time_obj.execute_function(std::thread(test_function, 20), "test_function", test_obj, SUCCESS_LOG, add_log);   // test the TIME class and it's main function, PASS YOUR FUNCTION WITHIN
    CONSOLE::printl(WARNING_COL, "TEST_PREFIX", "testing test prefix"); // similar to custom_log() in LOG class.     A STD::THREAD!
    
    test_obj.end_logging(); // finish the logging and print out log counter values.

    return 0;
}

