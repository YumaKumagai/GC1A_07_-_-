#include "DxLib.h"
#include<math.h>
#include<stdio.h>
#include<stdlib.h>

#define ��(X) ((X)*3.1415/180)
#define NUM 300

// �E�B���h�E�̃^�C�g���ɕ\�����镶����
const char TITLE[] = "GC1A_07_�N�}�K�C���E�}: ����";

// �E�B���h�E����
const int WIN_WIDTH = 800;

// �E�B���h�E�c��
const int WIN_HEIGHT = 800;

// �ŐV�̃L�[�{�[�h���p
char keys[256] = { 0 };

// 1���[�v(�t���[��)�O�̃L�[�{�[�h���
char oldkeys[256] = { 0 };

typedef struct
{
	float PosX;
	float PosY;
	float Left;	//���[
	float Upper;	//��[
	int FlagLR;
	int FlagUD;
	int PhaseFlag;
	int TimeAxis;	//���Ԏ�(�̎���)
	int Remain;
	int BlazTimer[2];	//�e���p�^�C�}�[�Ȃ�
	int numofLengs[2];	//�e���̒���(�J�E���^�[�I)
	int numofTimes[2];	//�e���̉�(�J�E���^�[�I)
	int GraphHandle;
}state_t;

typedef struct
{
	float PosX;
	float PosY;
	float Left;
	float Upper;
	float Velo;
	double Angle;
	int Radius;
	int HitFlag;
	int NumFlag;
	int GraphHandle;
	int AnimationTimer;
}blaz_state_t;

//�v���g�^�C�v�錾
void DrawSuujiGraph(int, float, float, int, int*);
void DrawMojiGraph(int, float, float, char*, int*);
void background();
void subscreen(int);
int range(int, float, float, float, float, float, float, float, float, float, float);
void player_update(state_t*, blaz_state_t*, int, int*);
void player_draw(state_t*, state_t*, int*, int*, int);
void mosque_update(int, float, float, float, float, float, float, state_t*, state_t*, blaz_state_t*, int*);
void arakune_update(int, int, float, float, float, state_t*, state_t*, blaz_state_t*, int, int*);
void dugaw_update(int, float, float, float, float, float, int, float, float, state_t*, state_t*, blaz_state_t*, int, int*);
void espadar_update(int, float, float, float, float, float, float, float, int, int, int, state_t*, state_t*, blaz_state_t*, int*);


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {
	// �E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);

	// �E�B���h�E�T�C�Y���蓮�ł͕ύX�������A
	// ���E�B���h�E�T�C�Y�ɍ��킹�Ċg��ł��Ȃ��悤�ɂ���
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// �^�C�g����ύX
	SetMainWindowText(TITLE);

	// ��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�(���j�^�[�̉𑜓x�ɍ��킹��)
	SetGraphMode(WIN_WIDTH + 400, WIN_HEIGHT, 32);

	// ��ʃT�C�Y��ݒ�(�𑜓x�Ƃ̔䗦�Őݒ�)
	SetWindowSizeExtendRate(1.0);

	// ��ʂ̔w�i�F��ݒ肷��
	SetBackgroundColor(0x00, 0x00, 0x00);

	// DXlib�̏�����
	if (DxLib_Init() == -1) { return -1; }

	// (�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);


	// �Q�[�����[�v�Ŏg���ϐ��̐錾
	int SceneNo = 0;
	int TimeAxis = 0;
	int numofGroup = 0, meshofGroup = 10;
	state_t player = { 200,600,48,64,0,0,0,0,5,0,0,0,0 };
	blaz_state_t blaz[NUM];
	state_t moray;
	state_t mosque[10];
	state_t arakune[3];
	state_t dugaw[3];
	state_t espadar[3];
	for (int i = 0; i < 10; i++)
	{
		mosque[i].TimeAxis = 0;
		mosque[i].PhaseFlag = -1;
		mosque[i].Remain = -1;
		if (i < 3)
		{
			arakune[i].TimeAxis = 0;
			arakune[i].PhaseFlag = -1;
			arakune[i].Remain = -1;
			dugaw[i].TimeAxis = 0;
			dugaw[i].PhaseFlag = -1;
			dugaw[i].Remain = -1;
			espadar[i].TimeAxis = 0;
			espadar[i].PhaseFlag = -1;
			espadar[i].Remain = -1;
		}
	}

	// �摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���
	int mojiGraphHandle[38];
	int moji2GraphHandle[38];
	int opGraphHandle = LoadGraph("Resource/kokuu2.png", 0);
	int subscreenframeGraphHandle = LoadGraph("Resource/subscreen_frame.png", 0);
	int playerGraphHandle[3];
	int player_blazGraphHandle = LoadGraph("Resource/player_bullet01.png", 0);
	int small_exploGraphHandle[11];
	int small_explo2GraphHandle[4];
	int explosionGraphHandle[11];
	int explo_blueGraphHandle[11];
	int bullet3GraphHandle[6];
	int bluebulletGraphHandle[3];
	int pinkbulletGraphHandle[3];
	int morayGraphHandle = LoadGraph("Resource/moray.png", 0);
	int mosqueGraphHandle[3];
	int arakuneGraphHandle = LoadGraph("Resource/arakune.png", 0);
	int dugawGraphHandle = LoadGraph("Resource/dugaw.png", 0);
	int espadarGraphHandle[4];
	if (LoadDivGraph("Resource/0123ABCD.png", 38, 10, 4, 16, 16, mojiGraphHandle, 0) == -1
		|| LoadDivGraph("Resource/0123ABCD_2.png", 38, 10, 4, 64, 80, moji2GraphHandle, 0) == -1
		|| LoadDivGraph("Resource/player.png", 3, 3, 1, 96, 96, playerGraphHandle, 0) == -1
		|| LoadDivGraph("Resource/explosion.png", 11, 3, 4, 96, 96, explosionGraphHandle, 0) == -1
		|| LoadDivGraph("Resource/explosion(blue).png", 11, 3, 4, 96, 96, explo_blueGraphHandle, 0) == -1
		|| LoadDivGraph("Resource/small_explo.png", 11, 3, 4, 96, 96, small_exploGraphHandle, 0) == -1
		|| LoadDivGraph("Resource/small_explo2(blue).png", 4, 4, 1, 64, 64, small_explo2GraphHandle, 0) == -1
		|| LoadDivGraph("Resource/bullet3.png", 6, 3, 2, 16, 16, bullet3GraphHandle, 0) == -1
		|| LoadDivGraph("Resource/bullet1.png", 3, 3, 1, 32, 32, bluebulletGraphHandle, 0) == -1
		|| LoadDivGraph("Resource/bullet2.png", 3, 3, 1, 32, 32, pinkbulletGraphHandle, 0) == -1
		|| LoadDivGraph("Resource/mosque.png", 3, 3, 1, 96, 64, mosqueGraphHandle, 0) == -1
		|| LoadDivGraph("Resource/espadar.png", 4, 1, 4, 512, 256, espadarGraphHandle, 0) == -1)
	{
		return -1;
	}


	//�e�z�񏉊���
	for (int i = 0; i < NUM; i++)
	{
		blaz[i].HitFlag = 0;
		blaz[i].NumFlag = -1;
		blaz[i].AnimationTimer = 0;
	}
	int number_of_blaz = 0;


	// �Q�[�����[�v
	while (true) {
		// �ŐV�̃L�[�{�[�h��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
		for (int i = 0; i < 256; i++)
		{
			oldkeys[i] = keys[i];
		}
		// �ŐV�̃L�[�{�[�h�����擾
		GetHitKeyStateAll(keys);

		// ��ʃN���A
		ClearDrawScreen();
		//---------  ��������v���O�������L�q  ----------//

		// �X�V����
		switch (SceneNo) {
		case 0: {
			if (keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE])
			{
				SceneNo = 1;
			}
		}
			  break;
		case 1: {
			//player
			player_update(&player, blaz, player_blazGraphHandle, small_explo2GraphHandle);

			//�S�����ґ����̂P //�N�_��Remain>=0
			if (TimeAxis >= 120 && TimeAxis < 330)
			{
				if (TimeAxis % meshofGroup == 0 && numofGroup < 10)
				{
					mosque[numofGroup].Remain = 4;
					numofGroup++;
				}
				for (int i = 0; i < numofGroup; i++)
				{
					if (mosque[i].Remain >= 0)
					{
						mosque_update(i, WIN_WIDTH + 100, -100, WIN_WIDTH + 50, -20, -8, 10, mosque, &player, blaz, mosqueGraphHandle);
					}
				}
				if (TimeAxis == 329)
				{
					numofGroup = 0;
				}
			}
			//�S�����ґ����̂Q
			if (TimeAxis >= 330 && TimeAxis < 610)
			{
				if (TimeAxis % meshofGroup == 0 && numofGroup < 10)
				{
					mosque[numofGroup].Remain = 5;
					numofGroup++;
				}
				for (int i = 0; i < numofGroup; i++)
				{
					if (mosque[i].Remain >= 0)
					{
						mosque_update(i, 500, -100, 400, -30, 0, 8, mosque, &player, blaz, mosqueGraphHandle);
					}
				}
				if (TimeAxis == 609)
				{
					numofGroup = 0;
				}
			}
			//�S�����ґ����̂R
			if (TimeAxis >= 610 && TimeAxis < 910)
			{
				if (TimeAxis % meshofGroup == 0 && numofGroup < 10)
				{
					mosque[numofGroup].Remain = 5;
					numofGroup++;
				}
				for (int i = 0; i < numofGroup; i++)
				{
					if (mosque[i].Remain >= 0)
					{
						mosque_update(i, 500, -100, -30, 100, 10, 4, mosque, &player, blaz, mosqueGraphHandle);
					}
				}
				if (TimeAxis == 909)
				{
					numofGroup = 0;
				}
			}

			//�r�����̂P	//�N�_��PhaseFlag=0
			if (TimeAxis >= 720)
			{
				if (TimeAxis == 720)
				{
					arakune[0].PhaseFlag = 0;
				}
				if (arakune[0].PhaseFlag != -1)
				{
					arakune_update(0, 55, 200, -50, 2.5, arakune, &player, blaz, arakuneGraphHandle, explo_blueGraphHandle);
				}
			}
			//�r�����̂Q
			if (TimeAxis >= 920)
			{
				if (TimeAxis == 920)
				{
					arakune[1].PhaseFlag = 0;
				}
				if (arakune[1].PhaseFlag != -1)
				{
					arakune_update(1, 55, 500, -50, 2.5, arakune, &player, blaz, arakuneGraphHandle, explo_blueGraphHandle);
				}
			}
			//�r�����̂R
			if (TimeAxis >= 1100)
			{
				if (TimeAxis == 1100)
				{
					arakune[2].PhaseFlag = 0;
				}
				if (arakune[2].PhaseFlag != -1)
				{
					arakune_update(2, 55, 400, -50, 2.5, arakune, &player, blaz, arakuneGraphHandle, explo_blueGraphHandle);
				}
			}

			//�W�����̂P	//�N�_PhaseFlag=0
			if (TimeAxis >= 1250)
			{
				if (TimeAxis == 1250)
				{
					dugaw[0].PhaseFlag = 0;
				}
				if (dugaw[0].PhaseFlag != -1)
				{
					dugaw_update(0, 50, -50, 350, 250, 200, 45, 5, 0, dugaw, &player, blaz, dugawGraphHandle, explo_blueGraphHandle);
				}
			}
			//�W�����̂Q
			if (TimeAxis >= 1320)
			{
				if (TimeAxis == 1320)
				{
					dugaw[1].PhaseFlag = 0;
				}
				if (dugaw[1].PhaseFlag != -1)
				{
					dugaw_update(1, 60, -50, 250, 400, 150, 45, 5, 0, dugaw, &player, blaz, dugawGraphHandle, explo_blueGraphHandle);
				}
			}
			//�W�����̂R
			if (TimeAxis >= 1350)
			{
				if (TimeAxis == 1350)
				{
					dugaw[2].PhaseFlag = 0;
				}
				if (dugaw[2].PhaseFlag != -1)
				{
					dugaw_update(2, 70, -50, 150, 550, 100, 45, 3, 0, dugaw, &player, blaz, dugawGraphHandle, explo_blueGraphHandle);
				}
			}

			//Espadar���̂P
			if (TimeAxis >= 1500)
			{
				if (TimeAxis == 1500)
				{
					espadar[0].PhaseFlag = 0;
				}
				if (espadar[0].PhaseFlag != -1)
				{
					espadar_update(0, 45, WIN_WIDTH - 250, -50, WIN_WIDTH - 250, WIN_HEIGHT - 100, WIN_WIDTH - 250, 150, 60, 5, 45, espadar, &player, blaz, espadarGraphHandle);
				}
			}
			//Espadar���̂Q
			if (TimeAxis >= 1600)
			{
				if (TimeAxis == 1600)
				{
					espadar[1].PhaseFlag = 0;
				}
				if (espadar[1].PhaseFlag != -1)
				{
					espadar_update(1, 50, 250, -50, 250, WIN_HEIGHT - 200, 250, 150, 60, 5, 45, espadar, &player, blaz, espadarGraphHandle);
				}
			}
			//Espadar���̂R
			if (TimeAxis >= 1700)
			{
				if (TimeAxis == 1700)
				{
					espadar[2].PhaseFlag = 0;
				}
				if (espadar[2].PhaseFlag != -1)
				{
					espadar_update(2, 55, -50, 400, 400, WIN_HEIGHT - 300, 400, 150, 60, 5, 45, espadar, &player, blaz, espadarGraphHandle);
				}
			}

			//�e������
			for (int i = 0; i < NUM; i++)
			{
				if (blaz[i].NumFlag >= 0)
				{
					blaz[i].PosX += blaz[i].Velo * cos(blaz[i].Angle);
					blaz[i].PosY += blaz[i].Velo * sin(blaz[i].Angle);
					if ((blaz[i].NumFlag >= 1 && blaz[i].NumFlag <= 11)
						|| (blaz[i].NumFlag >= 18 && blaz[i].NumFlag <= 20))	//�s���N���ȉ~�e	//�S�����ґ����̂P�`�R,�r�����̂P�`�R�Őj
					{
						blaz[i].GraphHandle = bullet3GraphHandle[(TimeAxis / 3) % 6];
						blaz[i].Radius = blaz[i].Upper = blaz[i].Left = 8;
					}
					if ((blaz[i].NumFlag >= 12 && blaz[i].NumFlag <= 17)
						|| (blaz[i].NumFlag >= 21 && blaz[i].NumFlag <= 26)
						|| (blaz[i].NumFlag >= 30 && blaz[i].NumFlag <= 32))	//�e	//�r�����̂P�`�R���E��,�W�����̂P�`�R�f���e�����E�e��
					{
						blaz[i].GraphHandle = bluebulletGraphHandle[(TimeAxis / 3) % 3];
						blaz[i].Upper = blaz[i].Left = 16;
						blaz[i].Radius = blaz[i].Left - 4;
					}
					if ((blaz[i].NumFlag >= 27 && blaz[i].NumFlag <= 29))
					{
						blaz[i].GraphHandle = pinkbulletGraphHandle[(TimeAxis / 3) % 3];
						blaz[i].Upper = blaz[i].Left = 16;
						blaz[i].Radius = blaz[i].Left - 4;
					}
					if (!range(0, blaz[i].PosX, blaz[i].PosY, 0, 0, 0, 0, 0, WIN_WIDTH, 0, WIN_HEIGHT))
					{
						blaz[i].NumFlag = -1;
					}
				}
			}
			TimeAxis++;
			if (TimeAxis >= 2000 || player.Remain < 0)
			{
				SceneNo = 2;
			}
			break;
		}
		}

		// �`�揈��	�@//NumFlag=-1��Remain=-1�̂Ƃ��`�悳��Ȃ�
		switch (SceneNo) {
		case 0: {
			DrawGraph(250, 150, opGraphHandle, 1);
			char str_op[30] = "PLESS_SPACE_TO_START";
			DrawMojiGraph(16, 400, 650, str_op, mojiGraphHandle);
		}
			  break;
		case 1: {
			background();	//�w�i�`��
			player_draw(&player, &moray, playerGraphHandle, explo_blueGraphHandle, morayGraphHandle);	//player�`��
			for (int i = 0; i < numofGroup; i++)	//�S�����ґ�
			{
				if (mosque[i].Remain >= 0)
				{
					DrawGraph(mosque[i].PosX - mosque[i].Left, mosque[i].PosY - mosque[i].Upper, mosqueGraphHandle[mosque[i].FlagLR], 1);
				}
			}
			for (int i = 0; i < 3; i++)		//�r��
			{
				if (arakune[i].PhaseFlag != -1)
				{
					if (arakune[i].PhaseFlag == 3)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
						DrawGraph(arakune[i].PosX - arakune[i].Left, arakune[i].PosY - arakune[i].Upper, arakune[i].GraphHandle, 1);
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
					}
					else
					{
						DrawGraph(arakune[i].PosX - arakune[i].Left, arakune[i].PosY - arakune[i].Upper, arakune[i].GraphHandle, 1);
					}
				}
			}
			for (int i = 0; i < 3; i++)	//�W��
			{
				if (dugaw[i].PhaseFlag != -1)
				{
					if (dugaw[i].PhaseFlag == 4)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
						DrawGraph(dugaw[i].PosX - dugaw[i].Left, dugaw[i].PosY - dugaw[i].Upper, dugaw[i].GraphHandle, 1);
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
					}
					else
					{
						DrawGraph(dugaw[i].PosX - dugaw[i].Left, dugaw[i].PosY - dugaw[i].Upper, dugaw[i].GraphHandle, 1);
					}
				}
			}
			for (int i = 0; i < 3; i++)
			{
				if (espadar[i].PhaseFlag != -1)
				{
					DrawGraph(espadar[i].PosX - espadar[i].Left, espadar[i].PosY - espadar[i].Upper, espadar[i].GraphHandle, 1);
				}
			}
			for (int i = 0; i < NUM; i++)	//�e���`��
			{
				if (blaz[i].NumFlag != -1) {
					number_of_blaz++;
					if (blaz[i].NumFlag == 0 || blaz[i].NumFlag == -2)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
						DrawGraph(blaz[i].PosX - blaz[i].Left, blaz[i].PosY - blaz[i].Upper, blaz[i].GraphHandle, 1);
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
					}
					else
					{
						DrawGraph(blaz[i].PosX - blaz[i].Left, blaz[i].PosY - blaz[i].Upper, blaz[i].GraphHandle, 1);
					}
				}
			}
			subscreen(subscreenframeGraphHandle);

			char
				str1[24] = "NUM_OF_BULLET",
				str2[24] = "PLAYER_REMAIN",
				str3[24] = "TIME_AXIS";
			DrawMojiGraph(16, WIN_WIDTH + 100, 100, str1, mojiGraphHandle);
			DrawSuujiGraph(16, WIN_WIDTH + 300, 120, number_of_blaz, mojiGraphHandle);
			number_of_blaz = 0;
			DrawMojiGraph(16, WIN_WIDTH + 100, 140, str2, mojiGraphHandle);
			DrawSuujiGraph(16, WIN_WIDTH + 300, 160, player.Remain, mojiGraphHandle);
			DrawMojiGraph(16, WIN_WIDTH + 100, 180, str3, mojiGraphHandle);
			DrawSuujiGraph(16, WIN_WIDTH + 300, 200, TimeAxis, mojiGraphHandle);
		}
			  break;
		case 2: {
			char str_ed[] = "GAME_OVER";
			DrawMojiGraph(64, 250, 300, str_ed, moji2GraphHandle);
		}
			  break;
		}
		//---------  �����܂łɃv���O�������L�q  ---------//
		// (�_�u���o�b�t�@)����
		ScreenFlip();

		if (keys[KEY_INPUT_X])
		{
			WaitTimer(5);
		}
		else
		{
			// 20�~���b�ҋ@(�^��60FPS)
			WaitTimer(20);
		}

		// Windows�V�X�e�����炭�������������
		if (ProcessMessage() == -1) {
			break;
		}

		// ESC�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	// Dx���C�u�����I������
	DxLib_End();

	// ����I��
	return 0;
}

void DrawMojiGraph(int space, float PosX, float PosY, char* str, int* mojiGraph)	//:,A�`Z
{
	int i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == ':')
		{
			DrawGraph(PosX + i * space, PosY, mojiGraph[11], 0);
		}
		else if (str[i] == 95)
		{
			DrawFormatString(PosX + i * space, PosY, 0, " ");
		}
		else
		{
			DrawGraph(PosX + i * space, PosY, mojiGraph[str[i] - 54], 0);
		}
		i++;
	}
}

void DrawSuujiGraph(int space, float PosX, float PosY, int number, int* mojiGraph)	//0�`9
{
	int eachNum[13];
	int i = 0;
	do
	{
		eachNum[i] = number % 10;
		number /= 10;
		i++;
	} while (number);
	for (int j = 0; j < i; j++)
	{
		DrawGraph(PosX - space * j, PosY, mojiGraph[eachNum[j]], 0);
	}
}

void background()
{
	//�����C���[
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 96);
	int back_longiNum = 7, back_latiNum = 9;	//�o���̐�,�ܐ��̐�
	static float back_key_latiY = 0;	//�n�ܐ���Y���W�i�X�N���[���j
	for (int i = 0; i < back_longiNum; i++)
	{
		float back_longiX = WIN_WIDTH / (back_longiNum + 1) * (i + 1);
		DrawLine(back_longiX, 0, back_longiX, WIN_HEIGHT, GetColor(24, 127, 48), 3);
	}
	for (int i = 0; i < back_latiNum; i++)
	{
		float back_latiY = (int)(back_key_latiY + WIN_HEIGHT / back_latiNum * i) % WIN_HEIGHT;
		DrawLine(0, back_latiY, WIN_WIDTH, back_latiY, GetColor(24, 127, 48), 3);
	}
	back_key_latiY += 2;

	//��O���C���[
	int front_longiNum = 2, front_latiNum = 3;	//�o���̐�,�ܐ��̐�
	static float front_key_latiY = 0;	//�n�ܐ���Y���W�i�X�N���[���j
	for (int i = 0; i < front_longiNum; i++)
	{
		float front_longiX = WIN_WIDTH / (front_longiNum + 1) * (i + 1);
		DrawLine(front_longiX, 0, front_longiX, WIN_HEIGHT, GetColor(0, 255, 0), 6);
		DrawLine(front_longiX, 0, front_longiX, WIN_HEIGHT, GetColor(196, 255, 196), 2);
		DrawLine(front_longiX + 3, 0, front_longiX + 3, WIN_HEIGHT, GetColor(32, 196, 64), 1);
		DrawLine(front_longiX - 3, 0, front_longiX - 3, WIN_HEIGHT, GetColor(32, 196, 64), 1);
	}
	for (int i = 0; i < front_latiNum; i++)
	{
		float front_latiY = (int)(front_key_latiY + WIN_WIDTH / front_latiNum * i) % WIN_HEIGHT;
		DrawLine(0, front_latiY, WIN_WIDTH, front_latiY, GetColor(0, 255, 0), 6);
		DrawLine(0, front_latiY - 3, WIN_WIDTH, front_latiY - 3, GetColor(196, 255, 196), 2);
		DrawLine(0, front_latiY + 3, WIN_WIDTH, front_latiY + 3, GetColor(32, 196, 64), 3);
	}
	front_key_latiY += 12;
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void subscreen(int subscreenframe)
{
	DrawBox(WIN_WIDTH, 0, WIN_WIDTH + 400, WIN_HEIGHT, 0, 1);
	DrawGraph(WIN_WIDTH, 0, subscreenframe, 1);
}

int range(int circleFlag, float PosX1, float PosY1, float PosX2, float PosY2, float Radius1, float Radius2, float x_min, float x_max, float y_min, float y_max)
{
	if (circleFlag)
	{
		return (pow((double)PosX1 - PosX2, 2) + pow((double)PosY1 - PosY2, 2) <= pow((double)Radius1 + Radius2, 2));	//�ڐG�܂��͔͈͓��Ȃ�P�A�͈͊O�Ȃ�O��Ԃ�
	}
	else
	{
		return (PosX1 + Radius1 >= x_min && PosX1 - Radius1 <= x_max && PosY1 + Radius1 >= y_min && PosY1 - Radius1 <= y_max);
	}
}

void player_update(state_t* p, blaz_state_t* b, int blazGraphHandle, int* small_explo2GraphHandle)
{
	static float velo = 12;
	int Interval = 2;
	int angle = -1;
	static int Timer;
	//���@�X�V����
	if (p->PhaseFlag == 0 || p->PhaseFlag == 3)
	{
		if ((!keys[KEY_INPUT_UP] && oldkeys[KEY_INPUT_UP]) || (!keys[KEY_INPUT_W] && oldkeys[KEY_INPUT_W])
			|| (!keys[KEY_INPUT_DOWN] && oldkeys[KEY_INPUT_DOWN]) || (!keys[KEY_INPUT_S] && oldkeys[KEY_INPUT_S])
			|| (!keys[KEY_INPUT_RIGHT] && oldkeys[KEY_INPUT_RIGHT]) || (!keys[KEY_INPUT_D] && oldkeys[KEY_INPUT_D])
			|| (!keys[KEY_INPUT_LEFT] && oldkeys[KEY_INPUT_LEFT]) || (!keys[KEY_INPUT_A] && oldkeys[KEY_INPUT_A]))
		{
			keys[KEY_INPUT_UP] = oldkeys[KEY_INPUT_UP] = keys[KEY_INPUT_W] = oldkeys[KEY_INPUT_W] = 0;
			keys[KEY_INPUT_DOWN] = oldkeys[KEY_INPUT_DOWN] = keys[KEY_INPUT_S] = oldkeys[KEY_INPUT_S] = 0;
			keys[KEY_INPUT_RIGHT] = oldkeys[KEY_INPUT_RIGHT] = keys[KEY_INPUT_D] = oldkeys[KEY_INPUT_D] = 0;
			keys[KEY_INPUT_LEFT] = oldkeys[KEY_INPUT_LEFT] = keys[KEY_INPUT_A] = oldkeys[KEY_INPUT_A] = 0;
		}

		if (!(keys[KEY_INPUT_UP] || keys[KEY_INPUT_W] || keys[KEY_INPUT_DOWN] || keys[KEY_INPUT_S]))
		{
			p->FlagUD = 0;
		}
		if (!(keys[KEY_INPUT_RIGHT] || keys[KEY_INPUT_D] || keys[KEY_INPUT_LEFT] || keys[KEY_INPUT_A]))
		{
			p->FlagLR = 0;
		}
		if (keys[KEY_INPUT_UP] && !oldkeys[KEY_INPUT_UP] || keys[KEY_INPUT_W] && !oldkeys[KEY_INPUT_W])
		{
			p->FlagUD = -1;
		}
		else if (keys[KEY_INPUT_DOWN] && !oldkeys[KEY_INPUT_DOWN] || keys[KEY_INPUT_S] && !oldkeys[KEY_INPUT_S])
		{
			p->FlagUD = 1;
		}
		if (keys[KEY_INPUT_RIGHT] && !oldkeys[KEY_INPUT_RIGHT] || keys[KEY_INPUT_D] && !oldkeys[KEY_INPUT_D])
		{
			p->FlagLR = 2;
		}
		else if (keys[KEY_INPUT_LEFT] && !oldkeys[KEY_INPUT_LEFT] || keys[KEY_INPUT_A] && !oldkeys[KEY_INPUT_A])
		{
			p->FlagLR = 1;
		}

		if (p->FlagUD)
		{
			if (p->FlagLR == 2)
			{
				angle = 45;
			}
			else if (p->FlagLR == 1)
			{
				angle = 135;
			}
			else if (p->FlagLR == 0)
			{
				angle = 90;
			}
		}
		else
		{
			if (p->FlagLR == 2)
			{
				angle = 0;
			}
			else if (p->FlagLR == 1)
			{
				angle = 180;
			}
			else if (p->FlagLR == 0)
			{
				angle = -1;
			}
		}
		if (p->FlagUD)
		{
			angle *= p->FlagUD;
		}

		//���x�@�\
		if (!keys[KEY_INPUT_SPACE] && keys[KEY_INPUT_C])
		{
			velo = 16;
		}
		else if (keys[KEY_INPUT_SPACE])
		{
			velo = 7.5;
		}
		if (angle != -1)
		{
			p->PosX += velo * cos(��(angle));
			p->PosY += velo * sin(��(angle));
		}
		if (p->PosX - p->Left <= 0)
		{
			p->PosX = p->Left;
		}
		if (p->PosX + p->Left >= WIN_WIDTH)
		{
			p->PosX = WIN_WIDTH - p->Left;
		}
		if (p->PosY - p->Upper <= 0)
		{
			p->PosY = p->Upper;
		}
		if (p->PosY + p->Upper >= WIN_HEIGHT)
		{
			p->PosY = WIN_HEIGHT - p->Upper;
		}
	}

	//���@�_���[�W����
	if (!p->PhaseFlag && p->Remain >= 0 && !keys[KEY_INPUT_X])
	{
		for (int i = 0; i < NUM; i++)
		{
			if (b[i].NumFlag > 0
				&& range(1, b[i].PosX, b[i].PosY, p->PosX, p->PosY, b[i].Radius, 5, 0, 0, 0, 0))
			{
				p->PhaseFlag = 1;
				b[i].NumFlag = -1;
				break;
			}
		}
	}
	//���e�X�V����	//1//	//�e���ŏ�����NumFlag=-2,small_exploGraphHandle�z��ɂ�
	for (int i = 0; i < NUM; i++)
	{
		if (b[i].NumFlag == 0)
		{
			if (b[i].HitFlag)	//�G�ɏՓ˂�����HitFlag=1,�Փ˔���͊e�G�̊֐�����
			{
				b[i].NumFlag = -2;	//NumFlag=-2�Ŕ��j���Œi�K�ֈڍs,���̎��Փ˔���͏�����
				b[i].HitFlag = 0;
				b[i].Velo = 0;
			}
		}
	}
	//space�L�[������ԏ���
	if (keys[KEY_INPUT_SPACE] && p->PhaseFlag != 1 && p->PhaseFlag != 2)
	{
		Timer++;
		if (Timer % Interval == 0)
		{
			for (int i = 0; i < NUM; i++)
			{
				if (b[i].NumFlag == -1)
				{
					b[i].NumFlag = 0;
					b[i].PosX = p->PosX;
					b[i].PosY = p->PosY - 80;
					b[i].Velo = 30;
					b[i].Angle = ��(-90);
					b[i].Left = 24;
					b[i].Upper = 24;
					b[i].Radius = 30;
					b[i].GraphHandle = blazGraphHandle;
					break;
				}
			}
		}
	}
	else
	{
		Timer = 0;
		velo = 8;
	}
	//�e���ŏ���
	for (int i = 0; i < NUM; i++)
	{
		if (b[i].NumFlag == -2)
		{
			b[i].Left = 32;
			b[i].Upper = 32;
			if (b[i].AnimationTimer < 4)
			{
				b[i].GraphHandle = small_explo2GraphHandle[b[i].AnimationTimer];
			}
			else if (b[i].AnimationTimer == 4)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
				b[i].GraphHandle = small_explo2GraphHandle[3];
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			}
			else if (b[i].AnimationTimer == 5)
			{
				b[i].AnimationTimer = -1;
				b[i].NumFlag = -1;
			}
			b[i].AnimationTimer++;
		}
	}

}

void player_draw(state_t* p, state_t* moray, int* pGraphHandle, int* explo_blueGraphHandle, int morayGraphHandle)
{
	static int AnimationTimer;
	if (p->PhaseFlag == 0 && p->Remain >= 0)
	{
		DrawGraph(p->PosX - p->Left, p->PosY - p->Upper, pGraphHandle[p->FlagLR], 1);
	}
	else if (p->PhaseFlag == 1)
	{
		if (AnimationTimer / 4 < 11)
		{
			DrawGraph(p->PosX - p->Left, p->PosY - p->Upper, explo_blueGraphHandle[AnimationTimer / 4], 1);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
			if (AnimationTimer <= 20)
			{
				DrawGraph(p->PosX - p->Left, p->PosY - p->Upper, pGraphHandle[p->FlagLR], 1);
			}
			else if (AnimationTimer <= 40)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 85);
				DrawGraph(p->PosX - p->Left, p->PosY - p->Upper, pGraphHandle[p->FlagLR], 1);
			}
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
		else if (AnimationTimer / 4 == 11)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
			DrawGraph(p->PosX - p->Left, p->PosY - p->Upper, explo_blueGraphHandle[10], 1);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
		else if (AnimationTimer / 4 == 12)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 85);
			DrawGraph(p->PosX - p->Left, p->PosY - p->Upper, explo_blueGraphHandle[10], 1);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
		else if (AnimationTimer == 52)
		{
			p->FlagLR = 0;
			p->PosX = -60;
			p->PosY = 600;
			AnimationTimer = -1;
			p->PhaseFlag = 2;
			moray->PosX = -390;
			moray->PosY = 600 - 96;
			p->Remain--;
		}
		AnimationTimer++;
	}
	else if (p->PhaseFlag == 2)
	{
		if (AnimationTimer >= 20 && AnimationTimer <= 110)
		{
			moray->PosX += (float)260 / 90;
			p->PosX += (float)260 / 90;

			if (AnimationTimer == 110)
			{
				p->PosX = 200;
				p->PosY = 600;
				p->PhaseFlag = 3;
				AnimationTimer = -1;
			}
		}
		DrawGraph(p->PosX - p->Left, p->PosY - p->Upper, pGraphHandle[0], 1);
		DrawGraph(moray->PosX, moray->PosY, morayGraphHandle, 1);
		AnimationTimer++;
	}
	else if (p->PhaseFlag == 3)
	{
		if (AnimationTimer == 0)
		{
			keys[KEY_INPUT_UP] = oldkeys[KEY_INPUT_UP] = keys[KEY_INPUT_W] = oldkeys[KEY_INPUT_W] = 0;
			keys[KEY_INPUT_DOWN] = oldkeys[KEY_INPUT_DOWN] = keys[KEY_INPUT_S] = oldkeys[KEY_INPUT_S] = 0;
			keys[KEY_INPUT_RIGHT] = oldkeys[KEY_INPUT_RIGHT] = keys[KEY_INPUT_D] = oldkeys[KEY_INPUT_D] = 0;
			keys[KEY_INPUT_LEFT] = oldkeys[KEY_INPUT_LEFT] = keys[KEY_INPUT_A] = oldkeys[KEY_INPUT_A] = 0;
		}
		if (AnimationTimer <= 120)
		{
			if (AnimationTimer >= 20 && AnimationTimer <= 110)
			{
				moray->PosX -= (float)260 / 90;
				if (AnimationTimer == 110)
				{
					moray->PosX = -390;
					moray->PosY = 600 - 96;
				}
			}
			DrawGraph(moray->PosX, moray->PosY, morayGraphHandle, 1);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100 * (1 + AnimationTimer % 2));
			DrawGraph(p->PosX - p->Left, p->PosY - p->Upper, pGraphHandle[p->FlagLR], 1);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			if (AnimationTimer == 120)
			{
				p->PhaseFlag = 0;
				AnimationTimer = -1;
			}
			AnimationTimer++;
		}
	}
}


void mosque_update(int number, float pointAX, float pointAY, float pointBX, float pointBY, float escVeloX, float escVeloY, state_t* m, state_t* p, blaz_state_t* b, int* mosqueGraphHandle)	//�S����(mosque)	
{
	if (m[number].Remain >= 0)
	{
		double vectorMPX, vectorMPY, cross1, cross2;
		double distanceX = pointBX - pointAX;
		double distanceY = pointBY - pointAY;
		double distanceMP = sqrt(pow((double)m[number].PosX - p->PosX, 2) + pow((double)m[number].PosY - p->PosY, 2));
		float ratio = 90 / 25;
		int blazMesh = 4, blazLong = 7, way = 3, blazInterval = 10;
		static double angle, times;
		if (m[number].TimeAxis == 0)
		{
			m[number].PosX = pointAX;
			m[number].PosY = pointAY;
			m[number].Upper = 32;
			m[number].Left = 48;
			m[number].FlagLR = 0;
		}
		if (m[number].TimeAxis <= 90 / ratio)
		{
			m[number].PosX = pointAX + distanceX * sin(��(m[number].TimeAxis * ratio));
			m[number].PosY = pointAY + distanceY * sin(��(m[number].TimeAxis * ratio));
		}
		else if (m[number].TimeAxis >= 90 / ratio)
		{
			//�e���i���@�_��3way�j   
			if (times < 10)
			{
				if (m[number].TimeAxis % blazInterval == blazInterval - blazMesh)
				{
					angle = ((p->PosX - m[number].PosX < 0) ? acos((p->PosY - m[number].PosY) / distanceMP)
						: -acos((p->PosY - m[number].PosY) / distanceMP));
				}
				if (m[number].TimeAxis % blazInterval >= 0 && m[number].TimeAxis % blazInterval < blazLong * blazMesh && !(m[number].TimeAxis % blazMesh) && (p->PhaseFlag == 0 || p->PhaseFlag == 3))
				{
					for (int j = 0; j < way; j++)
					{
						for (int k = 0; k < NUM; k++)
						{
							if (b[k].NumFlag == -1)
							{
								b[k].NumFlag = number + 1;
								b[k].PosX = m[number].PosX;
								b[k].PosY = m[number].PosY;
								b[k].Velo = 14;
								b[k].Angle = angle - (1 - j) * ��(30) + ��(90);
								break;
							}
						}
					}
				}
				if (m[number].TimeAxis > blazInterval && m[number].TimeAxis % blazInterval == blazInterval - blazMesh)
				{
					times++;
				}
			}
			//�G�@�ړ�����
			m[number].PosX += escVeloX;
			m[number].PosY += escVeloY;
			//��ʊO����
			if (!range(0, m[number].PosX, m[number].PosY, 0, 0, m[number].Upper + 10, 0, 0, WIN_WIDTH, 0, WIN_HEIGHT))
			{
				m[number].Remain = -1;
			}
		}
		//���@�e�Փ˔��菈��
		for (int i = 0; i < NUM; i++)
		{
			if (b[i].NumFlag == 0)
			{
				if (range(1, b[i].PosX, b[i].PosY, m[number].PosX, m[number].PosY, b[i].Radius, m[number].Upper + 10, 0, 0, 0, 0))
				{
					b[i].HitFlag = 1;
					m[number].Remain -= (p->PhaseFlag == 0 ? 1 : 3);
				}
			}
		}
		//�����蔻��
		if (!p->PhaseFlag && p->Remain >= 0 && !keys[KEY_INPUT_X])
		{
			if (range(1, p->PosX, p->PosY, m[number].PosX, m[number].PosY, 5, 16, 0, 0, 0, 0))
			{
				p->PhaseFlag = 1;
			}
		}
		//�A�j���[�V��������
		if (m[number].TimeAxis >= 0)
		{
			vectorMPX = p->PosX - m[number].PosX;
			vectorMPY = p->PosY - m[number].PosY;
			cross1 = -1 * vectorMPY - sqrt(3) * vectorMPX;
			cross2 = 1 * vectorMPY - sqrt(3) * vectorMPX;
			if (cross1 < 0 && cross2 < 0)
			{
				m[number].FlagLR = 1;
			}
			else if (cross1 > 0 && cross2 > 0)
			{
				m[number].FlagLR = 2;
			}
			else
			{
				m[number].FlagLR = 0;
			}
		}
		m[number].TimeAxis++;
		if (m[number].Remain < 0)
		{
			m[number].TimeAxis = 0;
			times = 0;
		}
	}
}

void arakune_update(int number, int Remain, float pointAX, float pointAY, float moveVeloY, state_t* ar, state_t* p, blaz_state_t* b, int arGraphHandle, int* explo_blueGraphHandle)	//�r��(arakune)
{
	int blazMesh0 = 5, blazLong0 = 1, blazInterval0 = 12, way0 = 2;
	int blazMesh1 = 3, blazLong1 = 6, blazInterval1 = 45, way1 = 1;
	static int InitTime;
	double distance = 0;
	static double angle = 0;
	if (ar[number].PhaseFlag >= 0)	//�N�_��PhaseFlag
	{
		if (ar[number].TimeAxis == 0)
		{
			ar[number].PosX = pointAX;
			ar[number].PosY = pointAY;
			ar[number].Left = 64;
			ar[number].Upper = 48;
			ar[number].numofTimes[0] = 0;
			ar[number].numofTimes[1] = 0;
			ar[number].Remain = Remain;
		}
		//�ړ�����
		if (ar[number].PhaseFlag == 0)
		{
			ar[number].PosY += 10;
		}
		else if (ar[number].PhaseFlag == 1)
		{
			ar[number].PosY += moveVeloY;
		}
		//��ʓ��ɓ�������PhaseFlag=1
		if (ar[number].PhaseFlag == 0 && range(0, ar[number].PosX, ar[number].PosY, 0, 0, ar[number].Left, 0, 0, WIN_WIDTH, 0, WIN_HEIGHT))
		{
			ar[number].PhaseFlag = 1;
		}
		//�e����
		if (p->PhaseFlag == 0 || p->PhaseFlag == 3)
		{
			//���e��(�����΂�܂�)	//�E���e��NumFlag=12+number,�����e��NumFlag=15+number
			if (ar[number].numofTimes[0] < 72)
			{
				if (ar[number].TimeAxis % blazInterval0 == 0)
				{
					ar[number].BlazTimer[0] = 0;
					ar[number].numofLengs[0] = 0;
				}
				if (ar[number].BlazTimer[0] % blazMesh0 == 0 && ar[number].numofLengs[0] < blazLong0)
				{
					for (int i = 0; i < way0; i++)
					{
						//�E��
						for (int j1 = 0; j1 < NUM; j1++)
						{
							if (b[j1].NumFlag == -1)
							{
								b[j1].NumFlag = 12 + number;
								b[j1].PosX = ar[number].PosX - ar[number].Left + 8;
								b[j1].PosY = ar[number].PosY + ar[number].Upper - 4;	//�e�����W
								b[j1].Velo = 15;
								b[j1].Angle = ��((135 - (int)(45 * sin(��(45 * (ar[number].numofTimes[0] % 9))))) + 30 * i);
								break;
							}
						}
						//����
						for (int j2 = 0; j2 < NUM; j2++)
						{
							if (b[j2].NumFlag == -1)
							{
								b[j2].NumFlag = 15 + number;
								b[j2].PosX = ar[number].PosX + ar[number].Left - 8;
								b[j2].PosY = ar[number].PosY + ar[number].Upper - 4;	//�e�����W
								b[j2].Velo = 15;
								b[j2].Angle = ��(45 + (int)(45 * sin(��(45 * (ar[number].numofTimes[0] % 9)))) - 30 * i);
								break;
							}
						}
					}
					ar[number].numofLengs[0]++;
					if (ar[number].numofLengs[0] == blazLong0)
					{
						ar[number].numofTimes[0]++;
					}
				}
				ar[number].BlazTimer[0]++;
			}
			//�Őj(���@�_��)	//NumFlag=18+number
			if (ar[number].numofTimes[1] < 10)
			{
				if (ar[number].TimeAxis % blazInterval1 - blazMesh1 == 0)
				{
					ar[number].BlazTimer[1] = 0;
					ar[number].numofLengs[1] = 0;
					distance = sqrt(pow((double)ar[number].PosX - p->PosX, 2) + pow((double)ar[number].PosY - p->PosY, 2));
					angle = ((p->PosX - ar[number].PosX < 0) ? acos((p->PosY - ar[number].PosY) / distance) : -acos((p->PosY - ar[number].PosY) / distance));
				}
				if (ar[number].BlazTimer[1] % blazMesh1 == 0 && ar[number].numofLengs[1] < blazLong1)
				{
					for (int i = 0; i < way1; i++)
					{
						for (int j = 0; j < NUM; j++)
						{
							if (b[j].NumFlag == -1)
							{
								b[j].NumFlag = 18 + number;
								b[j].PosX = ar[number].PosX;
								b[j].PosY = ar[number].PosY;
								b[j].Velo = 15;
								b[j].Angle = angle + ��(90);
								break;
							}
						}
						ar[number].numofLengs[1]++;
					}
				}
				ar[number].BlazTimer[1]++;
			}
		}

		//���@�ڐG����
		if (!p->PhaseFlag && p->Remain >= 0 && !keys[KEY_INPUT_X])
		{
			if (range(0, p->PosX, p->PosY, 0, 0, 5, 0, ar[number].PosX - ar[number].Left, ar[number].PosX - ar[number].Left + 32, ar[number].PosY - ar[number].Upper, ar[number].PosY + ar[number].Upper)	//�E��
				|| range(0, p->PosX, p->PosY, 0, 0, 5, 0, ar[number].PosX + ar[number].Left - 32, ar[number].PosX + ar[number].Left, ar[number].PosY - ar[number].Upper, ar[number].PosY + ar[number].Upper)	//����
				|| range(0, p->PosX, p->PosY, 0, 0, 5, 0, ar[number].PosX - 16, ar[number].PosX + 16, ar[number].PosY - ar[number].Upper, ar[number].PosY + ar[number].Upper + 16))	//����
			{
				p->PhaseFlag = 1;
			}

		}

		//�_���[�W���菈��
		if (ar[number].PhaseFlag < 2)
		{
			for (int i = 0; i < NUM; i++)
			{
				if (b[i].NumFlag == 0)
				{
					if (range(0, b[i].PosX, b[i].PosY, 0, 0, b[i].Radius, 0
						, ar[number].PosX - ar[number].Left + 8, ar[number].PosX + ar[number].Left - 8
						, ar[number].PosY - ar[number].Upper, ar[number].PosY + ar[number].Upper - 8))
					{
						ar[number].Remain -= ((p->PhaseFlag == 0) ? 1 : 3);
						b[i].HitFlag = 1;
						if (ar[number].Remain < 0)
						{
							ar[number].PhaseFlag = 2;
							InitTime = ar[number].TimeAxis;
						}
					}
				}
			}
		}

		if (ar[number].PhaseFlag == 0 || ar[number].PhaseFlag == 1)
		{
			ar[number].GraphHandle = arGraphHandle;
		}
		else if (ar[number].PhaseFlag == 2 || ar[number].PhaseFlag == 3)
		{
			//���j���ŏ���
			ar[number].Left = ar[number].Upper = 48;
			if (ar[number].TimeAxis - InitTime <= 20)
			{
				ar[number].GraphHandle = explo_blueGraphHandle[(ar[number].TimeAxis - InitTime) / 2];
			}
			if (ar[number].TimeAxis - InitTime == 22)
			{
				ar[number].PhaseFlag = 3;
				ar[number].GraphHandle = explo_blueGraphHandle[10];
			}
			if (ar[number].TimeAxis - InitTime == 24)
			{
				ar[number].PhaseFlag = -1;
			}
		}

		//��ʊO����
		if (ar[number].PhaseFlag == 1)
		{
			if (!range(0, ar[number].PosX, ar[number].PosY, 0, 0, ar[number].Upper + 8, 0, 0, WIN_WIDTH, 0, WIN_HEIGHT))
			{
				ar[number].PhaseFlag = -1;
			}
		}

		//���Ŏ�����
		if (ar[number].PhaseFlag == -1)
		{
			ar[number].TimeAxis = -1;
			ar[number].numofTimes[0] = 0;
			ar[number].Remain = -1;
		}
		ar[number].TimeAxis++;
	}

}

void dugaw_update(int number, float Remain, float pointAX, float pointAY, float pointBX, float pointBY, int phase0deadline, float escVeloX, float escVeloY, state_t* du, state_t* p, blaz_state_t* b, int duGraphHandle, int* explo_blueGraphHandle)	//�W��(dugaw)
{
	int shotTiming = 30, equalTiming = 20;
	static int InitTime;
	float equalXsize = 75, equalYsize = 800;
	float muzzlePosX1, muzzlePosX2, muzzlePosY;	//muzzlePosX1:�E�e��,muzzlePosX2:���e��
	float blazLocateX[13] = { 0.35,0.45,1,0.92,0.6,0.42,0,-0.42,-0.6,-0.92,-1,-0.45,-0.35 };
	float blazLocateY[13] = { 0.1,0.21,0.36,0.55,0.72,0.92,1,0.92,0.72,0.55,0.36,0.21,0.1 };
	for (int i = 0; i < 13; i++)
	{
		blazLocateX[i] *= equalXsize;
		blazLocateY[i] *= equalYsize;
	}
	if (du[number].PhaseFlag >= 0)	//�N�_��PhaseFlag
	{
		if (du[number].TimeAxis == 0)
		{
			du[number].PosX = pointAX;
			du[number].PosY = pointAY;
			du[number].Left = du[number].Upper = 96;
			du[number].numofTimes[0] = 0;
			du[number].Remain = Remain;
		}
		if (du[number].PhaseFlag == 0)
		{
			du[number].PosX = pointAX + (pointBX - pointAX) * sin(��(90 * (1 - (float)(phase0deadline - du[number].TimeAxis) / phase0deadline)));
			if (du[number].TimeAxis >= phase0deadline)
			{
				du[number].PhaseFlag = 1;
			}
		}
		if (du[number].PhaseFlag == 1)
		{
			//�e���t�F�[�Y
			muzzlePosX1 = du[number].PosX - 24;
			muzzlePosX2 = du[number].PosX + 24;
			muzzlePosY = du[number].PosY + 80;
			if (du[number].TimeAxis >= phase0deadline + shotTiming)
			{
				//�f���e��
				for (int i = 0; i < 13; i++)
				{
					//�E�e��	//NumFlag=21+number
					for (int j = 0; j < NUM; j++)
					{
						if (b[j].NumFlag == -1)
						{
							double hypo, distance;
							b[j].NumFlag = 21 + number;
							b[j].PosX = muzzlePosX1;
							b[j].PosY = muzzlePosY;
							hypo = sqrt(pow((double)blazLocateX[i], 2) + pow((double)blazLocateY[i], 2));
							distance = sqrt(pow((double)blazLocateX[i], 2) + pow((double)blazLocateY[i] - muzzlePosY, 2));
							b[j].Velo = distance / equalTiming;
							b[j].Angle = ((blazLocateX[i] < 0) ? acos(blazLocateY[i] / hypo) : -acos(blazLocateY[i] / hypo)) + ��(90);
							break;
						}
					}
					//���e��	//NumFlag=24+number
					for (int j = 0; j < NUM; j++)
					{
						if (b[j].NumFlag == -1)
						{
							double hypo, distance;
							b[j].NumFlag = 24 + number;
							b[j].PosX = muzzlePosX2;
							b[j].PosY = muzzlePosY;
							hypo = sqrt(pow((double)blazLocateX[i], 2) + pow((double)blazLocateY[i], 2));
							distance = sqrt(pow((double)blazLocateX[i], 2) + pow((double)blazLocateY[i] - muzzlePosY, 2));
							b[j].Velo = distance / equalTiming;
							b[j].Angle = ((blazLocateX[i] < 0) ? acos(blazLocateY[i] / hypo) : -acos(blazLocateY[i] / hypo)) + ��(90);
							break;
						}
					}
				}
				du[number].PhaseFlag = 2;
			}
		}
		if (du[number].PhaseFlag == 2)
		{
			//�E�o�t�F�[�Y
			du[number].PosX += escVeloX;
			du[number].PosY += escVeloY;
			//��ʊO����
			if (!range(0, du[number].PosX, du[number].PosY, 0, 0, du[number].Left, 0, 0, WIN_WIDTH, 0, WIN_HEIGHT))
			{
				du[number].PhaseFlag = -1;
			}
		}

		if (du[number].PhaseFlag >= 0 && du[number].PhaseFlag <= 2)
		{
			du[number].GraphHandle = duGraphHandle;
		}

		//���@�ڐG����
		if (!p->PhaseFlag && p->Remain >= 0 && !keys[KEY_INPUT_X])
		{
			if (range(0, p->PosX, p->PosY, 0, 0, 5, 0, du[number].PosX - 32, du[number].PosX + 32, du[number].PosY - 64, du[number].PosY + 80)
				|| range(0, p->PosX, p->PosY, 0, 0, 5, 0, du[number].PosX - 64, du[number].PosX + 64, du[number].PosY - 64, du[number].PosY - 16))
			{
				p->PhaseFlag = 1;

			}
		}

		//��e����
		if (du[number].PhaseFlag >= 0 && du[number].PhaseFlag <= 2)
			for (int i = 0; i < NUM; i++)
			{
				if (b[i].NumFlag == 0)
				{
					if (range(0, b[i].PosX, b[i].PosY, 0, 0, b[i].Radius, 0, du[number].PosX - 64, du[number].PosX + 64, du[number].PosY - 80, du[number].PosY + 32))
					{
						du[number].Remain -= ((p->Remain == 0) ? 1 : 3);
						b[i].HitFlag = 1;
						if (du[number].Remain < 0)
						{
							du[number].PhaseFlag = 3;
							du[number].TimeAxis = InitTime;
						}
					}
				}
			}
		//���j���ŏ���
		if (du[number].PhaseFlag == 3 || du[number].PhaseFlag == 4)
		{
			du[number].Left = du[number].Upper = 48;
			if (du[number].TimeAxis - InitTime < 22)
			{
				du[number].GraphHandle = explo_blueGraphHandle[(du[number].TimeAxis - InitTime) / 2];
			}
			else if (du[number].TimeAxis - InitTime == 22)
			{
				du[number].PhaseFlag = 4;
				du[number].GraphHandle = explo_blueGraphHandle[10];
			}
			else if (du[number].TimeAxis - InitTime == 24)
			{
				du[number].PhaseFlag = -1;
			}
		}

		//���Ŏ�����
		if (du[number].PhaseFlag == -1)
		{
			du[number].TimeAxis = -1;
			du[number].numofTimes[0] = 0;
			du[number].Remain = -1;
		}
		du[number].TimeAxis++;
	}

}

void espadar_update(int number, float Remain, float pointAX, float pointAY, float pointBX, float pointBY, float pointCX, float pointCY, int phase0deadline, int phase1deadline, int phase2deadline, state_t* es, state_t* p, blaz_state_t* b, int* esGraphHandle)	//Espadar
{
	float escVeloX = 0, escVeloY = -5, blazVelo0 = 12;
	int way0 = 8, blazLengs1 = 6;
	static int InitTime;
	if (es[number].PhaseFlag >= 0)
	{
		if (es[number].PhaseFlag == 0)
		{
			//���~�t�F�[�Y
			if (es[number].TimeAxis == 0)
			{
				es[number].PosX = pointAX;
				es[number].PosY = pointAY;
				es[number].Upper = 128;
				es[number].Left = 256;
				es[number].numofTimes[0] = 0;
				es[number].numofTimes[1] = 0;
				es[number].Remain = Remain;
				es[number].GraphHandle = esGraphHandle[0];
			}
			es[number].PosX = pointAX + (pointBX - pointAX) * sin(��(90 * (1 - (float)(phase0deadline - es[number].TimeAxis) / phase0deadline)));
			es[number].PosY = pointAY + (pointBY - pointAY) * sin(��(90 * (1 - (float)(phase0deadline - es[number].TimeAxis) / phase0deadline)));
			if (es[number].TimeAxis == phase0deadline)
			{
				es[number].PhaseFlag = 1;
				es[number].PosX = pointBX;
				es[number].PosY = pointBY;
			}
		}
		if (es[number].PhaseFlag == 1)
		{
			//�a���t�F�[�Y
			int index = (es[number].TimeAxis - phase0deadline) / (phase1deadline / 2) + 1;
			es[number].GraphHandle = esGraphHandle[index];
			if (es[number].TimeAxis == phase0deadline + phase1deadline)
			{
				es[number].PhaseFlag = 2;
			}
		}
		if (es[number].PhaseFlag == 2)
		{
			//�㏸�t�F�[�Y
			es[number].PosX = pointBX + (pointCX - pointBX) * sin(��(90 * (1 - (float)(phase2deadline + phase1deadline + phase0deadline - es[number].TimeAxis) / phase2deadline)));
			es[number].PosY = pointBY + (pointCY - pointBY) * sin(��(90 * (1 - (float)(phase2deadline + phase1deadline + phase0deadline - es[number].TimeAxis) / phase2deadline)));
			es[number].GraphHandle = esGraphHandle[3];
			if (es[number].TimeAxis == phase0deadline + phase1deadline + phase2deadline)
			{
				es[number].PhaseFlag = 3;
			}
		}
		if (es[number].PhaseFlag == 3)
		{
			//�e���t�F�[�Y
			if (es[number].TimeAxis >= phase0deadline + phase1deadline + phase2deadline + 60 && es[number].numofTimes[0] < 1)
			{
				double distance = sqrt(pow((double)p->PosX - es[number].PosX, 2) + pow((double)p->PosY - es[number].PosY, 2));
				es[number].TimeAxis = esGraphHandle[3];
				//18way2�A�΂�܂�
				for (int i = 0; i < way0; i++)	//NumFlag=27+number
				{
					for (int j = 0; j < 2; j++)
					{
						for (int k = 0; k < NUM; k++)
						{
							if (b[k].NumFlag == -1)
							{
								b[k].NumFlag = 27 + number;
								b[k].PosX = es[number].PosX;
								b[k].PosY = es[number].PosY + 16;
								b[k].Angle = ��(45) + ��((float)90 / (way0 - 1) * i);
								b[k].Velo = (j == 1) ? blazVelo0 : (blazVelo0 * 2 / 3);
								break;
							}
						}
					}
				}
				//���@�_��
				for (int i = 0; i < blazLengs1; i++)	//NumFlag=30+number
				{
					for (int j = 0; j < NUM; j++)
					{
						if (b[j].NumFlag == -1)
						{
							b[j].NumFlag = 30 + number;
							b[j].PosX = es[number].PosX;
							b[j].PosY = es[number].PosY + 16;
							b[j].Angle = ((p->PosX - es[number].PosX < 0) ? acos((p->PosY - es[number].PosY) / distance) : -acos((p->PosY - es[number].PosY) / distance)) + ��(90);
							b[j].Velo = 2.5 * (i + 1);
							break;
						}
					}
				}
				es[number].numofTimes[0]++;
				es[number].PhaseFlag = 4;
			}
		}
		if (es[number].PhaseFlag == 4)
		{
			//�E�o�t�F�[�Y
			es[number].GraphHandle = esGraphHandle[3];
			es[number].PosX += escVeloX;
			es[number].PosY += escVeloY;
			//��ʊO����
			if (!range(0, es[number].PosX, es[number].PosY, 0, 0, es[number].Left, 0, 0, WIN_WIDTH, 0, WIN_HEIGHT))
			{
				es[number].PhaseFlag = -1;
			}
		}
		//��e����
		if (es[number].PhaseFlag >= 0 && es[number].PhaseFlag <= 4)
		{
			for (int i = 0; i < NUM; i++)
			{
				if (b[i].NumFlag == 0)
				{
					if (range(0, b[i].PosX, b[i].PosY, 0, 0, b[i].Radius, 0, es[number].PosX - 8, es[number].PosX + 8, es[number].PosY - 128, es[number].PosY + 32)
						|| range(0, b[i].PosX, b[i].PosY, 0, 0, b[i].Radius, 0, es[number].PosX - 48, es[number].PosX + 48, es[number].PosY - 112, es[number].PosY - 48))
					{
						es[number].Remain -= ((p->Remain == 0) ? 1 : 3);
						b[i].HitFlag = 1;
						if (es[number].Remain < 0)
						{
							es[number].PhaseFlag = 5;
							InitTime = es[number].TimeAxis;
						}
					}
				}
			}
		}
		//�����蔻��
		if (!p->PhaseFlag && p->Remain >= 0 && !keys[KEY_INPUT_X])
		{
			if (es[number].PhaseFlag == 0)
			{
				//���~�t�F�[�Y
				if (range(0, p->PosX, p->PosY, 0, 0, 5, 0, es[number].PosX - 72, es[number].PosX + 72, es[number].PosY - 88, es[number].PosY - 56)
					|| range(0, p->PosX, p->PosY, 0, 0, 5, 0, es[number].PosX - 56, es[number].PosX + 56, es[number].PosY - 112, es[number].PosY + 32)
					|| range(0, p->PosX, p->PosY, 0, 0, 5, 0, es[number].PosX - 64, es[number].PosX - 16, es[number].PosY + 32, es[number].PosY + 112))
				{
					p->PhaseFlag = 1;
				}
			}
			if (es[number].PhaseFlag == 1)
			{
				//�a���t�F�[�Y
				if (range(1, p->PosX, p->PosY, es[number].PosX, es[number].PosY - 126, 5, 264, 0, 0, 0, 0)
					&& !range(0, p->PosX, p->PosY, 0, 0, 5, 0, es[number].PosX - 264, es[number].PosX + 264, 0, es[number].PosY - 80)
					&& !range(0, p->PosX, p->PosY, 0, 0, 5, 0, es[number].PosX - 8, es[number].PosX + 8, es[number].PosY + 32, es[number].PosY + 80))
				{
					p->PhaseFlag = 1;
				}
			}
			if (es[number].PhaseFlag > 1 && es[number].PhaseFlag <= 2)
			{
				if (range(0, p->PosX, p->PosY, 0, 0, 5, 0, es[number].PosX - 232, es[number].PosX + 232, es[number].PosY - 80, es[number].PosY - 48)
					|| range(0, p->PosX, p->PosY, 0, 0, 5, 0, es[number].PosX - 112, es[number].PosX + 112, es[number].PosY - 56, es[number].PosY - 96)
					|| range(0, p->PosX, p->PosY, 0, 0, 5, 0, es[number].PosX - 8, es[number].PosX + 8, es[number].PosY - 128, es[number].PosY + 32))
				{
					p->PhaseFlag = 1;
				}
			}
		}

		//���j���ŏ���
		if (es[number].PhaseFlag == 5 || es[number].PhaseFlag == 6)
		{
			es[number].PhaseFlag = -1;
		}

		//���Ŏ�����
		if (es[number].PhaseFlag == -1)
		{
			es[number].TimeAxis = -1;
			es[number].numofTimes[0] = es[number].numofTimes[1] = 0;
			es[number].Remain = -1;
		}
		es[number].TimeAxis++;
	}
}