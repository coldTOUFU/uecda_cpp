#ifndef HAND_HPP_
#define HAND_HPP_

#include <memory>
#include <vector>
#include "error/hand_error.hpp"
#include "cards.hpp"
#include "hand_summary.hpp"
#include "table.hpp"
#include "uecda_common.hpp"

class Hand {
 public:
  /* 空の手 = パス */
  Hand(): cards_(), joker_(), summary_() {};

  /* 配列形式のカードから手を生成。 */
  Hand(const uecda_common::CommunicationBody src): cards_(Hand::createCards(src)), joker_(Hand::createJoker(src)), summary_(Hand::summarize(cards_.getCard(), joker_.getCard())) {};

  /* ビットカードから手を生成。 */
  Hand(const Cards::bitcards src, const Cards::bitcards joker_src, const HandSummary& hs): cards_(Cards(src)), joker_(Cards(joker_src)), summary_(HandSummary(hs)) {};

  /* 与えられた状況に対して合法手か？ */
  bool isLegal(const Table& tbl, const Hand& table_hand) const;

  /* 手のサマリを返す。 */
  HandSummary getSummary() const { return this->summary_; }

  /* 与えられたベクターに、与えられたカードから生成できる手をすべて追加する。 */
  static void pushHands(const Cards& src, std::vector<Hand>& hand_vec);

  /* 与えられた配列に手の構成カードを置く。 */
  void putCards(uecda_common::CommunicationBody dst) const;

  /* デバッグ用に手を出力。 */
  void printHand() const {
    uecda_common::CommunicationBody src = {{}};
    this->putCards(src);
    std::cout << std::endl;
    std::cout << "カードの種類: " << ((this->summary_.card_type == Cards::CARD_TYPES::kPair) ? "枚数型" : "階段") << std::endl;
    std::cout << "カードの枚数: " << this->summary_.quantity << std::endl;
    Cards::printCards(src);
  }

 private:
  static const int kPairFilterSize[4];
  static const Cards::bitcards pairFilters[4][6];
  /* 順に1枚, 2枚, 3枚, 4枚, ..., 14枚の階段用。 */
  static const Cards::bitcards sequenceFilters[14];

  Cards cards_;
  Cards joker_;
  HandSummary summary_;

  /* コンストラクタ用のヘルパーメソッド。配列形式のカードからジョーカー以外の部分のCardsを生成。 */
  static Cards createCards(const uecda_common::CommunicationBody src);

  /* コンストラクタ用のヘルパーメソッド。配列形式のカードからジョーカーの部分のCardsを生成。 */
  static Cards createJoker(const uecda_common::CommunicationBody src);

  /* 与えられたカードを手とみなしてサマリを作る。 */
  static HandSummary summarize(Cards::bitcards src, Cards::bitcards joker_src);

  /* ジョーカーなしで与えられた配列に指定された枚数の枚数組を作る。 */
  static void pushPair(Cards::bitcards src, std::vector<Hand>& hand_vec,
                       int pair_qty);

  /* ジョーカー必ず込みで与えられた配列に指定された枚数の枚数組を作る。 */
  static void pushPairWithJoker(Cards::bitcards src,
                                std::vector<Hand>& hand_vec, int pair_qty);

  /* ジョーカーなしで与えられた配列に指定された枚数の階段を作る。 */
  static void pushSequence(Cards::bitcards src, std::vector<Hand>& hand_vec,
                           int seq_qty);

  /* ジョーカー必ず込みで与えられた配列に指定された枚数の階段を作る。 */
  static void pushSequenceWithJoker(Cards::bitcards src,
                                    std::vector<Hand>& hand_vec, int seq_qty);
};

#endif  // HANDS_HPP_
