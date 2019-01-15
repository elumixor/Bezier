//
// Created by Vladyslav Yazykov on 22/12/2018.
//

#include <iostream>

#ifndef BEZIER_CODE_ORGANIZERS_H
#define BEZIER_CODE_ORGANIZERS_H

#ifdef VERBOSE
#define INFO(message) std::clog << "LOG: " << message << "\n"
#else
#define INFO(arg)
#endif

#define error(message) std::cerr << "ERROR: " << message << "\n"; return {};
#define require(condition, message) if (!(condition)) { error(message) }

#define OUT std::cout
#define ENDL std::endl
#define NEWL OUT << ENDL

#define COPY(destination, source, count) std::copy(source, source + count, destination)

#define arr_len(array) (sizeof(array)/sizeof(*array))

#endif //BEZIER_CODE_ORGANIZERS_H
