#pragma once
#include "CTankEntry.h"
//这是通过Git Bash中的mingw-64模拟Linux下的vim文本编辑器编写的,完全没有用vs2017。
//这个很方便是因为编写完成后可以直接push，但是无法调试，这个可以在Linux下，配置gcc环境来实现。
class CBot :
	public CTankEntry
{
public:
	CBot();
	CBot(float x, float y, PCTSTR szImgName);
	virtual ~CBot();
};






