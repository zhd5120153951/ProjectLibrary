# CLogUtils

#### Description

win32 Logger utils

#### Software Architecture

Win32, C++


#### Installation

Just include the header files and source files in the project

#### Instructions

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

#### Contribution

FlameCyclone