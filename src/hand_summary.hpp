#ifndef HAND_SUMMARY_HPP_
#define HAND_SUMMARY_HPP_

#include <bitset>

#include "cards.hpp"

namespace uecda {
  struct HandSummary {
    int quantity;
    bool is_pass;
    bool is_sequence;
    Cards::bitcards weakest_order;
    Cards::bitcards strongest_order;
    bool has_joker;
    int suits;

    void print() const {
      std::cout << "枚数: " << quantity << std::endl;
      std::cout << "パス？: " << (this->is_pass ? "YES" : "NO") << std::endl;
      std::cout << "階段？: " << (this->is_sequence) << std::endl;
      std::cout << "最弱のカード: " << std::bitset<15>(weakest_order) << std::endl;
      std::cout << "最強のカード: " << std::bitset<15>(strongest_order) << std::endl;
      std::cout << "ジョーカーあり？: " << (this->has_joker ? "YES" : "NO") << std::endl;
      std::cout << "スート: " << std::bitset<4>(suits) << std::endl;
    }

    bool operator ==(const HandSummary &src) const {
      return quantity == src.quantity &&
          is_pass == src.is_pass &&
          is_sequence == src.is_sequence &&
          weakest_order == src.weakest_order &&
          strongest_order == src.strongest_order &&
          has_joker == src.has_joker &&
          suits == src.suits;
    }
  };
}

#endif  // HAND_SUMMARY_HPP_
