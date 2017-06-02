#include <istream>
#include "logger.h"

using namespace std;

void Logger::write(logLevel printLevel, std::string str){
    if(printLevel > level)
        return; //Not important enough to log

    switch (printLevel) {
        case DBG:
            out << "DBG " << str;
            break;
        case INFO:
            out << "INFO " << str;
            break;
        case WARN:
            out << "WARN " << str;
        case ERR:
            out << "ERR " << str;
            if(!same)
                err << "ERR " << str;
            break;
    }
}

// std::istream& Logger::logLevel::operator >>(std::istream& str, const Logger::logLevel& level)
// {
//     std::string s(std::istreambuf_iterator<char>(str), {});
//     Logger::write(level, s);
//     return str;
// }