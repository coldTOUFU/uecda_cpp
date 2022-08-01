## 各ソースの説明
外部から見える(publicな)関数や変数について、ユーザ側でどう扱うかについて説明します。

- 「使い道(ユーザ)」と表記されるものは、ユーザに対して想定されている使い道です。
- 「使い道(内部)」と表記されるものは、このライブラリ内のプログラムに想定されている使い道です。
- 「使い道(ユーザ)」の表記がない場合、それにユーザがアクセスすることは想定されていません。

### cards
カード集合を、配列よりも高速な表現で扱うCardsクラスが定義されています。
ユーザは、自分の手札をこのインスタンスで保持できます。
ユーザに想定されるのは、手札配列を渡してインスタンスを作成し、必要に応じてHandクラスに渡すことのみです。

`Cards(int src[8][15])`
使い道(ユーザ): 自分の手札からCardsインスタンスを作成。

`Cards(bitcards src)`
使い道(内部): Handでビットカード表現からCardsインスタンスを作成。

`static int count(bitcards src)`
使い道(内部): CardsやHandでカード枚数を数える。

`bitcards getCard()`
使い道(内部): Handで役生成のためビットカード表現を得る。

`int getSuits()`
使い道(内部): Handでカードのスート情報を得る。

`int quantity()`
使い道(内部): Handでカードの枚数を得る。

`int hasJoker()`
使い道(内部): Cardsが手札集合を表している場合に、Handでジョーカーの有無を得る。

`bitcards weakestOrder()`
使い道(内部): Handで最弱のカードの位置を得る。

`bitcards strongestOrder()`
使い道(内部): Handで最強のカードの位置を得る。

### hand
役として成り立つカード集合をCardsインスタンスで保持するHandクラスが定義されています。
Cardsクラスとの違いは、Cardsが配列表現のみから得られる情報しか扱わないのに対し、
Handクラスでは大貧民のルールに基づく情報を扱える点にあります。

`Hand(int src[8][15])`
使い道(ユーザ): 合法手であることの判定などに用いるため、場の手からHandインスタンスを作成。

`Hand(Cards::bitcards src, Cards::bitcards joker_src, HandSummary *hs)`
使い道(内部): Handで与えられたカード集合からHandインスタンスを生成。

`bool isLegal(Table *tbl, Hand *table_hand)`
使い道(ユーザ): この手が場において合法であるかを判定。

`HandSummary getSummary()`
使い道(ユーザ): この手のサマリを取得。

`static void pushHands(Cards *src, std::vector<Hand*> *hand_vec)`
使い道(ユーザ): 手札から生成したCardsインスタンスをもとに、手の集合を配列に書き込む。このとき合法かは問わない。

`void putCards(int dst[8][15])`
使い道(ユーザ): この手を通信に用いられるカード表現として配列に書き込む。

### hand_summary
手のサマリを保持する構造体です。ユーザは、この構造体によって実際のカード集合を見ることなく手の基本的な情報が得られます。

`int quantity`
カードの枚数。

`CARD_TYPES card_type`
カードのタイプ。`kPair`は枚数組、`kSequence`は階段組を意味する。

`Cards::bitcards weakest_order`
最弱のカードの位置。位置は16bit表現内に見いだされる。1bit目が最も強く、16bitに近づくにつれ弱くなる。強さは常に革命でないときに基づく。

`Cards::bitcards strongest_order`
最強のカードの位置。

`bool has_joker`
ジョーカーを含むかどうか。

`int suits`
含まれるスート。4bit表現内に見いだされる。4bit目はスペード、3bit目はハート、2bit目はダイヤモンド、1bit目はクローバーに対応する。

### table
場の情報を配列から読み込み、保持するクラス。

`Table(uint16_t src[8][15])`
使い道(ユーザ): サーバから送られた場の情報を含む配列から、
必要な情報を読み取る。

`bool getIsMyTurn()`
使い道(ユーザ): 現在自分のターンかどうかを取得。

`bool getIsStartOfTrick()`
使い道(内部): Handインスタンスの合法判定。
使い道(ユーザ): 現在がトリックの最初か(場にカードがないか)どうかを取得。

`bool getIsLock()`
使い道(内部): Handインスタンスの合法判定。
使い道(ユーザ): 現在場が縛られているかどうかを取得。

`bool getIsRev()`
使い道(内部): Handインスタンスの合法判定。
使い道(ユーザ): 現在革命状態かどうかを取得。

### uecda_client
サーバとの通信を仲介するクラス。

`GAME_FINISH_STATE`
現在のゲームの終了に関する状態を管理するための列挙体。
`kContinue`はラウンドが続いていることを、`kRoundFinish`はラウンドが終わったことを、`kGameFinish`はゲームが終わったことを表す。

`UECdaClient(std::string pname, int port, std::string sname)`
使い道(ユーザ): プレイヤ名、ポート番号、サーバアドレスを指定して初期化する。ローカルで単一のサーバを立てて実行する分にはデフォルト値だけで十分。

`void enterGame()`
使い道(ユーザ): サーバに接続する。返り値として自分のプレイヤ番号を返すので、座席の位置関係などを把握するのに利用できる。

`void exitGame()`
使い道(ユーザ): サーバとの接続を終える。

`void receiveMyInitialCards(CommunicationBody dst)`
使い道(ユーザ): サーバからラウンド開始時の手札を受け取る。内容は交換実行前のものになっている。

void sendExchangeCards(CommunicationBody cards);
使い道(ユーザ): 交換するカードを提出する。富豪以上の場合のみ実行する。

void receiveMyCards(CommunicationBody dst);
使い道（ユーザ): サーバから自分の手札を受け取る。

bool sendSubmissionCards(CommunicationBody src);
使い道（ユーザ): カードを提出し受理されたか否かを受け取る。

GAME_FINISH_STATE receiveGameFinishState(void);
使い道(ユーザ): ラウンドの最後にゲームか終ったかをサーバから受けとりその値を返す。

`void receiveTable(CommunicationBody dst)`
使い道(ユーザ): 場札を受け取る。