#ifndef    _SERIAL_COMM_H_
#define    _UDP_COMM_H_

// 受信バッファサイズ
#define SERIAL_COMM_BUFF_SIZE 256

// シリアル通信処理クラス
class SerialComm
{
public:
    SerialComm();
    // 開始する
    void begin();
    // 周期処理 loop()で必ず呼び出す
    void loop();
    // データを送信する
    void send(char* data);
    
    // 受信コールバック
    void (*onReceive)(char* data);
    
private:
    // 受信状態
    int state;
    // 受信データのバッファ
    char buff[SERIAL_COMM_BUFF_SIZE];
    int ptr;
};

#endif
