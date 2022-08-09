#include "cards.hpp"

Cards::Cards(uecda_common::CommunicationBody src) {
  this->cards_ = 0;

  /* Joker。 */
  if (src[4][1] == 2) {
    this->cards_++;
  }
  this->cards_ <<= 1;

  /* Joker以外の各札。 */
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 15; j++) {
      if (src[i][j] == 1) {
        this->cards_++;
      }
      this->cards_ <<= 1;
    }
  }

  /* 上で余計に1回左シフトした分を戻す。 */
  this->cards_ >>= 1;
}

int Cards::getSuits() {
  bitcards tmp = this->cards_;
  tmp &= 0xfffffffffffffff; // Jokerをビット列から落とす。

  int s = 0;

  for (int i = 0; i < 4; i++) {
    bitcards filter = (0xffff >> 1); // 1-15ビット目が1。
    filter <<= 15 * (3 - i); // 目的のスートのカードのみフィルタリングできるようにシフトする。
    s += (tmp & filter) != 0;
    s <<= 1;
  }
  s >>= 1; // 一回余分にシフトした分を戻す。
  return s;
}

int Cards::quantity() {
  return this->count(this->cards_);
}

int Cards::count(bitcards src) {
  /*
    32bitsの場合の例。
      tmp = 0b10010010100100011001001010110001
    とする。
      tmp = (tmp & 0x5555555555555555) + (tmp >> 1 & 0x5555555555555555)
    の結果は、
      1010001010100010101000101100001
    となる。
    元のtmpと比べると、
      10|01|00|10|10|01|00|01|10|01|00|10|10|11|00|01
      01|01|00|01|01|01|00|01|01|01|00|01|01|10|00|01
    のように、2bits区切で1を数えているのがわかる。
  */
  bitcards tmp = src;
  tmp = (tmp & 0x5555555555555555) + (tmp >> 1 & 0x5555555555555555);   // 2bits区切でビット数を数える。
  tmp = (tmp & 0x3333333333333333) + (tmp >> 2 & 0x3333333333333333);   // 4bits区切。
  tmp = (tmp & 0x0f0f0f0f0f0f0f0f) + (tmp >> 4 & 0x0f0f0f0f0f0f0f0f);   // 8bits。
  tmp = (tmp & 0x00ff00ff00ff00ff) + (tmp >> 8 & 0x00ff00ff00ff00ff);   // 16bits。
  tmp = (tmp & 0x0000ffff0000ffff) + (tmp >> 16 & 0x0000ffff0000ffff);  // 32bits。
  return (tmp & 0x00000000ffffffff) + (tmp >> 32 & 0x00000000ffffffff); // 64bits。
}

Cards::bitcards Cards::weakestOrder() {
  bitcards tmp = this->cards_;
  tmp &= 0xfffffffffffffff; // Jokerをビット列から落とす。

  /*
    一番左(弱い)の1がどのスートの位置にあるかわからないので、
    15ずつずらして論理和をとり、2^0から2^14の範囲内に収める。
    一番左の1がクローバー以外のスートにある場合は2^15以降の範囲に
    1が立っているので、これを落とす。
  */
  tmp = (tmp | (tmp >> 15) | (tmp >> 30) | (tmp >> 45)) & (0xffff >> 1);

  /* 一番左の1が立っているビットから右をすべて1にする。 */
  tmp |= (tmp >> 1); // 1が1つ右隣に伝播する(eg. 01000000 -> 01100000)。
  tmp |= (tmp >> 2); // 1が2つ右隣に伝播する(eg. 01100000 -> 01111000)。
  tmp |= (tmp >> 4); // 1が4つ右隣に伝播する。
  tmp |= (tmp >> 8);

  /* 一番左の1以外を落とす。 */
  tmp ^= (tmp >> 1);

  return tmp;
}

Cards::bitcards Cards::strongestOrder() {
  bitcards tmp = this->cards_;
  tmp &= 0xfffffffffffffff; // Jokerをビット列から落とす。

  /*
    一番右(強い)の1がどのスートの位置にあるかわからないので、
    15ずつずらして論理和をとり、2^0から2^14の範囲内に収める。
    一番右の1がクローバー以外のスートにある場合は2^15以降の範囲に
    1が立っているので、これを落とす。
  */
  tmp = (tmp | (tmp >> 15) | (tmp >> 30) | (tmp >> 45)) & (0xffff >> 1);

  /* 一番右のbitだけ立てて返す。 */
  return tmp & (-tmp);
}

void Cards::putCards(uecda_common::CommunicationBody dst) {
  bitcards src = this->cards_;

  /* Joker以外の各札。 */
  for (int i = 3; i >= 0; i--) {
    for (int j = 14; j >= 0; j--) {
      dst[i][j] = src % 2;
      src >>= 1;
    }
  }
  /* Joker。 */
  dst[4][1] = (src % 2) * 2;
}


void Cards::printCards(uecda_common::CommunicationBody src) {
  std::cout << "-----------------------------------------------------------------------------\n"
               " |  * |  3 |  4 |  5 |  6 |  7 |  8 |  9 | 10 |  J |  Q |  K |  1 |  2 |  * |\n"
               "-----------------------------------------------------------------------------\n";
  for (int suit = 0; suit < 5; suit++) {
    switch (suit) {
      case Cards::kSpade:
        std::cout << "S|";
        break;
      case Cards::kHeart:
        std::cout << "H|";
        break;
      case Cards::kDiamond:
        std::cout << "D|";
        break;
      case Cards::kClover:
        std::cout << "C|";
        break;
      default:
        std::cout << "*|";
        break;
    }
    for (int order = 0; order < 15; order++) {
      std::cout << "  ";
      if (src[suit][order] == 1) {
        std::cout << 1;
      } else if (src[suit][order] == 2) {
        std::cout << 2;
      } else {
        std:: cout << " ";
      }
      std::cout << " |";
    }
    std::cout << std::endl;
    std::cout << "-----------------------------------------------------------------------------" << std::endl;
  }
}
