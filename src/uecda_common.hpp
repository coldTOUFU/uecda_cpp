#ifndef UECDA_COMMON_HPP_
#define UECDA_COMMON_HPP_

#include <cstdint>
#include <iostream>

namespace uecda_common {
using CommunicationBody = uint32_t[8][15];
void printCommunicationBody(CommunicationBody src);
}  // namespace uecda_common

#endif  // UECDA_COMMON_HPP_
