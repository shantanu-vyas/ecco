#ifndef ASSERTMACROS_HPP
#define ASSERTMACROS_HPP

#include <cassert>
#include <iostream>

//if i want to quickly disable all asserts for some debugging but have log statements (maybe looking for a second assert fix)
#define assertIsWarning 0


//I would like to have a local assert is warning for specific asserts
//maybe just make ecco_assert_fn(cond, message, localAssertIsWarning, file, line) and a macro wrapper?
#define ecco_assert(condition, message)                                     \
    do {                                                                    \
        if (!(condition)) {                                                 \
            std::cerr << "Asserted at " << __FILE__ << ":"                  \
                      << __LINE__ << "\nCondition: " << #condition          \
                      << "\nMessage: " << message << std::endl;             \
            assert(!assertIsWarning && condition);                          \
        }                                                                   \
    } while (0)

#endif
