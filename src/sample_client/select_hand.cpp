#include "select_hand.hpp"

std::vector<Hand> select_change_hands(std::vector<Hand> &hands) {
  std::vector<Hand> single_without_joker;

  /* 交換候補として、ジョーカー以外の1枚出しを取り出す */
  for (const Hand& hand : hands) {
    HandSummary summary = hand.getSummary();
    if (!summary.has_joker && summary.quantity == 1) {
      single_without_joker.push_back(hand);
    }
  }

  sort(single_without_joker.begin(), single_without_joker.end(), [](Hand &h1, Hand &h2) {
    return h1.getSummary().strongest_order > h2.getSummary().strongest_order;
  });

  return single_without_joker;
}

/* 手の枚数と強さの観点のみから最善の手を選ぶ。 */
Hand &select_best_hand_in_same_card_type(std::vector<Hand> &hands, bool is_rev) {
  Hand &best_hand = hands[0];
  for (Hand &hand : hands) {
    const HandSummary &best_summary = best_hand.getSummary();
    const HandSummary &cur_summary = hand.getSummary();
    if (cur_summary.quantity < best_summary.quantity) {
      continue;
    }
    else if (cur_summary.quantity > best_summary.quantity) {
      best_hand = hand;
    }
    else if (!is_rev && cur_summary.strongest_order > best_summary.strongest_order) {
      best_hand = hand;
    }
    else if (is_rev && cur_summary.strongest_order < best_summary.strongest_order) {
      best_hand = hand;
    }
  }
  return best_hand;
}

Hand select_hand(std::vector<Hand> &hands, const Hand &table_hand, const Table &table) {
  /* 手の候補がなければ(あり得ないが)、パスの手を返す。 */
  if (hands.size() <= 0) {
    return Hand();
  }

  std::vector<Hand> single_without_joker(0);
  std::vector<Hand> pair_without_joker(0);
  std::vector<Hand> sequence_without_joker(0);
  std::vector<Hand> single_with_joker(0);
  std::vector<Hand> pair_with_joker(0);
  std::vector<Hand> sequence_with_joker(0);

  /* 着手候補を手の種類・ジョーカーの有無で別々の配列に分ける。 */
  for (const Hand &hand : hands) {
    if (!hand.isLegal(table, table_hand)) {
      continue;
    }

    const HandSummary summary = hand.getSummary();
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
    return select_best_hand_in_same_card_type(sequence_without_joker, table.is_rev);
  }

  if (sequence_with_joker.size() > 0) {
    return select_best_hand_in_same_card_type(sequence_with_joker, table.is_rev);
  }

  if (pair_without_joker.size() > 0) {
    return select_best_hand_in_same_card_type(pair_without_joker, table.is_rev);
  }

  if (pair_with_joker.size() > 0) {
    return select_best_hand_in_same_card_type(pair_with_joker, table.is_rev);
  }

  if (single_without_joker.size() > 0) {
    return select_best_hand_in_same_card_type(single_without_joker, table.is_rev);
  }

  if (single_with_joker.size() > 0) {
    return select_best_hand_in_same_card_type(single_with_joker, table.is_rev);
  }

  /* 合法手がなかった場合はパスを返す。 */
  return Hand();
}
