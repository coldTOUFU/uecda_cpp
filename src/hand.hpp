#ifndef HAND_HPP_
#define HAND_HPP_

#include <memory>
#include <vector>

#include "error/hand_error.hpp"
#include "cards.hpp"
#include "hand_summary.hpp"
#include "table.hpp"
#include "uecda_common.hpp"

namespace uecda {
  class Hand {
   public:
    /* 空の手 = パス */
    Hand(): cards_(), joker_(), summary_(Hand::summarize({}, {})) {};

    /* 配列形式のカードから手を生成。 */
    Hand(const uecda::common::CommunicationBody& src): cards_(Hand::createCards(src)), joker_(Hand::createJoker(src)), summary_(Hand::summarize(cards_.toBitcards(), joker_.toBitcards())) {};

    /* ビットカードから手を生成。 */
    Hand(const Cards::bitcards src, const Cards::bitcards joker_src): cards_(Cards(src)), joker_(Cards(joker_src)), summary_(summarize(src, joker_src)) {};

    /* コピー。 */
    Hand(const uecda::Hand& src): cards_(src.cards_), joker_(src.joker_), summary_(src.summary_) {};

    /* 与えられた状況に対して合法手か？ */
    bool isLegal(const Table& tbl, const Hand& table_hand) const;

    /* 手のサマリを返す。 */
    HandSummary getSummary() const { return summary_; }

    /* 与えられたベクターに、与えられたカードから生成できる手をすべて追加する。パスは除く。 */
    static void pushHands(const Cards& src, std::vector<Hand>& hand_vec);

    /* 与えられたベクターに、与えられたカードから生成できる合法手をすべて追加する。パスは除く。 */
    static void pushLegalHands(const Cards& src, std::vector<Hand>& hand_vec, const Table& table, const Hand& table_hand);

    /* ジョーカー以外のカードを返す。 */
    Cards getCards() const {
      return cards_;
    }

    /* ジョーカーを返す。 */
    Cards getJoker() const {
      return joker_;
    }

    /* ジョーカーを含むカードを返す。ジョーカーの位置は特定できない。 */
    Cards getWholeBitcards() const {
      return cards_ + joker_;
    }

    /* 革命を起こせる？ */
    constexpr bool canRevolute() const {
      return summary_.is_sequence ? summary_.quantity >= 5 : summary_.quantity >= 4;
    }

    /* 与えられた配列に手の構成カードを置く。 */
    void putCards(uecda::common::CommunicationBody& dst) const;

    /* デバッグ用に手を出力。 */
    void print() const {
      std::cout << *this;
    }

    bool operator ==(const Hand& src) const {
      return cards_ == src.cards_ && joker_ == src.joker_ && summary_ == src.summary_;
    }

    /* order1がorder2より強い？ */
    constexpr static bool isFormerStronger(bool is_rev, Cards::bitcards order1, Cards::bitcards order2) {
      /* orderは15bit整数で、(革命でない場合)小さいほど強い。 */
      return (!is_rev && order1 < order2) || (is_rev && order1 > order2);
    }

    /* order1がorder2より弱い？ */
    constexpr static bool isFormerWeaker(bool is_rev, Cards::bitcards order1, Cards::bitcards order2) {
      /* orderは15bit整数で、(革命でない場合)大きいほど弱い。 */
      return (!is_rev && order1 > order2) || (is_rev && order1 < order2);
    }

   private:
    static constexpr std::array<int, 4> kPairFilterSize{4, 6, 4, 1};
    static constexpr std::array<std::array<Cards::bitcards, 6>, 4> kPairFilters{{
      {1, 32768, 1073741824, 35184372088832},                                          /* 1枚用。 */
      {32769, 1073741825, 35184372088833, 1073774592, 35184372121600, 35185445830656}, /* 2枚用。4つから2つ選ぶから6通り。 */
      {1073774593, 35184372121601, 35185445830657, 35185445863424},                    /* 3枚用。4つから3つ選ぶから4通り。 */
      {35185445863425}}};                                                              /* 4枚用。 */
    static constexpr std::array<Cards::bitcards, 14> kSequenceFilters{
      1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383}; /* 1枚, 2枚, 3枚, 4枚, ..., 14枚の階段用。*/

    Cards cards_;
    Cards joker_;
    HandSummary summary_;

    /* コンストラクタ用のヘルパーメソッド。配列形式のカードからジョーカー以外の部分のCardsを生成。 */
    static Cards createCards(const uecda::common::CommunicationBody& src);

    /* コンストラクタ用のヘルパーメソッド。配列形式のカードからジョーカーの部分のCardsを生成。 */
    static Cards createJoker(const uecda::common::CommunicationBody& src);

    /* 与えられたカードを手とみなしてサマリを作る。 */
    static HandSummary summarize(const Cards::bitcards src, const Cards::bitcards joker_src);

    /* ジョーカーなしで与えられた配列に指定された枚数の枚数組を作る。 */
    static void pushPair(const Cards::bitcards src, std::vector<Hand>& hand_vec, const int pair_qty);

    /* ジョーカー必ず込みで与えられた配列に指定された枚数の枚数組を作る。 */
    static void pushPairWithJoker(const Cards::bitcards src, std::vector<Hand>& hand_vec, const int pair_qty);

    /* ジョーカーなしで与えられた配列に指定された枚数の階段を作る。 */
    static void pushSequence(const Cards::bitcards src, std::vector<Hand>& hand_vec, int seq_qty);

    /* ジョーカー必ず込みで与えられた配列に指定された枚数の階段を作る。 */
    static void pushSequenceWithJoker(const Cards::bitcards src, std::vector<Hand>& hand_vec, const int seq_qty);

    friend std::ostream& operator<<(std::ostream& os, const Hand& src) {
      common::CommunicationBody body{};
      src.putCards(body);
      os << src.summary_;
      os << (common::CommunicationBody)body;
      return os;
    }
  };
}

#endif  // HANDS_HPP_
