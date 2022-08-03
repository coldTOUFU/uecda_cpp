#include <gtest/gtest.h>

#include "../../src/cards.hpp"

TEST(CountTest, Empty) {
  EXPECT_EQ(0, Cards::count(0));
}

TEST(CountTest, Normal) {
  Cards::bitcards src = 0b0000010000000000000001000000000000000100000000000000010000000;
  EXPECT_EQ(4, Cards::count(src));
}

TEST(CountTest, Full) {
  /* 使う場所をすべて埋めた状態。 */
  Cards::bitcards src = 0b1111111111111111111111111111111111111111111111111111111111111;
  EXPECT_EQ(61, Cards::count(src));
}

TEST(GetCardTest, Empty) {
  uecda_common::CommunicationBody src = {{}};
  Cards *cards = new Cards(src);

  EXPECT_EQ(0, cards->getCard());
  delete cards;
}

TEST(GetCardTest, Normal) {
  uecda_common::CommunicationBody src = {
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);
  Cards::bitcards dst = 0b0000010000000000000001000000000000000100000000000000010000000;

  EXPECT_EQ(dst, cards->getCard());
  delete cards;
}

TEST(GetCardTest, Full) {
  /* 使う場所をすべて埋めた状態。 */
  uecda_common::CommunicationBody src = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);
  Cards::bitcards dst = 0b1111111111111111111111111111111111111111111111111111111111111;

  EXPECT_EQ(dst, cards->getCard());
  delete cards;
}

TEST(GetSuits, Empty) {
  uecda_common::CommunicationBody src = {{}};
  Cards *cards = new Cards(src);

  EXPECT_EQ(0, cards->getSuits());
  delete cards;
}

TEST(GetSuits, OneSpade) {
  uecda_common::CommunicationBody src = {
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);

  EXPECT_EQ(0b1000, cards->getSuits());
  delete cards;
}

TEST(GetSuits, OneHeart) {
  uecda_common::CommunicationBody src = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);

  EXPECT_EQ(0b0100, cards->getSuits());
  delete cards;
}

TEST(GetSuits, OneDiamond) {
  uecda_common::CommunicationBody src = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);

  EXPECT_EQ(0b0010, cards->getSuits());
  delete cards;
}

TEST(GetSuits, OneClover) {
  uecda_common::CommunicationBody src = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);

  EXPECT_EQ(0b0001, cards->getSuits());
  delete cards;
}

TEST(GetSuits, Two) {
  uecda_common::CommunicationBody src = {
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);

  EXPECT_EQ(0b1010, cards->getSuits());
  delete cards;
}

TEST(GetSuits, Three) {
  uecda_common::CommunicationBody src = {
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);

  EXPECT_EQ(0b1110, cards->getSuits());
  delete cards;
}

TEST(GetSuits, All) {
  uecda_common::CommunicationBody src = {
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);

  EXPECT_EQ(0b1111, cards->getSuits());
  delete cards;
}

TEST(GetSuits, Full) {
  /* 使う場所をすべて埋めた状態。 */
  uecda_common::CommunicationBody src = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);

  EXPECT_EQ(0b1111, cards->getSuits());
  delete cards;
}

TEST(QuantityTest, Empty) {
  uecda_common::CommunicationBody src = {{}};
  Cards *cards = new Cards(src);

  EXPECT_EQ(0, cards->quantity());
  delete cards;
}

TEST(QuantityTest, Normal) {
  uecda_common::CommunicationBody src = {
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);

  EXPECT_EQ(4, cards->quantity());  
  delete cards;
}

TEST(QuantityTest, Full) {
  /* 使う場所をすべて埋めた状態。 */
  uecda_common::CommunicationBody src = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);

  EXPECT_EQ(61, cards->quantity());
  delete cards;
}

TEST(HasJokerTest, False) {
  uecda_common::CommunicationBody src = {
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);

  EXPECT_FALSE(cards->hasJoker());
  delete cards;
}

TEST(HasJokerTest, True) {
  uecda_common::CommunicationBody src = {
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);

  EXPECT_TRUE(cards->hasJoker());
  delete cards;
}

TEST(WeakestOrderTest, Empty) {
  uecda_common::CommunicationBody src = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);

  EXPECT_EQ(0, cards->weakestOrder());
  delete cards;
}

TEST(WeakestOrderTest, Weakest) {
  uecda_common::CommunicationBody src = {
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);
  
  EXPECT_EQ(0b100000000000000, cards->weakestOrder());
  delete cards;
}

TEST(WeakestOrderTest, Normal) {
  uecda_common::CommunicationBody src = {
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);
  
  EXPECT_EQ(0b000010000000000, cards->weakestOrder());
  delete cards;
}

TEST(WeakestOrderTest, Strongest) {
  uecda_common::CommunicationBody src = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);
  
  EXPECT_EQ(0b000000000000001, cards->weakestOrder());
  delete cards;
}

TEST(StrongestOrderTest, Empty) {
  uecda_common::CommunicationBody src = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);

  EXPECT_EQ(0, cards->strongestOrder());
  delete cards;
}

TEST(StrongestOrderTest, Weakest) {
  uecda_common::CommunicationBody src = {
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);

  EXPECT_EQ(0b100000000000000, cards->strongestOrder());
  delete cards;
}

TEST(StrongestOrderTest, Normal) {
  uecda_common::CommunicationBody src = {
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);
  
  EXPECT_EQ(0b000000010000000, cards->strongestOrder());
  delete cards;
}

TEST(StrongestOrderTest, Strongest) {
  uecda_common::CommunicationBody src = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);

  EXPECT_EQ(0b000000000000001, cards->strongestOrder());
  delete cards;
}

TEST(PutCardsTest, Empty) {
  uecda_common::CommunicationBody src = {{}};
  Cards *cards = new Cards(src);
  uecda_common::CommunicationBody result = {{}};
  cards->putCards(result);

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 15; j++) {
      EXPECT_EQ(src[i][j], result[i][j]);
    }
  }
  delete cards;
}

TEST(PutCardsTest, Normal) {
  uecda_common::CommunicationBody src = {
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
  Cards *cards = new Cards(src);
  uecda_common::CommunicationBody result = {{}};
  cards->putCards(result);

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 15; j++) {
      EXPECT_EQ(src[i][j], result[i][j]);
    }
  }
  delete cards;
}

TEST(PutCardsTest, Full) {
  /* 使う場所をすべて埋めた状態。 */
  uecda_common::CommunicationBody src = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  Cards *cards = new Cards(src);
  uecda_common::CommunicationBody result = {{}};
  cards->putCards(result);

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 15; j++) {
      EXPECT_EQ(src[i][j], result[i][j]);
    }
  }
  delete cards;
}
