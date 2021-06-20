/*!
 * \file: GameMusic.h
 * \author: Daito
 * \date: 2021/06/17 19:43
 * \version: 1.0.0
 * \adress: https://blog.csdn.net/zhd5120153951
 * \e-mail: 2462491568@qq.com
 */

#pragma once
#include <Windows.h>
#include <iostream>
#include <random>
#include <ctime>
typedef int(__stdcall* w32mci)(const char*, char*, int, int);
typedef int(__stdcall* w32mcierror)(int, char*, int);
using namespace std;
class GameMusic
{
private:
	HINSTANCE hins;
	w32mci wmci;
	w32mcierror wmcierror;
public:
	GameMusic();
	~GameMusic();
	char buf[256];
	bool send(string command);
};
class AudioClip
{
public:
	AudioClip();
	~AudioClip();
	bool Load(const string& filename);
	bool Play(int start_ms = 0, int end_ms = -1);//���ź���,ȱʡ����
	bool Stop();//��ֹ--��������
	bool Pause();//��ͣ����
	bool Unpause();//��������
	int Millisecond();//��ʱ����

private:
	GameMusic gameMusic;
	string mFileName;
	string mAliaas;
	int mLength_ms;
};

