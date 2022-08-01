#ifndef TABLE_CPP_
#define TABLE_CPP_

#include <cstdint>

class Table {
 public:
  Table(uint16_t src[8][15]) {
    this->is_my_turn = src[5][2];
    this->is_start_of_trick = src[5][4];
    this->is_lock = src[5][7];
    this->is_rev = src[5][6];
    for (int i = 0; i < 4; i++) {
      this->is_out[i] = src[6][i] == 0;
    }
  }
  bool getIsMyTurn() { return this->is_my_turn; }
  bool getIsStartOfTrick() { return this->is_start_of_trick; }
  bool getIsLock() { return this->is_lock; }
  bool getIsRev() { return this->is_rev; }

 private:
  bool is_my_turn;
  bool is_start_of_trick;
  bool is_lock;
  bool is_rev;
  bool has_passed[4]; // 各プレイヤがパスをしたか。
  bool is_out[4]; // 各プレイヤがあがったか。
};

#endif // TABLE_CPP_
