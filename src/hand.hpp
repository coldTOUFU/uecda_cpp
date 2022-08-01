#ifndef HAND_HPP_
#define HAND_HPP_

#include <vector>
#include <cstdint>
#include <memory>
#include "cards.hpp"
#include "table.cpp"
#include "hand_summary.cpp"

class Hand {
 public:
  /* 配列形式のカードから手を生成。 */
  Hand(uint16_t src[8][15]);

  /* ビットカードから手を生成。 */
  Hand(Cards::bitcards src, Cards::bitcards joker_src, HandSummary *hs);

  ~Hand() {
    delete this->cards;
    delete this->joker;
    delete this->summary;
  }

  /* 与えられた状況に対して合法手か？ */
  bool isLegal(Table *tbl, Hand *table_hand);

  /* 手のサマリを返す。 */
  HandSummary getSummary() { return *(this->summary); }

  /* 与えられたベクターに、与えられたカードから生成できる手をすべて追加する。 */
  static void pushHands(Cards *src, std::vector<Hand*> *hand_vec);

  /* 与えられた配列に手の構成カードを置く。 */
  void putCards(uint16_t dst[8][15]);

 private:
  static const int kPairFilterSize[4];
  static const Cards::bitcards pairFilters[4][6];
  /* 順に1枚, 2枚, 3枚, 4枚, ..., 14枚の階段用。 */
  static const Cards::bitcards sequenceFilters[14];

  Cards *cards;
  Cards *joker;
  HandSummary *summary;

  /* 与えられたカードを手とみなしてサマリを作る。 */
  static HandSummary *summarize(Cards::bitcards src, Cards::bitcards joker_src);

  /* ジョーカーなしで与えられた配列に指定された枚数の枚数組を作る。 */
  static void pushPair(Cards::bitcards src, std::vector<Hand*> *hand_vec, int pair_qty);

  /* ジョーカー必ず込みで与えられた配列に指定された枚数の枚数組を作る。 */
  static void pushPairWithJoker(Cards::bitcards src, std::vector<Hand*> *hand_vec, int pair_qty);

  /* ジョーカーなしで与えられた配列に指定された枚数の階段を作る。 */
  static void pushSequence(Cards::bitcards src, std::vector<Hand*> *hand_vec, int seq_qty);

  /* ジョーカー必ず込みで与えられた配列に指定された枚数の階段を作る。 */
  static void pushSequenceWithJoker(Cards::bitcards src, std::vector<Hand*> *hand_vec, int seq_qty);
};

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

#endif // HANDS_HPP_
