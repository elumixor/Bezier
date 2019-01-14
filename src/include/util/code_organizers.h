//
// Created by Vladyslav Yazykov on 22/12/2018.
//

#include <iostream>

#ifndef BEZIER_CODE_ORGANIZERS_H
#define BEZIER_CODE_ORGANIZERS_H

//#define _variadic_(_1, _2, _3, _4, _5, _6, _7, _8, _9, NAME, ...) NAME
//#define _variadic_4(_1, _2, _3, _4, NAME, ...) NAME
//#define _variadic_3(_1, _2, _3, NAME, ...) NAME
//#define _variadic_2(_1, _2, NAME, ...) NAME
//
//#if (PRODUCTION)
//#define LOG(message)
//#define ERROR(message)
//#else
//#define LOG(message) printf("LOG: %s\n", String(message).c_str())
//#define ERROR(message) fprintf(stderr, "ERROR: %s\n", String(message).c_str());
//#endif
//
//#define CRASH exit(1);
//
//#define String std::string
//#define CString const char *
//
//#define USE_PROGRAM(program) glUseProgram(graphics::shader::active = program)
//
//#define __require_simple(condition) if (!(condition)) { CRASH }
//#define __require_message(condition, message) if (!(condition)) { ERROR(message); CRASH }
//#define require(...) _variadic_2(__VA_ARGS__, __require_message, __require_simple)(__VA_ARGS__)

#ifdef VERBOSE
#define Log(message) std::clog << "LOG: " << message << "\n"
#else
#define Log(arg)
#endif

#define error(message) std::cerr << "ERROR: " << message << "\n"; return {};
#define require(condition, message) if (!(condition)) { error(message) }

#define OUT std::cout
#define ENDL std::endl
#define NEWL OUT << ENDL

#define COPY(destination, source, count) std::copy(source, source + count, destination)

#define arr_len(array) (sizeof(array)/sizeof(*array))

#endif //BEZIER_CODE_ORGANIZERS_H
