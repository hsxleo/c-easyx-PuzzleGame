#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")//音频


#define BK_WIDTH 800
#define BK_HEIGHT 500
#define IMG_SIZE 100


IMAGE menubackground;
IMAGE gamebackground;
IMAGE game;
IMAGE imgArr[10];
clock_t  start, end;	   //游戏计时变量

int arr[4][4]; //存放3*3的图片

int record[100] = {0}; //存放记录


void menuMouse(); //游戏菜单界面

//读取最高纪录文件
void ReadHighRecordFile ();
//保存最高纪录文件
void SaveHighRecordFile ();
//绘制最高记录界面
void DrawHighScore ();

//加载资源
void loadResource() {
	loadimage(&menubackground, "images\\menubackground.jpg", BK_WIDTH, BK_HEIGHT);
	loadimage(&gamebackground, "images\\gamebackground.jpg", BK_WIDTH, BK_HEIGHT);
	loadimage(&game, "images\\game.jpg", 2*IMG_SIZE, 2*IMG_SIZE);
	for (int i = 1; i <= 9; i++) {
		char str[15];
		sprintf(str, "images\\%d.jpg", i);
		loadimage(&imgArr[i], str, IMG_SIZE, IMG_SIZE);
	}
}

//初始化图片
void initImg() {
	memset(arr, 0, sizeof(arr)); //初始化为0
	srand(unsigned int(time(NULL)));
	int x = rand() % 9 + 1;
	arr[rand() % 3 + 1][rand() % 3 + 1] = -x;
	int tmpArr[10] = { 0 };

	for (int i = 1; i <= 3; i++) {
		for (int j = 1; j <= 3; j++) {
			int r = rand() % 9 + 1;
			if (arr[i][j] == -x) {
				continue;
			}
			else if (tmpArr[r] == 0 && r != x) {
				tmpArr[r] = 1;
				arr[i][j] = r;
			}
			else {
				j--;
				continue;
			}
		}
	}
}

//逆序数
int inverseNumber () {
	int sum = 0, k = 1;		//sum为逆序数
	int tmp[10];
	//将arr里面的数据导入到一维数组tmp里面去
	for (int i = 1; i <= 3; i++) {
		for (int j = 1; j <= 3; j++) {
			if (arr[i][j] > 0) {
				tmp[k++] = arr[i][j];
			}
		}
	}
	//求逆序数
	for (int i = 2; i < 9; i++)		//从第二个数起有逆序数
	{
		for (int j = 1; j < i; j++)		//循环遍历 第i个数 前面的数
		{
			if (tmp[i] < tmp[j]) {
				sum++;
			}
		}
	}
	return sum;	//返回逆序数

}

//绘制菜单界面
void drawMenu() {
	initgraph(BK_WIDTH, BK_HEIGHT);
	//菜单背景图片
	putimage(0, 0, &menubackground);

	//文字布局
	settextcolor(WHITE);                            //设置字体颜色
	settextstyle(30, 0, "华文新魏"); //设置文字样式
	setbkmode(TRANSPARENT);                         //文字背景透明
	
	outtextxy(460, 100, "开始游戏");
	outtextxy(460, 150, "排行榜");
	outtextxy(460, 200, "游戏设置");
	outtextxy(460, 250, "退出游戏");

	settextstyle(60, 0, "华文新魏"); //设置文字样式
	outtextxy(200, 100, "滑  动");
	outtextxy(200, 200, "拼  图");
}

//绘制游戏进行界面
void drawGame() {
	BeginBatchDraw();
	//菜单背景图片
	putimage(0, 0, &gamebackground);
	putimage(500, 100, &game);
	for (int i = 1; i <= 3; i++) {
		for (int j = 1; j <= 3; j++) {
			if (arr[i][j] > 0) {
				putimage(j * IMG_SIZE, i * IMG_SIZE, &imgArr[arr[i][j]]);
			}
		}
	}
	
	//文字布局
	settextcolor(BLACK);                            //设置字体颜色
	settextstyle(30, 0, "华文新魏"); //设置文字样式
	setbkmode(TRANSPARENT);                         //文字背景透明

	char str[10];
	sprintf(str, "time:%ds", (end - start) / 1000);
	outtextxy(100, 50, str);
	outtextxy(350, 50, "quit");
	EndBatchDraw();
}

//判断游戏胜利
int isWin() {
	for (int i = 1; i <= 3; i++) {
		for (int j = 1; j <= 3; j++) {
			if (arr[i][j] > 0) {
				if (arr[i][j] != (i - 1) * 3 + j) {
					return 0;
				}
			}
		}
	}
	return 1;
}

//游戏进行
void playGame() {
	initgraph(BK_WIDTH, BK_HEIGHT);
	MOUSEMSG m;
	drawGame();
	//播放菜单背景音乐
	mciSendString ("open musics\\bkmusic.mp3 alias bkmusic", nullptr, 0, nullptr);
	mciSendString ("play bkmusic repeat", nullptr, 0, nullptr);
	int ti = 0, tj = 0;
	start = clock();
	int over = 0;
	while (TRUE) {
		if (over == 0) {
			end = clock ();
		}
		drawGame();
		if (MouseHit()) {
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN) {
				for (int i = 1; i <= 3; i++) {
					for (int j = 1; j <= 3; j++) {
						if (m.x >= i * IMG_SIZE && m.x <= (i + 1) * IMG_SIZE && m.y >= j * IMG_SIZE && m.y <= (j + 1) * IMG_SIZE) {
							ti = i;
							tj = j;
							break;
						}
					}
				}
				if (m.x >= 350 && m.x <= 410 && m.y >= 50 && m.y <= 80) {
					mciSendString ("close bkmusic", nullptr, 0, nullptr);
					menuMouse();
					while (1) {
						initImg ();
						int n = inverseNumber ();
						if (n % 2 == 0) {
							break;
						}
					}
				}
			}
			if (m.uMsg == WM_LBUTTONUP) {
				for (int i = 1; i <= 3; i++) {
					for (int j = 1; j <= 3; j++) {
						if (m.x >= i * IMG_SIZE && m.x <= (i + 1) * IMG_SIZE && m.y >= j * IMG_SIZE && m.y <= (j + 1) * IMG_SIZE) {
							if (arr[tj][ti] < 0 || arr[j][i] > 0) {
								ti = 0;
								tj = 0;
								break;
							}
							else if (abs(ti - i) == 1 && abs(tj - j) == 0 || (abs(ti - i) == 0 && abs(tj - j) == 1)) {
								int tmp = arr[j][i];
								arr[j][i] = arr[tj][ti];
								arr[tj][ti] = tmp;
								drawGame();
								if (isWin() == 1) {
									for (int i = 1; i <= 3; i++) {
										for (int j = 1; j <= 3; j++) {
											if (arr[i][j] < 0) {
												arr[i][j] = -arr[i][j];
											}
										}
									}
									drawGame();
									settextcolor (RED);
									outtextxy (540, 350, "victory!!");
									over = 1;
									int k;
									for (k = 0; k < 100; k++) {
										if (record[k] == 0) {
											record[k] = (end - start) / 1000;
											break;
										}
									}
									int n = k + 1;
									for (k = 0; k < n - 1; k++) {
										for (int l = 0; l < n - k - 1; l++) {
											if (record[l] > record[l + 1]) {
												int t = record[l];
												record[l] = record[l + 1];
												record[l + 1] = t;
											}
										}
									}
									SaveHighRecordFile ();
								}
							}
						}
					}
				}
			}
		}
		

	}
}

//游戏菜单界面
void menuMouse() {
	drawMenu();      //绘制菜单界面
	MOUSEMSG m;
	while (TRUE) {
		//获取鼠标位置
		m = GetMouseMsg();

		//开始游戏
		if (m.x >= 460 && m.x <= 580 && m.y >= 100 && m.y <= 130) {
			//鼠标左击
			if (m.uMsg == WM_LBUTTONDOWN) {
				mciSendString ("open musics\\button.mp3 alias button", nullptr, 0, nullptr);
				mciSendString ("play button wait", nullptr, 0, nullptr);
				mciSendString ("close button", nullptr, 0, nullptr);
				//播放游戏进行背景音乐
				mciSendString("open musics\\bkmusic.mp3 alias bkmusic", nullptr, 0, nullptr);
				mciSendString("play bkmusic repeat", nullptr, 0, nullptr);
				playGame();
			}
		}

		//排行榜
		else if (m.x >= 460 && m.x <= 550 && m.y >= 150 && m.y <= 180) {
			//鼠标左击
			if (m.uMsg == WM_LBUTTONDOWN) {
				mciSendString ("open musics\\button.mp3 alias button", nullptr, 0, nullptr);
				mciSendString ("play button wait", nullptr, 0, nullptr);
				mciSendString ("close button", nullptr, 0, nullptr);
				DrawHighScore ();
			}
		}

		//游戏设置
		else if (m.x >= 460 && m.x <= 580 && m.y >= 200 && m.y <= 230) {
			//鼠标左击
			if (m.uMsg == WM_LBUTTONDOWN) {
			}
		}

		//退出游戏
		else if (m.x >= 460 && m.x <= 580 && m.y >= 250 && m.y <= 280) {
			//鼠标左击
			if (m.uMsg == WM_LBUTTONDOWN) {
				exit(0);
			}
		}
	}
}


int main() {
	ReadHighRecordFile ();
	loadResource();
	while (1) {
		initImg ();
		int n = inverseNumber ();
		if (n % 2 == 0) {
			break;
		}
	}
	menuMouse();
	return 0;
}




//绘制最高记录界面
void DrawHighScore () {
	initgraph (BK_WIDTH, BK_HEIGHT);
	putimage (0, 0, &menubackground);
	//文字属性设置
	settextcolor (WHITE);                            //设置字体颜色
	settextstyle (30, 0, "华文新魏"); //设置文字样式
	setbkmode (TRANSPARENT);                         //文字背景透明  

	for (int i = 0; i < 3; i++) {
		char str[10];
		sprintf (str, "第%d名", i+1);
		outtextxy (200, 100 * (i+1), str);
		sprintf (str, "%ds", record[i]);
		outtextxy (400, 100 * (i+1), str);
	}
	
	//返回菜单按钮
	//outtextxy (WINDOW_LONG / 7, WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10, L"返回菜单");
}



//读取最高纪录文件
void ReadHighRecordFile () {
	FILE* fp;
	if ((fopen_s (&fp, "最高纪录.txt", "r")) != 0) 	{
		if ((fopen_s (&fp, "最高纪录.txt", "w")) != 0) 		{
			printf ("创建最高纪录文件失败\n");
			exit (-1);
		}
	}
	else 	{
		for (int i = 0; i < 3; i++) 		{
			fread (&record[i], sizeof (int), 1, fp);
		}
	}
	fclose (fp);
}

//保存最高纪录文件
void SaveHighRecordFile () {
	FILE* fp;
	if ((fopen_s (&fp, "最高纪录.txt", "r+")) != 0) 	{
		printf ("打开最高纪录文件失败\n");
		exit (-1);
	}
	else 	{
		for (int i = 0; i < 3; i++) 		{
			fwrite (&record[i], sizeof(int), 1, fp);
		}
	}
	fclose (fp);
}


