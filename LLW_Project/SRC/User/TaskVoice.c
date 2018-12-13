#include ".\Lib\includes.h"
u16 const VoiceDelLen[VOICE_MAX]={ 
180,//0 VOICE_INSERTCOIN
200,//1 VOICE_PLEASECOIN
600,//2 VOICE_PLEASE2COIN
600,//3 VOICE_PLEASE3COIN
600,//4 VOICE_PLEASE4COIN
300,//5 VOICE_PRESSSTARTKEY
800,//6 VOICE_GETBALL
1,//7
1,//8
1,//9
300,//10 VOICE_TICKETERROR
1010,//11 VOICE_CONNECTSERVER
1,//12
1,//13
1,//14
1,//15
1,//16
340,//17 VOICE_GETSECONDPRIZE
150,//18 VOICE_WISHYOU
400,//19 VOICE_GETFIRSTPRIZE
340,//20 VOICE_BALLOVER
1,//21
1,//22
400,//23 VOICE_TIMEOVER
240,//24 VOICE_GETTHIRDPRIZE
1,//25
1100,//26 VOICE_TRYTOPLAY
1,//27
1,//28
1,//29
1,//30
50,//31 VOICE_INBALL
21000,//32	 VOICE_MUSIC
920,//33 VOICE_WHISTLE handclap whistle
1300,//34 VOICE_HANDCLAP
0,//35 VOICE_RELOAD
0,//36 VOICE_ONESHOT
0,0,0,0,0,0,0,0,0,
7070,//46 VOICE_HORSEFOR 7270
8610,//47 VOICE_HORSEMID 8610
3410,//48 VOICE_HORSEBACK 3410
1360,//49 VOICE_HORSEOVER 1360
320,//50
340,//51
210,//52
190,//53
220,//54
460,//55
245,//56
235,//57
260,//58
310,//59
340,//60
270,//61
200,//62
265,//63
285,//64
210,//65
200,//66
230,//67
535,//68
550,//69
560,//70
250,//71
200,//72
255,//73
735,//74
600,//75
500,//76
600,//77
505,//78
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//106
  370,//107
  150,//108
  0,//109
  0,//110
  0,//111
  0,//112
  0,//113
  0,//114
  0,//115
  600,//116
  700,//117
	200,//0x76 118
	240,
	200,
	200,
	300,
	200,
	250,
	230,
	260,
	120,
	120,
	120,
	120,
	120,
	120,
	120,
	120,
	120,
	120,
	120,
	120,
	120,
	120,
	120,
	120,
};
//语音任务
void vVoiceTask( void *pvParameters )
{
  WTV020ASend(VOICE_THANKUSE);
  vTaskDelay(1800);
  WTV020ASend(VOICE_RONGXIANGMACHINE);
  vTaskDelay(2000);
  for(;;)
  {
    if(Coin.InsertFlag==1)
    {
      Coin.InsertFlag=0;
      WTV020ASend(VOICE_INSERTCOIN);
    }
//    if(Game.StartKeySound==0)
//    {
//      #if LANGUAGE_CN
//      VoiceASendOrder(VOICE_PRESSSTARTKEY);//播放按开始键声音
//      #else
//      VoiceASendOrder(0x33);//播放按开始键声音
//      #endif
//      Game.StartKeySound=1;
//    }
    vTaskDelay(100);
  }
}



