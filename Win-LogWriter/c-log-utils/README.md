# CLogUtils

#### 介绍

win32 日志助手单元

#### 软件架构

Win32, C++


#### 安装教程

在工程中包含头文件和源文件即可

#### 使用说明

```c++
#include <iostream>
#include "CLogUtils.h"

int _tmain(int argc, LPCTSTR argv[])
{
    CLogUtils::CLogHelper log;
    int nCount = 500000;
    uint64_t uBegin = 0;
    uint64_t uEnd = 0;

#if 1
    while (true)
    {
        uBegin = CLogUtils::GetCurrentTimestamp();
        std::cout << " CurrentTimestamp: " << uBegin << std::endl;
        for (int i = 0; i < nCount; i++)
        {
            log.Info(_T("%d %s"), 1024, _T("FlameCyclone"));
        }
        uEnd = CLogUtils::GetCurrentTimestamp();
        std::cout << "Repeat " << nCount << " Cost time: " << uEnd - uBegin << std::endl;
        system("pause");
    }

#else

    while (true)
    {
        uBegin = CLogUtils::GetCurrentTimestamp();
        std::cout << " CurrentTimestamp: " << uBegin << std::endl;
        for (int i = 0; i < nCount; i++)
        {
            CLogUtils::LOG_INFO(_T("%d %s"), 1024, _T("FlameCyclone"));
        }
        uEnd = CLogUtils::GetCurrentTimestamp();
        std::cout << "Repeat " << nCount << " Cost time: " << uEnd - uBegin << std::endl;
        system("pause");
    }

#endif

    return 0;
}

```

![](/Images/example.png)

#### 参与贡献

FlameCyclone
