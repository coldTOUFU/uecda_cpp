#ifndef CARDS_HPP_
#define CARDS_HPP_

#include <unistd.h>
#include <iostream>
#include <vector>

#include "uecda_common.hpp"

class Cards {
 public:
  using bitcards = uint_fast64_t;

  enum class CARD_TYPES { kPair, kSequence };

  /* 整数は、UECda-2007における標準ルール下でのカード表現と対応している。 */
  static constexpr int kSpade = 0;
  static constexpr int kHeart = 1;
  static constexpr int kDiamond = 2;
  static constexpr int kClover = 3;

  /* 配列形式のカードから生成。 */
  Cards(uecda_common::CommunicationBody src);

  /* ビットカードから生成。 */
  Cards(bitcards src) { this->cards_ = src; }

  /* 与えられたビットカードの枚数を返す。 */
  static int count(bitcards src);

  /* カードを返す。 */
  bitcards getCard() { return this->cards_; }

  /* カードに含まれるスートを4bit形式で返す。 */
  int getSuits();

  /* カードの枚数を返す。 */
  int quantity();

  /* ジョーカーが含まれるか否かを返す。 */
  bool hasJoker() { return (this->cards_ >> 60) == 1; }

  /* 最弱のカードを15bit形式で返す。 */
  bitcards weakestOrder();

  /* 最強のカードを15bit形式で返す。 */
  bitcards strongestOrder();

  /* デバッグ出力用に配列にカードを埋め込む。 */
  void putCards(uecda_common::CommunicationBody dst);

  /* デバッグ用に与えられたカードを出力。 */
  static void printCards(uecda_common::CommunicationBody src);

 private:
  bitcards cards_;
};

#endif  // CARDS_HPP_
