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
Hand &select_best_hand_in_same_hand_type(std::vector<Hand> &hands, bool is_rev) {
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

/* 1枚組を出す場合、複数枚役を崩さないモノを優先する。 */
Hand select_isolated_hand(Cards my_cards, std::vector<Hand>& hands, const Hand& table_hand, const Table& table) {
  /* 手の候補がないかカードが空なら(あり得ないが)、パスの手を返す。 */
  if (my_cards.quantity() <= 0 || hands.size() <= 0) {
    return Hand();
  }

  /* ジョーカーなしで複数枚組を構成するカードを候補から除外。 */
  for (const Hand &hand : hands) {
    const HandSummary summary = hand.getSummary();
     if(!summary.has_joker) {
      if (summary.quantity > 1) {
        my_cards -= hand.getCards();
      }
    }
  }

  /* 候補のカードが0になったらパスを返す。 */
  if (my_cards.quantity() <= 0) {
    return Hand();
  }

  /* 候補の役を生成。 */
  std::vector<Hand> isolated_hands;  
  Hand::pushHands(my_cards, isolated_hands);

  /* 候補からジョーカー以外の合法な1枚出しを取り出す。 */
  std::vector<Hand> single_without_joker;
  for (const Hand &hand : hands) {
    if (!hand.isLegal(table, table_hand)) {
      continue;
    }
    const HandSummary summary = hand.getSummary();
    if (!summary.has_joker && summary.quantity == 1) {
      single_without_joker.push_back(hand);
    }
  }

  /* 最良の手を返す。 */
  if (single_without_joker.size() > 0) {
    return select_best_hand_in_same_hand_type(single_without_joker, table.is_rev);
  } else {
    return Hand();
  }
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
      } else if (!summary.is_sequence) {
        pair_without_joker.push_back(hand);
      } else {
        sequence_without_joker.push_back(hand);
      }
    } else {
      if (summary.quantity == 1) {
        single_with_joker.push_back(hand);
      } else if (!summary.is_sequence) {
        pair_with_joker.push_back(hand);
      } else {
        sequence_with_joker.push_back(hand);
      }
    }
  }

  /* 階段→n(>1)枚出し→1枚出し、ジョーカーなし→ジョーカーありの順で手を探索 */
  if (sequence_without_joker.size() > 0) {
    return select_best_hand_in_same_hand_type(sequence_without_joker, table.is_rev);
  }

  if (sequence_with_joker.size() > 0) {
    return select_best_hand_in_same_hand_type(sequence_with_joker, table.is_rev);
  }

  if (pair_without_joker.size() > 0) {
    return select_best_hand_in_same_hand_type(pair_without_joker, table.is_rev);
  }

  if (pair_with_joker.size() > 0) {
    return select_best_hand_in_same_hand_type(pair_with_joker, table.is_rev);
  }

  if (single_without_joker.size() > 0) {
    return select_best_hand_in_same_hand_type(single_without_joker, table.is_rev);
  }

  if (single_with_joker.size() > 0) {
    return select_best_hand_in_same_hand_type(single_with_joker, table.is_rev);
  }

  /* 合法手がなかった場合はパスを返す。 */
  return Hand();
}
