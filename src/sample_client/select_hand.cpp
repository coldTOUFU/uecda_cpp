#include "select_hand.hpp"

std::vector<Hand*> select_change_hands(std::vector<Hand*>* hands) {
  std::vector<Hand*> single_without_joker(0);

  /* 交換候補として、ジョーカー以外の1枚出しを取り出す */
  for (const auto& hand : *hands) {
    HandSummary summary = hand->getSummary();
    if (!summary.has_joker && summary.quantity == 1) {
      single_without_joker.push_back(hand);
    }
  }

  sort(single_without_joker.begin(), single_without_joker.end(), [](Hand *h1, Hand *h2) {
    return h1->getSummary().strongest_order > h2->getSummary().strongest_order;
  });

  return single_without_joker;
}

Hand* select_hand(std::vector<Hand*>* hands, Hand* table_hand, Table* table) {
  if (hands->size() <= 0) {
    return nullptr;
  }

  std::vector<Hand*> single_without_joker(0);
  std::vector<Hand*> pair_without_joker(0);
  std::vector<Hand*> sequence_without_joker(0);
  std::vector<Hand*> single_with_joker(0);
  std::vector<Hand*> pair_with_joker(0);
  std::vector<Hand*> sequence_with_joker(0);

  /* 着手候補を手の種類・ジョーカーの有無で別々の配列に分ける。 */
  for (const auto& hand : *hands) {
    if (!hand->isLegal(table, table_hand)) {
      continue;
    }

    HandSummary summary = hand->getSummary();
    if (!summary.has_joker) {
      if (summary.quantity == 1) {
        single_without_joker.push_back(hand);
      } else if (summary.card_type == Cards::CARD_TYPES::kPair) {
        pair_without_joker.push_back(hand);
      } else {
        sequence_without_joker.push_back(hand);
      }
    } else {
      if (summary.quantity == 1) {
        single_with_joker.push_back(hand);
      } else if (summary.card_type == Cards::CARD_TYPES::kPair) {
        pair_with_joker.push_back(hand);
      } else {
        sequence_with_joker.push_back(hand);
      }
    }
  }

  /* 階段→n(>1)枚出し→1枚出し、ジョーカーなし→ジョーカーありの順で手を探索 */
  if (sequence_without_joker.size() > 0) {
    Hand *best_hand = sequence_without_joker[0];
    for (const auto& hand : sequence_without_joker) {
      HandSummary best_summary = best_hand->getSummary();
      HandSummary cur_summary = hand->getSummary();
      if (cur_summary.quantity < best_summary.quantity) {
        continue;
      }
      else if (cur_summary.quantity > best_summary.quantity) {
        best_hand = hand;
      }
      else if (!table->getIsRev() && cur_summary.strongest_order > best_summary.strongest_order) {
        best_hand = hand;
      }
      else if (table->getIsRev() && cur_summary.strongest_order < best_summary.strongest_order) {
        best_hand = hand;
      }
    }
    return best_hand;
  }
  if (sequence_with_joker.size() > 0) {
    Hand *best_hand = sequence_with_joker[0];
    for (const auto& hand : sequence_with_joker) {
      HandSummary best_summary = best_hand->getSummary();
      HandSummary cur_summary = hand->getSummary();
      if (cur_summary.quantity < best_summary.quantity) {
        continue;
      }
      else if (cur_summary.quantity > best_summary.quantity) {
        best_hand = hand;
      }
      else if (!table->getIsRev() && cur_summary.strongest_order > best_summary.strongest_order) {
        best_hand = hand;
      }
      else if (table->getIsRev() && cur_summary.strongest_order < best_summary.strongest_order) {
        best_hand = hand;
      }
    }
    return best_hand;
  }

  if (pair_without_joker.size() > 0) {
    Hand *best_hand = pair_without_joker[0];
    for (const auto& hand : pair_without_joker) {
      HandSummary best_summary = best_hand->getSummary();
      HandSummary cur_summary = hand->getSummary();
      if (cur_summary.quantity < best_summary.quantity) {
        continue;
      }
      else if (cur_summary.quantity > best_summary.quantity) {
        best_hand = hand;
      }
      else if (!table->getIsRev() && cur_summary.strongest_order > best_summary.strongest_order) {
        best_hand = hand;
      }
      else if (table->getIsRev() && cur_summary.strongest_order < best_summary.strongest_order) {
        best_hand = hand;
      }
    }
    return best_hand;
  }
  if (pair_with_joker.size() > 0) {
    Hand *best_hand = pair_with_joker[0];
    for (const auto& hand : pair_with_joker) {
      HandSummary best_summary = best_hand->getSummary();
      HandSummary cur_summary = hand->getSummary();
      if (cur_summary.quantity < best_summary.quantity) {
        continue;
      }
      else if (cur_summary.quantity > best_summary.quantity) {
        best_hand = hand;
      }
      else if (!table->getIsRev() && cur_summary.strongest_order > best_summary.strongest_order) {
        best_hand = hand;
      }
      else if (table->getIsRev() && cur_summary.strongest_order < best_summary.strongest_order) {
        best_hand = hand;
      }
    }
    return best_hand;
  }
  if (single_without_joker.size() > 0) {
    Hand *best_hand = single_without_joker[0];
    for (const auto& hand : single_without_joker) {
      HandSummary best_summary = best_hand->getSummary();
      HandSummary cur_summary = hand->getSummary();
      if (!table->getIsRev() && cur_summary.strongest_order > best_summary.strongest_order) {
        best_hand = hand;
      }
      else if (table->getIsRev() && cur_summary.strongest_order < best_summary.strongest_order) {
        best_hand = hand;
      }
    }
    return best_hand;
  }
  if (single_with_joker.size() > 0) {
    Hand *best_hand = single_with_joker[0];
    for (const auto& hand : single_with_joker) {
      HandSummary best_summary = best_hand->getSummary();
      HandSummary cur_summary = hand->getSummary();
      if (!table->getIsRev() && cur_summary.strongest_order > best_summary.strongest_order) {
        best_hand = hand;
      }
      else if (table->getIsRev() && cur_summary.strongest_order < best_summary.strongest_order) {
        best_hand = hand;
      }
    }
    return best_hand;
  }

  /* 合法手がなかった場合。 */
  return nullptr;
}
