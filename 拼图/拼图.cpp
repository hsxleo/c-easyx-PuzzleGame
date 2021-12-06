#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")//��Ƶ


#define BK_WIDTH 800
#define BK_HEIGHT 500
#define IMG_SIZE 100


IMAGE menubackground;
IMAGE gamebackground;
IMAGE game;
IMAGE imgArr[10];
clock_t  start, end;	   //��Ϸ��ʱ����

int arr[4][4]; //���3*3��ͼƬ

int record[100] = {0}; //��ż�¼


void menuMouse(); //��Ϸ�˵�����

//��ȡ��߼�¼�ļ�
void ReadHighRecordFile ();
//������߼�¼�ļ�
void SaveHighRecordFile ();
//������߼�¼����
void DrawHighScore ();

//������Դ
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

//��ʼ��ͼƬ
void initImg() {
	memset(arr, 0, sizeof(arr)); //��ʼ��Ϊ0
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

//������
int inverseNumber () {
	int sum = 0, k = 1;		//sumΪ������
	int tmp[10];
	//��arr��������ݵ��뵽һά����tmp����ȥ
	for (int i = 1; i <= 3; i++) {
		for (int j = 1; j <= 3; j++) {
			if (arr[i][j] > 0) {
				tmp[k++] = arr[i][j];
			}
		}
	}
	//��������
	for (int i = 2; i < 9; i++)		//�ӵڶ���������������
	{
		for (int j = 1; j < i; j++)		//ѭ������ ��i���� ǰ�����
		{
			if (tmp[i] < tmp[j]) {
				sum++;
			}
		}
	}
	return sum;	//����������

}

//���Ʋ˵�����
void drawMenu() {
	initgraph(BK_WIDTH, BK_HEIGHT);
	//�˵�����ͼƬ
	putimage(0, 0, &menubackground);

	//���ֲ���
	settextcolor(WHITE);                            //����������ɫ
	settextstyle(30, 0, "������κ"); //����������ʽ
	setbkmode(TRANSPARENT);                         //���ֱ���͸��
	
	outtextxy(460, 100, "��ʼ��Ϸ");
	outtextxy(460, 150, "���а�");
	outtextxy(460, 200, "��Ϸ����");
	outtextxy(460, 250, "�˳���Ϸ");

	settextstyle(60, 0, "������κ"); //����������ʽ
	outtextxy(200, 100, "��  ��");
	outtextxy(200, 200, "ƴ  ͼ");
}

//������Ϸ���н���
void drawGame() {
	BeginBatchDraw();
	//�˵�����ͼƬ
	putimage(0, 0, &gamebackground);
	putimage(500, 100, &game);
	for (int i = 1; i <= 3; i++) {
		for (int j = 1; j <= 3; j++) {
			if (arr[i][j] > 0) {
				putimage(j * IMG_SIZE, i * IMG_SIZE, &imgArr[arr[i][j]]);
			}
		}
	}
	
	//���ֲ���
	settextcolor(BLACK);                            //����������ɫ
	settextstyle(30, 0, "������κ"); //����������ʽ
	setbkmode(TRANSPARENT);                         //���ֱ���͸��

	char str[10];
	sprintf(str, "time:%ds", (end - start) / 1000);
	outtextxy(100, 50, str);
	outtextxy(350, 50, "quit");
	EndBatchDraw();
}

//�ж���Ϸʤ��
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

//��Ϸ����
void playGame() {
	initgraph(BK_WIDTH, BK_HEIGHT);
	MOUSEMSG m;
	drawGame();
	//���Ų˵���������
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

//��Ϸ�˵�����
void menuMouse() {
	drawMenu();      //���Ʋ˵�����
	MOUSEMSG m;
	while (TRUE) {
		//��ȡ���λ��
		m = GetMouseMsg();

		//��ʼ��Ϸ
		if (m.x >= 460 && m.x <= 580 && m.y >= 100 && m.y <= 130) {
			//������
			if (m.uMsg == WM_LBUTTONDOWN) {
				mciSendString ("open musics\\button.mp3 alias button", nullptr, 0, nullptr);
				mciSendString ("play button wait", nullptr, 0, nullptr);
				mciSendString ("close button", nullptr, 0, nullptr);
				//������Ϸ���б�������
				mciSendString("open musics\\bkmusic.mp3 alias bkmusic", nullptr, 0, nullptr);
				mciSendString("play bkmusic repeat", nullptr, 0, nullptr);
				playGame();
			}
		}

		//���а�
		else if (m.x >= 460 && m.x <= 550 && m.y >= 150 && m.y <= 180) {
			//������
			if (m.uMsg == WM_LBUTTONDOWN) {
				mciSendString ("open musics\\button.mp3 alias button", nullptr, 0, nullptr);
				mciSendString ("play button wait", nullptr, 0, nullptr);
				mciSendString ("close button", nullptr, 0, nullptr);
				DrawHighScore ();
			}
		}

		//��Ϸ����
		else if (m.x >= 460 && m.x <= 580 && m.y >= 200 && m.y <= 230) {
			//������
			if (m.uMsg == WM_LBUTTONDOWN) {
			}
		}

		//�˳���Ϸ
		else if (m.x >= 460 && m.x <= 580 && m.y >= 250 && m.y <= 280) {
			//������
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




//������߼�¼����
void DrawHighScore () {
	initgraph (BK_WIDTH, BK_HEIGHT);
	putimage (0, 0, &menubackground);
	//������������
	settextcolor (WHITE);                            //����������ɫ
	settextstyle (30, 0, "������κ"); //����������ʽ
	setbkmode (TRANSPARENT);                         //���ֱ���͸��  

	for (int i = 0; i < 3; i++) {
		char str[10];
		sprintf (str, "��%d��", i+1);
		outtextxy (200, 100 * (i+1), str);
		sprintf (str, "%ds", record[i]);
		outtextxy (400, 100 * (i+1), str);
	}
	
	//���ز˵���ť
	//outtextxy (WINDOW_LONG / 7, WINDOW_WIDE / 7 + 6 * WINDOW_WIDE / 10, L"���ز˵�");
}



//��ȡ��߼�¼�ļ�
void ReadHighRecordFile () {
	FILE* fp;
	if ((fopen_s (&fp, "��߼�¼.txt", "r")) != 0) 	{
		if ((fopen_s (&fp, "��߼�¼.txt", "w")) != 0) 		{
			printf ("������߼�¼�ļ�ʧ��\n");
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

//������߼�¼�ļ�
void SaveHighRecordFile () {
	FILE* fp;
	if ((fopen_s (&fp, "��߼�¼.txt", "r+")) != 0) 	{
		printf ("����߼�¼�ļ�ʧ��\n");
		exit (-1);
	}
	else 	{
		for (int i = 0; i < 3; i++) 		{
			fwrite (&record[i], sizeof(int), 1, fp);
		}
	}
	fclose (fp);
}


