#include "common.h"
#include "UdpComm.h"
#include "SerialComm.h"

// UDP通信クラス
UdpComm udpComm;
// UDP受信コールバック
void udpComm_callback(char* buff);

// シリアル通信クラス
SerialComm serialComm;
// シリアル受信コールバック
void serialComm_callback(char* buff);

// 初期設定
void setup()
{
    // シリアル通信の設定
    serialComm.begin();
    serialComm.onReceive = serialComm_callback;
    
    // UDP通信の設定
    udpComm.beginAP(NULL, "12345678");
    udpComm.onReceive = udpComm_callback;
}

// メインループ
void loop()
{
    // UDP通信
    udpComm.loop();
    // シリアル通信
    serialComm.loop();
}

// UDP command received callback
// buff: command string buffer
void udpComm_callback(char* buff)
{
    serialComm.send(buff);
}

// Serial command received callback
// buff: command string buffer
void serialComm_callback(char* buff)
{
    udpComm.send(buff);
}
