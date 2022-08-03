#ifndef HAND_SUMMARY_HPP_
#define HAND_SUMMARY_HPP_

#include "cards.hpp"

struct HandSummary {
  int quantity;
  Cards::CARD_TYPES card_type;
  Cards::bitcards weakest_order;
  Cards::bitcards strongest_order;
  bool has_joker;
  int suits;
};

#endif  // HAND_SUMMARY_HPP_
