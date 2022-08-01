#ifndef UECDA_CLIENT_HPP_
#define UECDA_CLIENT_HPP_

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#include "error/uecda_client_error.hpp"
#include "uecda_common.hpp"

class UECdaClient {
 public:
  enum class GAME_FINISH_STATE { kContinue, kRoundFinish, kGameFinish };

  UECdaClient(std::string pname = kDefaultPlayerName,
              int port = kDefaultPort,
              std::string sname = kDefaultServerHostname);

  /* サーバに接続してゲームに参加する。自分のプレーヤ番号を返す。 */
  int enterGame();

  void exitGame() {
    /* サーバとの接続を切る。 */
    try {
      closeSocket();
    } catch (const std::exception& e) {
      throw;
    }
  }

  /* サーバからラウンド開始時の手札を受け取る。 */
  void receiveMyInitialCards(uecda_common::CommunicationBody dst);

  /* カード交換時のカードの提出 */
  void sendExchangeCards(uecda_common::CommunicationBody cards);

  /* カードを受け取る。 */
  void receiveMyCards(uecda_common::CommunicationBody dst);

  /* カードを提出し受理されたか否かを返す */
  bool sendSubmissionCards(uecda_common::CommunicationBody src);

  //ラウンドの最後にゲームか終ったかを、サーバから受けとりその値を返す。
  GAME_FINISH_STATE receiveGameFinishState(void);

  /* 場札を受け取る */
  void receiveTableCards(uecda_common::CommunicationBody dst);

 private:
  static constexpr int kProtocolVersion = 20070;
  static const std::string kDefaultServerHostname;
  static constexpr int kDefaultPort = 42485;
  static const std::string kDefaultPlayerName;
  /* UECda-2007のプロトコルにおける、ゲーム終了フラグの設定と対応。
     http://www.tnlab.inf.uec.ac.jp/daihinmin/2021/document_protocol.htmlでは
     ゲーム継続時の値が明記されていないが、標準クライアントでは0の場合でswitchしているので、
     0で継続ということにする。*/
  static constexpr int kContinueCase = 0;
  static constexpr int kRoundFinishCase = 1;
  static constexpr int kGameFinishCase = 2;

  int sockfd_;
  struct sockaddr_in client_addr_;
  std::string server_hostname_;
  int port_;
  std::string player_name_;

  /* ソケットの初期化を行う。 */
  void openSocket();

  /* 通信を切断する。 */
  void closeSocket() {
    if (close(this->sockfd_) != 0) {
      std::cerr << "サーバ切断に失敗しました: " << std::endl;
      throw CloseSocketException();
    }
  }

  /* クライアントの情報を送信。 */
  void sendClientProfile();

  /* サーバからカードを受け取ってdst_tableに埋め込む。 */
  void receiveCommunicationBody(uecda_common::CommunicationBody dst);

  /* サーバからカードを受け取ってdst_tableに埋め込む。 */
  void sendCommunicationBody(uecda_common::CommunicationBody src);
};

#endif  // UECDA_CLIENT_HPP_
