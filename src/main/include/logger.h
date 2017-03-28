#ifndef __LOGGER_H_INCLUDED__
#define __LOGGER_H_INCLUDED__
/**
 * A thread-safe logging utility for printing to std out, err, and/or a file
 */

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <istream>

class Logger {
public:
    enum logLevel {ERR = 0, WARN = 1, INFO = 2, DBG = 3};

    Logger(logLevel level, std::ostream& _out, std::ostream& err): out(out), err(err), level(level) { same = false; };
    Logger(logLevel level, std::ostream& _out): out(out), err(out), level(level) { same = true; };

    void write(logLevel level, std::string str);

    // friend std::ostream& operator<< (std::ostream& stream) {
        
    // }

private:
    logLevel level;
    std::ostream& out;
    std::ostream& err;
    bool same;
};

//std::istream& operator >>(std::istream& str, const Logger::logLevel& level);

#endif
