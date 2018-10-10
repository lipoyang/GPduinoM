#include "common.h"
#include "UdpComm.h"

// UDP通信クラス
UdpComm udpComm;
// UDP受信コールバック
void udpComm_callback(char* buff);

// 送信バッファ
static char txbuff[256];

/**
 * バッテリー電圧チェック
 */
void battery_check()
{
    if(!udpComm.isReady()) return;
    
    static int cnt1 = 0;
    static int cnt2 = 0;
    
    // 100msecごとに電圧値測定
    cnt1++;
    if(cnt1 < 100) return;
    cnt1 = 0;

    unsigned short Vbat_ave = 573; // 573 = 3.7V

    // 1秒ごとに電圧値送信
    cnt2++;
    if(cnt2 >= 10)
    {
        cnt2=0;
        
        txbuff[0]='#';
        txbuff[1]='B';
        Uint16ToHex(&txbuff[2], Vbat_ave, 3);
        txbuff[5]='$';
        txbuff[6]='\0';
        udpComm.send(txbuff);
    }
}

// 初期設定
void setup() {
    Serial.begin(115200);
    //Serial.println("setup() start!");
    
    // UDP通信の設定
    udpComm.beginAP(NULL, "12345678");
    udpComm.onReceive = udpComm_callback;
}

// メインループ
void loop() {
    
    // UDP通信
    udpComm.loop();
    // バッテリー電圧チェック
    battery_check();
    
    delay(1);
}


// UDP command received callback
// buff: command string buffer
void udpComm_callback(char* buff)
{
    Serial.write(buff, strlen(buff));
}
