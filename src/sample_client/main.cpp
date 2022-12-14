#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#include "../cards.hpp"
#include "../hand.hpp"
#include "../table.hpp"
#include "../uecda_client.hpp"
#include "../uecda_common.hpp"
#include "select_hand.hpp"

using namespace uecda;

int main(int argc, char* argv[]) {
  bool is_round_end{false};
  bool is_game_end{false};

  /* ゲームに参加 */
  UECdaClient client{};
  client.enterGame();

  /* ラウンドの繰り返し */
  while (!is_game_end) {
    is_round_end = false;
    uecda::common::CommunicationBody dealt_body{};
    uecda::common::CommunicationBody table_body{};

    /* 交換前の手札を受け取る */
    client.receiveMyInitialCards(dealt_body);

    /* 交換 */
    if (dealt_body.at(5).at(0) == 0) {
      std::cerr << "ラウンド開始時ですが、カード交換フラグが立っていません。\n";
      return 1;
    }
    const unsigned int qty_to_change{dealt_body.at(5).at(1)};
    if (qty_to_change == 0) {
      /* 平民以下なので何もしない。 */
    } else if (qty_to_change <= 2 && qty_to_change > 0) {
      /* 手札を作る */
      const Cards my_cards{Cards::communicationBody2Cards(dealt_body)};
      std::vector<Hand> hands{};
      Hand::pushHands(my_cards, hands);

      /* 交換するカードを決める */
      const std::vector<Hand> submission_hands = select_change_hands(hands);

      /* 提出用配列に着手を移す */
      uecda::common::CommunicationBody submission_body1{};
      uecda::common::CommunicationBody submission_body2{};
      submission_hands.at(0).putCards(submission_body1);
      if (qty_to_change >= 2) {
        submission_hands.at(1).putCards(submission_body2);
      }
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 15; j++) {
          if (submission_body2.at(i).at(j) == 1) {
            submission_body1.at(i).at(j) = 1;
          }
        }
      }

      /* 交換用カードを提出 */
      client.sendExchangeCards(submission_body1);
    } else {
      std::cerr << "要求されたカード交換枚数が異常です: " << qty_to_change << std::endl;
      return 1;
    }

    /* トリックの繰り返し。 */
    while (!is_round_end) {
      /* 自分の手札を受け取る */
      client.receiveMyCards(dealt_body);
      /* 場の情報を取得 */
      Table table{dealt_body};

      /* 着手 */
      if (table.is_my_turn) {
        /* 手札・場の手を作る */

        Cards my_cards{Cards::communicationBody2Cards(dealt_body)};
        Hand table_hand{table.is_start_of_trick ? Hand() : Hand(Hand::communicationBody2Hand(table_body))};

        /* 手の候補を作る */
        std::vector<Hand> hands{};
        Hand::pushHands(my_cards, hands);

        /* 着手を決める。 */
        Hand submission_hand{select_hand(hands, table_hand, table)};

        /* 提出用配列に着手を移す */
        uecda::common::CommunicationBody submission_body{};
        submission_hand.putCards(submission_body);

        /* カードを提出 */
        const bool is_submit_accepted{client.sendSubmissionCards(submission_body)};
        if (!submission_hand.getSummary().is_pass && !is_submit_accepted) { // パスの場合も不受理判定になるので弾く。
          std::cerr << "提出カードが受理されませんでした。" << std::endl;
        }
      } else {
        /* 他プレイヤのターン時の行動を記述 */
      }

      /* 場札を受け取る */
      client.receiveTableCards(table_body);

      switch (client.receiveGameFinishState()) {
        case UECdaClient::GAME_FINISH_STATE::kContinue:
          is_round_end = false;
          is_game_end = false;
          break;
        case UECdaClient::GAME_FINISH_STATE::kRoundFinish:
          is_round_end = true;
          is_game_end = false;
          break;
        case UECdaClient::GAME_FINISH_STATE::kGameFinish:
          is_round_end = true;
          is_game_end = true;
          break;
      }
    }
  }

  client.exitGame();
  return 0;
}
