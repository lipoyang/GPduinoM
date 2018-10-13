// UDP通信処理クラス
#include <string.h>
#include <Arduino.h>
#include "SerialComm.h"

// シリアルコマンド電文の受信処理

// 0x02/0x03だとデバッグしにくいので、#/$ を 電文開始/終了 に使う
// 電文開始
#define CODE_STX '#'
// 電文終了
#define CODE_ETX '$'

// 電文開始待ち状態
#define STATE_READY     0
// 電文受信中状態
#define STATE_RECEIVING 1

// コンストラクタ
SerialComm::SerialComm()
{
    onReceive = NULL;
    ptr = 0;
}

// 開始する
void SerialComm::begin()
{
    // シリアルポートを開く
    Serial.begin(115200);
}

// 周期処理 loop()で必ず呼び出す
void SerialComm::loop()
{
    char c;
    
    /* シリアル受信データがあるか？ */
    while (Serial.available() > 0)
    {
        //Serial.println("RECV ");
        c = Serial.read();
        switch(state)
        {
        /* 電文開始待ち状態 */
        case STATE_READY:
            /* 電文開始コードが来たら電文受信中状態へ */
            if(c == CODE_STX)
            {
                //Serial.println("STX ");
                state = STATE_RECEIVING;
                ptr = 0;
            }
            break;
        /* 電文受信中状態 */
        case STATE_RECEIVING:
            /* もしも電文開始コードが来たら受信中のデータを破棄 */
            if(c == CODE_STX)
            {
                //Serial.println("STX ");
                ptr = 0;
            }
            /* 電文終了コードが来たら、受信した電文のコマンドを実行 */
            else if(c == CODE_ETX)
            {
                //Serial.println("ETX ");
                buff[ptr] = '\0';
                // コールバック
                if(onReceive != NULL) onReceive(buff);
                state = STATE_READY;
            }
            /* 1文字受信 */
            else
            {
                buff[ptr] = c;
                ptr++;
                if(ptr>=SERIAL_COMM_BUFF_SIZE)
                {
                    state = STATE_READY;
                }
            }
            break;
        default:
            state = STATE_READY;
        }
    }
}

// データを送信する
void SerialComm::send(char* data)
{
    Serial.write(data, strlen(data));
}
