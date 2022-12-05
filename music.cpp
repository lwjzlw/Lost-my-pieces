#include <iostream>
#include <Windows.h>
#pragma comment(lib,"winmm.lib")
#include <thread>
#define SPEED 420 //规定SPEED毫秒为半拍 
using namespace std;
HMIDIOUT handle;
enum Scale
{
	Rest = 0, C8 = 108, B7 = 107, A7s = 106, A7 = 105, G7s = 104, G7 = 103, F7s = 102, F7 = 101, E7 = 100,
	D7s = 99, D7 = 98, C7s = 97, C7 = 96, B6 = 95, A6s = 94, A6 = 93, G6s = 92, G6 = 91, F6s = 90, F6 = 89,
	E6 = 88, D6s = 87, D6 = 86, C6s = 85, C6 = 84, B5 = 83, A5s = 82, A5 = 81, G5s = 80, G5 = 79, F5s = 78,
	F5 = 77, E5 = 76, D5s = 75, D5 = 74, C5s = 73, C5 = 72, B4 = 71, A4s = 70, A4 = 69, G4s = 68, G4 = 67,
	F4s = 66, F4 = 65, E4 = 64, D4s = 63, D4 = 62, C4s = 61, C4 = 60, B3 = 59, A3s = 58, A3 = 57, G3s = 56,
	G3 = 55, F3s = 54, F3 = 53, E3 = 52, D3s = 51, D3 = 50, C3s = 49, C3 = 48, B2 = 47, A2s = 46, A2 = 45,
	G2s = 44, G2 = 43, F2s = 42, F2 = 41, E2 = 40, D2s = 39, D2 = 38, C2s = 37, C2 = 36, B1 = 35, A1s = 34,
	A1 = 33, G1s = 32, G1 = 31, F1s = 30, F1 = 29, E1 = 28, D1s = 27, D1 = 26, C1s = 25, C1 = 24, B0 = 23,
	A0s = 22, A0 = 21
};
//音节参数
enum Voice
{
	X1 = C2, X2 = D2, X3 = E2, X4 = F2, X5 = G2, X6 = A2, X7 = B2,
	L1 = C3, L2 = D3, L3 = E3, L4 = F3, L5 = G3, L6 = A3, L7 = B3,
	M1 = C4, M2 = D4, M3 = E4, M4 = F4, M5 = G4, M6 = A4, M7 = B4,
	H1 = C5, H2 = D5, H3 = E5, H4 = F5, H5 = G5, H6 = A5, H7 = B5,
	Y1 = C6, Y2 = D6, Y3 = E6, Y4 = F6, Y5 = G6, Y6 = A6, Y7 = B6,
	_ = 0XFF
};
//简化表示


int ans = 1; int ans2 = 1000;

int gao = 0x5f;//音量高
int ggao = 0x7f;
int zhong = 0x5a;//音量中
int gzhong = 0x7a;
int voice = 0x0;

int sleep = SPEED;

int sta = SPEED;

int stb = SPEED;

void Th0()
{
	int tmp = 0;
	int vol = gao;
	int th0[] = { 300,H1,M5,M4,450,H1,400,H2,	600,M7,		300,M7,H1,M7,M6,M5,M4,M5,M6,	550,M2,400,M5,	
	500,M3,400,M4,M6,	500,H2,M6,	M7,M6+1,	600,M7,		400,M3,M4,M5,M2,
		M3,M4,M5,M6,	450,M7,300,M5,500,H2,	M6,20000,400,H7,Y1,		300,Y2,Y3,Y2,Y5,Y4,Y2,H6,Y3,	500,Y2,400,H6,Y1,
		300,H7,Y1,H7,Y3,Y2,H5,H6,Y1,	500,H7,400,H5,H2,	300,Y2,Y3,Y2,Y5,Y4,Y2,H6,Y3,	400,Y2,Y5,Y6,Y7+1,	500,Y7,Y6,Y6
	};
	for (auto i : th0)

	{

		
			if (i == 0) { sleep = sta / 2; continue; } //换节奏为四分之一拍 

			if (i == 300) { sleep = sta; continue; } //换节奏为半拍 

			if (i == 400) { sleep = sta*2; continue; } //换节奏为一拍
			if (i == 450) { sleep = sta * 3; continue; }
			if (i == 500) { sleep=sta*4; continue; }//两拍
			if (i == 550) { sleep = sta * 6; continue; }
			if (i == 600) { sleep=sta*8; continue; }//四拍

			if (i == 700) { midiOutShortMsg(handle, 0x7BB0); Sleep(sta / 2); continue; }//休眠四分之一拍 
			if (i == 800) { midiOutShortMsg(handle, 0x7BB0); Sleep(sta * 2); continue; }
			if (i == 1000) { midiOutShortMsg(handle, 0x7BB0); Sleep(sta * 8); continue; }
			if (i == _) { midiOutShortMsg(handle, 0x7BB0); Sleep(stb); continue; }//休眠半拍 
			if (i == 20000) { vol = ggao; continue; }
			if (i == 10000) { vol = gao; continue; }
		//过渡操作

		voice = (vol << 16) + ((i + tmp) << 8) + 0x90;//赋值语句，决定音量，音调和声道；
	    
		midiOutShortMsg(handle, voice);


		cout << ans << endl; ans++;

		Sleep(sleep);
	}
	midiOutClose(handle);
}

int main()

{

	midiOutOpen(&handle, 0, 0, 0, CALLBACK_NULL);
	midiOutShortMsg(handle, 2 << 8 | 0xC0); //通道0的乐器为 

	thread t0(Th0);

	t0.detach();

	while (1);



	return 0;

} 
