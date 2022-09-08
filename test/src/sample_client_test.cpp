#include <gtest/gtest.h>

#include <fstream>
#include <iostream>

#include "../../src/cards.hpp"
#include "../../src/hand.hpp"
#include "../../src/sample_client/select_hand.hpp"
#include "../../src/table.hpp"
#include "../../src/uecda_common.hpp"

using namespace uecda;

/* defaultが100回対戦したときの状況を再現し、同じ手を出すか確認。 */
TEST(SelectHandTest, Game100) {
  std::ifstream read_fstream("test/data/default_read.csv");
  std::ifstream write_fstream("test/data/default_write.csv");
  ASSERT_TRUE(read_fstream.is_open());
  ASSERT_TRUE(write_fstream.is_open());

  /* プレイヤ番号受け取り */
  int my_playernum;
  read_fstream >> my_playernum;

  int my_turn_cnt;
  common::CommunicationBody dealt_body = {};
  common::CommunicationBody table_body = {};

  while (!read_fstream.eof()) {
    /* 手札受け取り */
    read_fstream >> my_turn_cnt;
    std::cout << my_turn_cnt << "ターン目(read側)" << std::endl;
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 15; j++) {
        read_fstream >> dealt_body[i][j];
      }
    }

    Table table = Table(dealt_body);
    if (table.is_my_turn) {
      std::cout << "場の状態" << std::endl;
      table.print();
      std::cout << "手札" << std::endl;
      std::cout << dealt_body;
      std::cout << "場札" << std::endl;
      std::cout << table_body;
      /* 手札・場の手を作る */
      Cards my_cards = Cards(dealt_body);
      Hand table_hand;
      if (table.is_start_of_trick) {
        table_hand = Hand();
      } else {
        table_hand = Hand(table_body);
      }

      /* 手の候補を作る */
      std::vector<Hand> hands;
      Hand::pushHands(my_cards, hands);

      /* 着手を決める。 */
      Hand submission_hand = select_hand(hands, table_hand, table);

      /* 提出用配列に着手を移す */
      uecda::common::CommunicationBody submission_body = {};
      submission_hand.putCards(submission_body);

      /* defaultの実際の着手。 */
      write_fstream >> my_turn_cnt;
      std::cout << my_turn_cnt << "ターン目(write側)" << std::endl;
      common::CommunicationBody dst_body = {};
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 15; j++) {
          write_fstream >> dst_body[i][j];
        }
      }

      std::cout << "defaultの手" << std::endl;
      std::cout << dst_body;

      std::cout << "実際の手" << std::endl;
      std::cout << submission_body;

      /* ジョーカーを含む場合、ジョーカー以外の位置でみる */
      if (submission_hand.getSummary().has_joker && Hand(dst_body).getSummary().has_joker) {
        for (int i = 0; i < 8; i++) {
          for (int j = 0; j < 15; j++) {
            if (dst_body[i][j] == 2) {
              dst_body[i][j] = 0;
            }
            if (submission_body[i][j] == 2) {
              submission_body[i][j] = 0;
            }
          }
        }
      }

      /* 190ターン目はdefaultの手が非合法なので飛ばす。 */
      if (my_turn_cnt != 190) {
        ASSERT_EQ(dst_body, submission_body);
      }
    }

    /* 場のカード受け取り */
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 15; j++) {
        read_fstream >> table_body[i][j];
      }
    }
  }

  read_fstream.close();
  write_fstream.close();
}
