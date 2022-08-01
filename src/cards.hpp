#ifndef CARDS_HPP_
#define CARDS_HPP_

#include <cstdint>
#include <vector>

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
  Cards(uint16_t src[8][15]);

  /* ビットカードから生成。 */
  Cards(bitcards src) { this->cards = src; }

  /* 与えられたビットカードの枚数を返す。 */
  static int count(bitcards src);

  /* カードを返す。 */
  bitcards getCard() { return this->cards; }

  /* カードに含まれるスートを4bit形式で返す。 */
  int getSuits();

  /* カードの枚数を返す。 */
  int quantity();

  /* ジョーカーが含まれるか否かを返す。 */
  int hasJoker() { return (this->cards >> 60) == 1; }

  /* 最弱のカードを15bit形式で返す。 */
  bitcards weakestOrder();

  /* 最強のカードを15bit形式で返す。 */
  bitcards strongestOrder();

 private:
  bitcards cards;
};

#endif  // CARDS_HPP_
