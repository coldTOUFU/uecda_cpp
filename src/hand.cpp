#include "hand.hpp"

Hand::Hand(uint16_t src[8][15]) {
  Cards::bitcards c = (Cards::bitcards)0;
  Cards::bitcards j = (Cards::bitcards)0;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 15; j++) {
      if (src[i][j] == 1) {
        c++;
      }
      else if (src[i][j] == 2) {
        j++;
      }
      c <<= 1;
      j <<= 1;
    }
  }

  /* 上で余計に1回左シフトした分を戻す。 */
  c >>= 1;
  j >>= 1;

  this->cards = new Cards(c);
  this->joker = new Cards(j);
  this->summary = this->summarize(c, j);
}

Hand::Hand(Cards::bitcards src, Cards::bitcards joker_src, HandSummary *hs) {
  this->cards = new Cards(src);
  this->joker = new Cards(joker_src);
  this->summary = new HandSummary(*hs);
}

bool Hand::isLegal(Table *tbl, Hand *table_hand) {
  if (tbl->getIsStartOfTrick()) { return true; }

  HandSummary table_hand_summary = table_hand->getSummary();

  /* 場と同じ種類の手である必要がある。 */
  if (this->summary->card_type != table_hand_summary.card_type)  { return false; }

  /* 場のカードと同枚数である必要がある。 */
  if (this->summary->quantity != table_hand_summary.quantity) { return false; }

  /* 出すカードの最小が場のカードの最大より強い必要がある。 */
  /* *_ordは15bit整数で、小さいほど強い。第1bitと第15bitはJoker用。 */
  if (!tbl->getIsRev() && this->summary->weakest_order >= table_hand_summary.strongest_order) { return false; }
  if (tbl->getIsRev()  && this->summary->strongest_order <= table_hand_summary.weakest_order) { return false; }

  /* しばりなら、スートが一致する必要がある。 */
  if (tbl->getIsLock()) {
    if (this->summary->suits != table_hand_summary.suits) { return false; }
  }

  return true;
}

void Hand::pushHands(Cards *src, std::vector<Hand*> *hand_vec) {
  Cards::bitcards src_bit = src->getCard();

  for (int pair_qty = 1; pair_qty <= 4; pair_qty++) {
    Hand::pushPair(src_bit, hand_vec, pair_qty);
  }
  for (int seq_qty = 3; seq_qty <= 14; seq_qty++) {
    Hand::pushSequence(src_bit, hand_vec, seq_qty);
  }

  /* ジョーカーがあればジョーカーを用いた役を追加。 */
  if (src->hasJoker()) {
    /* ジョーカー単騎 */
    Cards::bitcards c = (Cards::bitcards)0;
    Cards::bitcards j = (Cards::bitcards)1;
    HandSummary *hs = Hand::summarize(c, j);
    hand_vec->push_back(new Hand(c, j, hs));
    delete hs;

    for (int pair_qty = 2; pair_qty <= 4; pair_qty++) {
      Hand::pushPair(src_bit, hand_vec, pair_qty);
    }
    for (int seq_qty = 3; seq_qty <= 14; seq_qty++) {
      Hand::pushSequence(src_bit, hand_vec, seq_qty);
    }
  }
}

void Hand::putCards(uint16_t dst[8][15]) {
  Cards::bitcards src = this->cards->getCard();
  Cards::bitcards src_joker = this->joker->getCard();

  for (int i = 14; i >= 0; i--) {
    for (int j = 14; j >= 0; j--) {
      dst[i][j] = src % 2;
      /* ジョーカーがある場合 */
      if (src_joker % 2) {
        dst[i][j] = 2;
      }

      src >>= 1;
      src_joker >>= 1;
    }
  }
}


const int Hand::kPairFilterSize[4] = {4, 6, 4, 1};
const Cards::bitcards Hand::pairFilters[4][6] = { 
  { 1, 32768, 1073741824, 35184372088832 }, /* 1枚用。 */
  { 32769, 1073741825, 35184372088833, 1073774592, 
    35184372121600, 35185445830656 }, /* 2枚用。4つから2つ選ぶから6通り。 */
  { 1073774593, 35184372121601, 35185445830657, 
   35185445863424 }, /* 3枚用。4つから3つ選ぶから4通り。 */
  { 35185445863425 } /* 4枚用。 */
};
const Cards::bitcards Hand::sequenceFilters[14] =  { 1, 3, 7, 15, 31, 63, 127, 255, 511,
                                                1023, 2047, 4095, 8191, 16383 };
 
HandSummary *Hand::summarize(Cards::bitcards src, Cards::bitcards joker_src) {
  Cards *src_card = new Cards(src);
  Cards *joker_src_card = new Cards(joker_src);

  Cards::bitcards w_ord = 
    (src_card->weakestOrder() > joker_src_card->weakestOrder())
    ? src_card->weakestOrder() : joker_src_card->weakestOrder();
  Cards::bitcards s_ord = 
    (src_card->strongestOrder() < joker_src_card->strongestOrder())
    ? src_card->strongestOrder() : joker_src_card->strongestOrder();
  
  HandSummary *hs = new HandSummary();
  *hs = {
    src_card->quantity() + joker_src_card->quantity(),
    Cards::CARD_TYPES::kPair,
    w_ord,
    s_ord,
    joker_src_card->quantity() > 0,
    src_card->getSuits() | joker_src_card->getSuits()
  };
  return hs;
}

void Hand::pushPair(Cards::bitcards src, std::vector<Hand*> *hand_vec, int pair_qty) {
  if (pair_qty < 1 || pair_qty > 4) { return; } // フィルターを定義していないseq_qtyが来たら何もしない。

  /* 各フィルター(=スートの組み合わせ)に対し、各数字についてペアを探す。 */
  for (int i = 0; i < Hand::kPairFilterSize[pair_qty - 1]; i++) {
    for (int j = 0; j < 15; j++){
      Cards::bitcards tmpfilter = (Hand::pairFilters[pair_qty - 1][i] << j);
      if ((src & tmpfilter) == tmpfilter) {
        Cards::bitcards j = (Cards::bitcards)0;
        HandSummary *hs = Hand::summarize(tmpfilter, j);
        hand_vec->push_back(new Hand(tmpfilter, j, hs));
        delete hs;
      }
    }
  }
}

void Hand::pushPairWithJoker(Cards::bitcards src, std::vector<Hand*> *hand_vec, int pair_qty) {
  if (pair_qty < 1 || pair_qty > 4) { return; } // フィルターを定義していないseq_qtyが来たら何もしない。

  for (int i = 0; i < Hand::kPairFilterSize[pair_qty - 1]; i++) {
    for (int j = 0; j < 15; j++) {
      Cards::bitcards tmpfilter= (Hand::pairFilters[pair_qty - 1][i] << j);
      if (Cards::count(src & tmpfilter) == pair_qty - 1) {
        Cards::bitcards c = (src & tmpfilter);
        Cards::bitcards j = (~src & tmpfilter);
        HandSummary *hs = Hand::summarize(c, j);
        hand_vec->push_back(new Hand(c, j, hs));
        delete hs;
      }
    }
  }
}

void Hand::pushSequence(Cards::bitcards src, std::vector<Hand*> *hand_vec, int seq_qty) {
  if (seq_qty < 3 || seq_qty > 14) { return; } // フィルターを定義していないseq_qtyが来たら何もしない。

  /* 各スートに対し、各数字に対して階段を探す。 */
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j <= 15 - seq_qty; j++) {
      /* filterを、i番目のスートでj番目から始めるものに適用できるようシフトする。 */
      Cards::bitcards tmpfilter = (Hand::sequenceFilters[seq_qty] << (15 * i + j));
      if((src & tmpfilter) == tmpfilter) { 
        Cards::bitcards j = (Cards::bitcards)0;
        HandSummary *hs = Hand::summarize(tmpfilter, j);
        hand_vec->push_back(new Hand(tmpfilter, j, hs));
        delete hs;
      }
    }
  }
}

void Hand::pushSequenceWithJoker(Cards::bitcards src, std::vector<Hand*> *hand_vec, int seq_qty) {
  if (seq_qty < 3 || seq_qty > 14) { return; }

  /* 各スートに対し、各数字に対して階段を探す。 */
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j <= 15 - seq_qty; j++) {
      /* filterを、i番目のスートでj番目から始めるものに適用できるようシフトする。 */
      Cards::bitcards tmpfilter = (Hand::sequenceFilters[seq_qty] << (15 * i + j));
      if (Cards::count(src & tmpfilter) == seq_qty - 1) {
        Cards::bitcards c = (src & tmpfilter);
        Cards::bitcards j = (~src & tmpfilter);
        HandSummary *hs = Hand::summarize(c, j);
        hand_vec->push_back(new Hand(c, j, hs));
        delete hs;
      }
    }
  }
}
