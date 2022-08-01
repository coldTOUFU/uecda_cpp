#ifndef SELECT_HAND_HPP_
#define SELECT_HAND_HPP_

#include <algorithm>

#include "../hand.hpp"
#include "../hand_summary.cpp"
#include "../table.cpp"

std::vector<Hand*> select_change_hands(std::vector<Hand*>* hands);
Hand* select_hand(std::vector<Hand*>* hands, Hand* table_hand, Table* table);

#endif // SELECT_HAND_HPP_
