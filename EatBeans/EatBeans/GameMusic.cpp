/*!
 * \file: GameMusic.cpp
 * \author: Daito
 * \date: 2021/06/17 19:44
 * \version: 1.0.0
 * \adress: https://blog.csdn.net/zhd5120153951
 * \e-mail: 2462491568@qq.com
 */

#define _CRT_SECURE_NO_WARNINGS
#include "GameMusic.h"

GameMusic::GameMusic()
{
	HINSTANCE hins = LoadLibraryA("winmm.dll");//加载库
	wmci = (w32mci)GetProcAddress(hins, "mciSendStringA");
	wmcierror = (w32mcierror)GetProcAddress(hins, "mciGetErrorStringA");
}

GameMusic::~GameMusic()
{
	FreeLibrary(hins);//释放库
}

bool GameMusic::send(string command)
{
	int errcode = wmci(command.c_str(), buf, 254, 0);
	if (errcode)
	{
		wmcierror(errcode, buf, 254);
		return false;
	}
	return true;
}

AudioClip::AudioClip()
{
	//无需构造
}

AudioClip::~AudioClip()
{
	string cmd;
	cmd = "close" + mAliaas;
	gameMusic.send(cmd);
}

bool AudioClip::Load(const string& filename)
{
	mFileName = filename;
	for (unsigned int i=0;i<mFileName.length();i++)
	{
		//把文件名的每个字符都显示出来
		if (mFileName[i]=='/')
			mFileName[i] = '\\';//这是把文件路径的斜杠替换成双返斜杠
	}
	mAliaas = "mp3_";
	srand(time(NULL));//种下时间种子,产生随机数
	char randstr[6];
	_itoa(rand() % 65536, randstr, 10);
	mAliaas.append(randstr);//把后缀mp3加到字符串中
	string cmd;
	cmd = "open" + mFileName + "mAliaas" + mAliaas;
	if (gameMusic.send(cmd)==false)
		return false;//发送歌曲失败
	mLength_ms = atoi(gameMusic.buf);//否则成功,而且得到歌曲的时长
	return true;
}

bool AudioClip::Play(int start_ms /*= 0*/, int end_ms /*= -1*/)//缺省参数start开始为0,结束默认为:没结束-1表示
{
	if (end_ms == -1)end_ms = mLength_ms;//时长则为前面的时长
	string cmd;
	char start_str[16], end_str[16];
	_itoa(start_ms, start_str, 10);
	_itoa(end_ms, end_str, 10);
	cmd = "play" + mAliaas + "from";
	cmd.append(start_str);
	cmd.append("to");
	cmd.append(end_str);
	return gameMusic.send(cmd);
	
}

bool AudioClip::Stop()
{
	string cmd;
	cmd = "stop" + mAliaas;
	if (gameMusic.send(cmd) == false)
		return false;
	cmd = "seek" + mAliaas + "to start";
	if (gameMusic.send(cmd) == false)
		return false;
	return true;
}

bool AudioClip::Pause()
{
	string cmd;
	cmd = "pause" + mAliaas;
	if (gameMusic.send(cmd) == false)
		return false;
	return true;
}

bool AudioClip::Unpause()
{
	string cmd;
	cmd = "unpause" + mAliaas;
	if (gameMusic.send(cmd) == false)
		return false;
	return true;
}

int AudioClip::Millisecond()
{
	return mLength_ms;
}
