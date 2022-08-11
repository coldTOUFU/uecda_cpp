#ifndef SELECT_HAND_HPP_
#define SELECT_HAND_HPP_

#include <algorithm>

#include "../hand.hpp"
#include "../hand_summary.hpp"
#include "../table.hpp"

std::vector<Hand> select_change_hands(std::vector<Hand>& hands);
Hand &select_best_hand_in_same_card_type(std::vector<Hand> &hands, bool is_rev);
Hand select_hand(std::vector<Hand>& hands, const Hand& table_hand, const Table& table);

#endif // SELECT_HAND_HPP_
