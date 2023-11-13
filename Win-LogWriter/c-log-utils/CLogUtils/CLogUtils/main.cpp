#include <iostream>
#include <algorithm>
#include <strsafe.h>
#include <stdio.h>
#include "CLogUtils.h"

void PrintHelp();
void PrintHelp_en_us();
void PrintHelp_zh_cn();
void CLogUtilsTest(int nTestCount, _tstring strPipe, _tstring strDir, _tstring strName, int nFileCount, int nFileSize, int nBufCount, int nTimeOut);

int _tmain(int argc, LPCTSTR argv[])
{
    ::setlocale(LC_ALL, "");

    _tstring strLogDir;
    _tstring strLogName;
    _tstring strPipeName;
    DWORD nTestCount = 100000;
    DWORD nFileSize = 0;
    DWORD nTempCount = 0;
    DWORD nTimeout = 0;
    DWORD nFileCount = 0;

    if (argc <= 1)
    {
        PrintHelp();
        return -1;
    }

    for (int i = 1; i < argc; i++)
    {
        _tstring strSwitchValue = argv[i];

        if (!(_T('-') == strSwitchValue[0] || _T('/') == strSwitchValue[0]))
        {
            continue;
        }

        _tstring strHelp = strSwitchValue.substr(1);
        if (strHelp == _T("help") || strHelp == _T("?"))
        {
            PrintHelp();
            return 0;
        }

        size_t nEqual = strSwitchValue.find(_T('='));
        if (_tstring::npos == nEqual)
        {
            continue;
        }

        _tstring strName = strSwitchValue.substr(1, nEqual - 1);
        _tstring strValue = strSwitchValue.substr(nEqual + 1);

        std::transform(strName.begin(), strName.end(), strName.begin(), [](TCHAR ch)->TCHAR {
            if (ch >= _T('A') && ch <= _T('Z')) ch &= ~0x20;
            return ch;
            });

        if (strName == _T("d"))
        {
            strLogDir = strValue;
            continue;
        }

        if (strName == _T("n"))
        {
            strLogName = strValue;
            continue;
        }

        if (strName == _T("a"))
        {
            _stscanf_s(strValue.c_str(), _T("%d"), &nTestCount);
            continue;
        }

        if (strName == _T("p"))
        {
            strPipeName = strValue;
            continue;
        }

        if (strName == _T("f"))
        {
            _stscanf_s(strValue.c_str(), _T("%d"), &nFileCount);
            continue;
        }

        if (strName == _T("s"))
        {
            _stscanf_s(strValue.c_str(), _T("%d"), &nFileSize);
            continue;
        }

        if (strName == _T("c"))
        {
            _stscanf_s(strValue.c_str(), _T("%d"), &nTempCount);
            continue;
        }

        if (strName == _T("t"))
        {
            _stscanf_s(strValue.c_str(), _T("%d"), &nTimeout);
            continue;
        }
    }

    CLogUtilsTest(nTestCount, strPipeName, strLogDir, strLogName, nFileCount, nFileSize, nTempCount, nTimeout * 1000);

    return 0;
}

void CLogUtilsTest(int nTestCount, _tstring strPipe, _tstring strDir, _tstring strName, int nFileCount, int nFileSize, int nBufCount, int nTimeOut)
{
    uint64_t uBegin = 0;
    uint64_t uEnd = 0;
    uint64_t uCost = 0;
    double lfSpeed = 0.0f;

    if (!strPipe.empty())
    {
        CLogUtils::CLogHelper log(true, strPipe, strDir, strName, nFileCount, nFileSize, nTimeOut, nBufCount);
        uBegin = CLogUtils::GetCurrentTimestamp();

        for (int i = 0; i < nTestCount; i++)
        {
            log.Info(_T("%d %s"), 1024, _T("FlameCyclone"));
        }
        log.FlushBuffers();

        uEnd = CLogUtils::GetCurrentTimestamp();
        uCost = uEnd - uBegin;
        lfSpeed = (double)nTestCount * 1000 / (uCost);

        _tprintf(
            _T("Repeat %d tims, cost time: %lld, speed: %lf/S\r\n"),
            nTestCount,
            uCost,
            lfSpeed
        );
    }
    else
    {
        CLogUtils::CLogHelper log(false, strPipe, strDir, strName, nFileCount, nFileSize, nTimeOut, nBufCount);

        uBegin = CLogUtils::GetCurrentTimestamp();

        for (int i = 0; i < nTestCount; i++)
        {
            log.Info(_T("%d %s"), 1024, _T("FlameCyclone"));
        }
        log.FlushBuffers();

        uEnd = CLogUtils::GetCurrentTimestamp();
        uCost = uEnd - uBegin;
        lfSpeed = (double)nTestCount * 1000 / (uCost);

        _tprintf(
            _T("Repeat %d tims, cost time: %lld, speed: %lf/S\r\n"),
            nTestCount,
            uCost,
            lfSpeed
        );
    }
}

void PrintHelp()
{
    LANGID langID = ::GetThreadUILanguage();

    if (LANG_CHINESE == PRIMARYLANGID(langID))
    {
        PrintHelp_zh_cn();
    }
    else
    {
        PrintHelp_en_us();
    }
}

void PrintHelp_en_us()
{
    std::wcout << _T("How to use:") << std::endl;
    std::wcout << _T("CLogUtils -options=value") << std::endl;
    std::wcout << std::endl;

    std::wcout << _T("-p=[number]:        Enable pipe mode and specify the pipe name.") << std::endl;
    std::wcout << _T("-a=[number]:        Number of test output logs.") << std::endl;
    std::wcout << _T("-d=[string]:        The log file output directory.") << std::endl;
    std::wcout << _T("-n=[string]:        The log file name prefix.") << std::endl;
    std::wcout << _T("-f=[number]:        Number of log files.") << std::endl;
    std::wcout << _T("-s=[number]:        Single log file size(bytes).") << std::endl;
    std::wcout << _T("-c=[number]:        Number of log caches.") << std::endl;
    std::wcout << _T("-t=[number]:        Automatically save logs to a file timeout value(seconds).") << std::endl;
    std::wcout << _T("-?/-help:           Help for CLogSrv.") << std::endl;
}

void PrintHelp_zh_cn()
{
    std::wcout << _T("使用方法:") << std::endl;
    std::wcout << _T("CLogUtils.exe -选项=值") << std::endl;
    std::wcout << std::endl;

    std::wcout << _T("-p=[字符串]:        启用管道模式并指定管道名.") << std::endl;
    std::wcout << _T("-a=[整数]:          测试输出日志数量.") << std::endl;
    std::wcout << _T("-d=[字符串]:        日志输出目录.") << std::endl;
    std::wcout << _T("-n=[字符串]:        日志文件名前缀.") << std::endl;
    std::wcout << _T("-f=[整数]:          日志文件数量.") << std::endl;
    std::wcout << _T("-s=[整数]:          单个日志文件大小(字节).") << std::endl;
    std::wcout << _T("-c=[整数]:          日志缓冲数量.") << std::endl;
    std::wcout << _T("-t=[整数]:          自动保存超时间隔(秒).") << std::endl;
    std::wcout << _T("-?/-help:           日志单元帮助.") << std::endl;
}
