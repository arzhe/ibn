#ifndef INDICATORS_CURSOR_MOVEMENT_HPP
#define INDICATORS_CURSOR_MOVEMENT_HPP

#include <iostream>

namespace indicators {

static inline void MoveUp(int lines)   { std::cout << "\033[" << lines << "A"; }
static inline void MoveDown(int lines) { std::cout << "\033[" << lines << "B"; }
static inline void MoveRight(int cols) { std::cout << "\033[" << cols << "C";  }
static inline void MoveLeft(int cols)  { std::cout << "\033[" << cols << "D";  }

} // !namespace indicators

#endif // !INDICATORS_CURSOR_MOVEMENT_HPP
