#include "CLogUtils.h"
#include <strsafe.h>
#include <tchar.h>
#include <future>

namespace CLogUtils
{
#define DEFAULT_LOG_FILE_COUNT_MIN                  (4)                     //默认最少日志文件历史数量
#define DEFAULT_LOG_TIMEOUT_MIN                     (1000 * 5)              //默认自动保存超时时间(毫秒)
#define DEFAULT_LOG_FILE_SIZE_MIN                   (1024 * 1024 * 1)       //默认单个日志文件大小阈值(字节)
#define DEFAULT_LOG_BUF_COUNT_MIN                   (256)                   //默认日志缓冲数量阈值

#define DEFAULT_LOG_FILE_COUNT_MAX                  (256)                   //默认最多日志文件历史数量
#define DEFAULT_LOG_TIMEOUT_MAX                     (1000 * 60)             //默认自动保存超时时间(毫秒)
#define DEFAULT_LOG_FILE_SIZE_MAX                   (1024 * 1024 * 256)     //默认单个日志文件大小阈值(字节)
#define DEFAULT_LOG_BUF_COUNT_MAX                   (1024 * 4)              //默认日志缓冲数量阈值

#define PIPE_NAME_PREFIX                            TEXT(R"(\\.\pipe\)")    //管道前缀名
#define PIPE_MAX_TIMEOUT                            (3000)                  //管道打开超时
#define PIPE_BUF_MAX_SIZE                           (1024 * 1024)           //管道发送缓冲大小(字节)
#define PIPE_MAX_CONNECT                            (1)                     //管道最大实例数量

    typedef struct _PIPE_DATA
    {
        DWORD dwSize = 0;
        BYTE data[0];
    }PIPE_DATA, * PPIPE_DATA;

    //全局实例构造
    CLogHelper global_logger(
        false,
        _T(""),
        _T(""),
        GetCurrentModuleName(true) + _T("_global"),
        LOG_FILE_SIZE,
        LOG_BUF_COUNT,
        LOG_TIMEOUT,
        LOG_FILE_COUNT
    );

    CLogHelper::CLogHelper(
        bool bPipeMode/* = false*/,
        const _tstring& strPipeName/* = _T("")*/,
        const _tstring& strDir/* = _T("")*/,
        const _tstring& strPreName/* = _T("")*/,
        DWORD nFileSize/* = LOG_FILE_SIZE*/,
        DWORD nTmpCount/* = LOG_BUF_COUNT*/,
        DWORD nTimeout/* = LOG_TIMEOUT*/,
        DWORD nFileCount/* = LOG_FILE_COUNT*/
    ) :
        m_bPipeMode(bPipeMode),
        m_strPipeName(strPipeName),
        m_strSaveDir(strDir),
        m_strSaveName(strPreName),
        m_nFileSize(nFileSize),
        m_nTempCount(nTmpCount),
        m_nTimeout(nTimeout),
        m_nFileCount(nFileCount)
    {
        //最小参数限制
        if (m_nFileSize < DEFAULT_LOG_FILE_SIZE_MIN)
        {
            m_nFileSize = DEFAULT_LOG_FILE_SIZE_MIN;
        }

        if (m_nFileCount < DEFAULT_LOG_FILE_COUNT_MIN)
        {
            m_nFileCount = DEFAULT_LOG_FILE_COUNT_MIN;
        }

        if (m_nTempCount < DEFAULT_LOG_BUF_COUNT_MIN)
        {
            m_nTempCount = DEFAULT_LOG_BUF_COUNT_MIN;
        }

        if (m_nTimeout < DEFAULT_LOG_TIMEOUT_MIN)
        {
            m_nTimeout = DEFAULT_LOG_TIMEOUT_MIN;
        }

        //最大参数限制
        if (m_nFileSize > DEFAULT_LOG_FILE_SIZE_MAX)
        {
            m_nFileSize = DEFAULT_LOG_FILE_SIZE_MAX;
        }

        if (m_nFileCount > DEFAULT_LOG_FILE_COUNT_MAX)
        {
            m_nFileCount = DEFAULT_LOG_FILE_COUNT_MAX;
        }

        if (m_nTempCount > DEFAULT_LOG_BUF_COUNT_MAX)
        {
            m_nTempCount = DEFAULT_LOG_BUF_COUNT_MAX;
        }

        if (m_nTimeout > DEFAULT_LOG_TIMEOUT_MAX)
        {
            m_nTimeout = DEFAULT_LOG_TIMEOUT_MAX;
        }

        //默认目录为当前进程目录
        if (m_strSaveDir.empty())
        {
            m_strSaveDir = GetCurrentModuleDir();
        }

        //默认文件名为当前进程名
        if (m_strSaveName.empty())
        {
            m_strSaveName = GetCurrentModuleName(true);
        }

        //目录不存在就创建目录
        if (!IsDirectory(m_strSaveDir))
        {
            CreateDir(m_strSaveDir);
        }

        if (m_strPipeName.empty())
        {
            m_strPipeName = _GetDefaultPipeName();
        }

        this->_Initialize();
    }

    CLogHelper::~CLogHelper()
    {
        this->_Uninitialize();

        if (nullptr != m_pRecvPipe)
        {
            delete m_pRecvPipe;
            m_pRecvPipe = nullptr;
        }

        if (nullptr != m_pSendPipe)
        {
            delete m_pSendPipe;
            m_pSendPipe = nullptr;
        }
    }

    void CLogHelper::DoNothing()
    {
    }

    bool CLogHelper::_Initialize()
    {
        if (m_bPipeMode)
        {
            m_pRecvPipe = new (std::nothrow) CNamedPipe;
            m_pSendPipe = new (std::nothrow) CNamedPipe;

            if (nullptr == m_pRecvPipe)
            {
                return false;
            }

            if (nullptr == m_pSendPipe)
            {
                return false;
            }

            std::promise<bool> m;
            std::future<bool> p = m.get_future();

            m_tPipeRecvTask = std::move(
                std::thread([this, &m]() -> void
                    {
                        m.set_value(true);
                        _ProcessByPipe();
                    }
                )
            );

            //等待线程启动完毕
            p.get();
        }
        else
        {
            if (nullptr == m_hEvent)
            {
                m_hEvent = ::CreateEvent(nullptr, false, false, nullptr);
            }

            if (nullptr == m_hEvent)
            {
                return false;
            }

            std::promise<bool> m;
            std::future<bool> p = m.get_future();

            m_tAutoSaveTask = std::move(
                std::thread([this, &m]()
                    {
                        m.set_value(true);
                        _ProcessTimeoutSave();
                    }
                )
            );

            //等待线程启动完毕
            p.get();
        }

        return true;
    }

    void CLogHelper::_Uninitialize()
    {
        if (!m_logList.empty() || !m_strLogContent.empty())
        {
            FlushBuffers();
        }

        if (INVALID_HANDLE_VALUE != m_hFile)
        {
            ::CloseHandle(m_hFile);
            m_hFile = INVALID_HANDLE_VALUE;
        }

        if (nullptr != m_hEvent)
        {
            m_bStop = true;
            ::SetEvent(m_hEvent);
            m_hEvent = nullptr;
        }

        if (m_tAutoSaveTask.joinable())
        {
            m_tAutoSaveTask.join();
        }

        if (m_bPipeMode)
        {
            if (m_pRecvPipe)
            {
                m_pRecvPipe->Close();
            }

            if (m_pSendPipe)
            {
                m_pSendPipe->Close();
            }

            if (m_tPipeRecvTask.joinable())
            {
                m_tPipeRecvTask.join();
            }
        }
    }

    bool CLogHelper::_LoggingByPipe(const _tstring& strLogContent)
    {
        bool bSuccess = false;

        if (nullptr == m_pSendPipe)
        {
            return false;
        }

        if (!m_pSendPipe->IsValid())
        {
            if (!m_pSendPipe->Open(m_strPipeName.c_str(), 1000))
            {
                return false;
            }
        }

        //写入日志内容到管道, 交给另一端处理(可用跨进程)
        bSuccess = m_pSendPipe->Write(strLogContent.c_str(), (DWORD)((strLogContent.size() + 1) * sizeof(TCHAR)));
        if (!bSuccess)
        {
            m_pSendPipe->Close();
        }

        return bSuccess;
    }

    bool CLogHelper::_FlushLogBuffers()
    {
        DWORD dwNumberOfBytesWrite = 0;
        bool bSuccess = false;

        if (INVALID_HANDLE_VALUE == m_hFile)
        {
            return false;
        }

#if USR_STRING_BUFFER_CONTENT
        //没有需要写入的日志
        if (m_strLogContent.empty())
        {
            return true;
        }

        bSuccess = ::WriteFile(m_hFile, m_strLogContent.c_str(), (DWORD)(m_strLogContent.size() * sizeof(TCHAR)), &dwNumberOfBytesWrite, NULL);
        m_strLogContent.clear();
#else
        //没有需要写入的日志
        if (m_logList.empty())
        {
            return true;
        }

        for (const auto& item : m_logList)
        {
            bSuccess = ::WriteFile(m_hFile, item.c_str(), (DWORD)(item.size() * sizeof(TCHAR)), &dwNumberOfBytesWrite, NULL);
            if (!bSuccess)
            {
                break;
            }
        }

#endif

        return bSuccess;
    }

    bool CLogHelper::_LoggingContent(const _tstring& strLogContent)
    {
        //获取单行日志内容 + 固定前缀内容 + 真实内容
        m_nNextItemSize = (DWORD)(strLogContent.size() * sizeof(TCHAR));

        std::lock_guard<std::mutex> lock(m_Lock);

        //首次启动时, 重置大小统计
        if (!m_bFirst)
        {
            _InitLogFile();
            _AdjustLogFile();
            m_nCurFileSize = (DWORD)GetFileSize(m_strFilePath);
            m_bFirst = true;
        }

        //单个日志文件大小即将达到或超过阈值则输出到文件, 启用新的文件存储
        if ((m_nCurFileSize + m_nNextItemSize) >= m_nFileSize)
        {
            _FlushLogBuffers();
            m_logList.clear();

            ::CloseHandle(m_hFile);
            m_hFile = INVALID_HANDLE_VALUE;

            (void)_GenerateLogFilePath();
            m_nCurFileSize = (DWORD)GetFileSize(m_strFilePath);
            _AdjustLogFile();
        }

        //已缓存条目达到阈值则输出到文件
#if USR_STRING_BUFFER_CONTENT
        else if (m_nLogItemCount >= m_nTempCount)
        {
            _FlushLogBuffers();
            m_strLogContent.clear();
            m_nLogItemCount = 0;
        }
#else
        else if (m_logList.size() >= m_nTempCount)
        {
            _FlushLogBuffers();
            m_logList.clear();
        }

#endif

#if USR_STRING_BUFFER_CONTENT
        m_strLogContent += strLogContent;
        m_nLogItemCount++;
#else
        m_logList.emplace_back(strLogContent);
#endif

        //累加统计单个日志文件大小
        m_nCurFileSize += m_nNextItemSize;
        return true;
    }

    bool CLogHelper::_ProcessTimeoutSave()
    {
        while (!m_bStop)
        {
            DWORD dwWait = ::WaitForSingleObject(m_hEvent, m_nTimeout);
            switch (dwWait)
            {
            case WAIT_TIMEOUT:
            case WAIT_OBJECT_0:
            {
                std::lock_guard<std::mutex> lock(m_Lock);
                this->_FlushLogBuffers();
                m_logList.clear();
            }
            break;
            default:
                break;
            }
        }

        return true;
    }

    bool CLogHelper::_ProcessByPipe()
    {
        if (m_pRecvPipe->IsValid())
        {
            return true;
        }

        if (!m_pRecvPipe->Create(m_strPipeName.c_str()))
        {
            return false;
        }

        DWORD dwCchCount = 1024 * 512;
        LPTSTR lpData = nullptr;
        bool bFailed = false;

        lpData = (LPTSTR)::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, dwCchCount * sizeof(TCHAR));
        if (nullptr == lpData)
        {
            return false;
        }

        while (!m_bStop && !bFailed)
        {
            if (!m_pRecvPipe->WaitConnect())
            {
                _tprintf(_T("WaitConnect failed!\r\n"));
                break;
            }

            while (!m_bStop)
            {
                bool isSuccess = m_pRecvPipe->Read(lpData, dwCchCount * sizeof(TCHAR));
                if (isSuccess)
                {
                    _LoggingContent(lpData);
                }

                if (ERROR_BROKEN_PIPE == ::GetLastError())
                {
                    m_pRecvPipe->Disconnect();
                    if (!m_pRecvPipe->WaitConnect())
                    {
                        _tprintf(_T("WaitConnect failed!\r\n"));
                        bFailed = true;
                        break;
                    }
                }

                if (!m_pRecvPipe->IsValid())
                {
                    break;
                }
            }
        }

        this->_FlushLogBuffers();
        m_logList.clear();

        if (nullptr != lpData)
        {
            ::HeapFree(::GetProcessHeap(), 0, lpData);
            lpData = nullptr;
        }

        return true;
    }

    void CLogHelper::Logging(
        LPCTSTR pstrLevel,
        LPCTSTR pstrFile,
        LPCTSTR pstrFunc,
        UINT nLine,
        LPCTSTR pstrFormat,
        ...
    )
    {
        if (nullptr == pstrFormat)
        {
            return;
        }

        TCHAR szBuf[MAX_PATH] = { 0 };
        DWORD dwPid = ::GetCurrentProcessId();
        DWORD dwTid = ::GetCurrentThreadId();
        _tstring strLogContent;

        //日志格式前缀 [时间] [等级] [十进制进程ID:十进制线程ID] [源码位置:行数] [函数名]
        ::StringCchPrintf(
            szBuf,
            _countof(szBuf),
            _T("%s %s [%d:%d] [%s:%d] [%s] "),
            GetCurrentTimeString().c_str(),
            pstrLevel,
            dwPid,
            dwTid,
            pstrFile,
            nLine,
            pstrFunc
        );

        strLogContent = szBuf;

        LPTSTR lpBuf = nullptr;
        DWORD dwCchCount = MAX_PATH;

        va_list args;
        va_start(args, pstrFormat);
        do
        {
            //分配缓冲
            lpBuf = (LPTSTR)::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, dwCchCount * sizeof(TCHAR));
            if (nullptr == lpBuf)
            {
                break;
            }

            //成功则赋值字符串并终止循环
            if (-1 != _vsntprintf_s(lpBuf, dwCchCount, _TRUNCATE, pstrFormat, args))
            {
                strLogContent += lpBuf;
                break;
            }

            //释放缓冲, 待下次重新分配
            ::HeapFree(::GetProcessHeap(), 0, lpBuf);
            lpBuf = nullptr;

            //缓冲字符数在合理范围内则扩大2倍
            if (dwCchCount < INT32_MAX)
            {
                dwCchCount *= 2;
            }
            else
            {
                //超限终止处理
                break;
            }

        } while (true);
        va_end(args);

        //释放缓冲
        if (nullptr != lpBuf)
        {
            ::HeapFree(::GetProcessHeap(), 0, lpBuf);
            lpBuf = nullptr;
        }

        strLogContent += _T("\r\n");

        if (m_bPipeMode)
        {
            _LoggingByPipe(strLogContent);
        }
        else
        {
            _LoggingContent(strLogContent);
        }
    }

    void CLogHelper::Clear()
    {
        std::lock_guard<std::mutex> lock(m_Lock);

        if (INVALID_HANDLE_VALUE != m_hFile)
        {
            ::CloseHandle(m_hFile);
            m_hFile = INVALID_HANDLE_VALUE;
        }

        m_logFileList = _GetLogFileList(m_strSaveDir);
        for (const auto& item : m_logFileList)
        {
            DeleteArchive(item.second);
        }

        m_logFileList.clear();
    }

    bool CLogHelper::FlushBuffers()
    {
        std::lock_guard<std::mutex> lock(m_Lock);
        return _FlushLogBuffers();
    }

    void CLogHelper::_AdjustLogFile()
    {
        //检查文件数量是否到达阈值, 到达的话删除前面的文件
        if (m_logFileList.size() > m_nFileCount)
        {
            size_t nDeleteCount = m_logFileList.size() - m_nFileCount;

            //从日志文件记录列表中删除
            for (size_t i = 0; i < nDeleteCount; i++)
            {
                auto itBegin = m_logFileList.begin();
                DeleteArchive(itBegin->second);
                m_logFileList.erase(m_logFileList.begin());
            }
        }
    }

    void CLogHelper::_InitLogFile()
    {
        //如果上次最后一个日志文件大小还能存储日志, 就沿用上次的日志文件
        m_logFileList = _GetLogFileList(m_strSaveDir);
        if (!m_logFileList.empty())
        {
            auto itLast = m_logFileList.end();
            itLast--;

            m_nFileTimetamp = itLast->first;
            m_strFilePath = itLast->second;

            //上次最后一个日志文件不能存储更多日志, 则生成新的日志文件路径
            unsigned long long ullFileSize = (DWORD)GetFileSize(m_strFilePath);
            if ((ullFileSize + m_nNextItemSize) >= m_nFileSize)
            {
                (void)_GenerateLogFilePath();
            }
            else
            {
                //打开文件以续写日志
                m_hFile = CreateFile(
                    m_strFilePath.c_str(),
                    GENERIC_WRITE,
                    FILE_SHARE_READ,
                    NULL,
                    OPEN_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL
                );

                //在文件末尾追加内容
                LARGE_INTEGER  liDistanceToMove = { 0 };
                ::SetFilePointerEx(m_hFile, liDistanceToMove, NULL, FILE_END);
            }
        }
        else
        {
            (void)_GenerateLogFilePath();
        }
    }

    //
    // @brief: 获取日志管道名
    // @param: void
    // @ret: _tstring 管道名
    _tstring CLogHelper::_GetDefaultPipeName() const
    {
        return Format(_T("%s_%s"), m_strSaveDir.c_str(), m_strSaveName.c_str());
    }

    void CLogHelper::_GenerateLogFilePath()
    {
        //得到日志文件时间戳
        m_nFileTimetamp = GetCurrentTimestamp();

        //得到日志文件路径
        m_strFilePath = Format(_T("%s\\%s_%s.log"),
            m_strSaveDir.c_str(),
            m_strSaveName.c_str(),
            TimestampToString(m_nFileTimetamp, _T("%04d-%02d-%02d_%02d-%02d-%02d-%03d")).c_str()
        );

        //创建一下文件(防止在资源管理器中看不到新的日志文件)
        m_hFile = CreateFile(
            m_strFilePath.c_str(),
            GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

        //在文件末尾追加内容
        LARGE_INTEGER  liDistanceToMove = { 0 };
        ::SetFilePointerEx(m_hFile, liDistanceToMove, NULL, FILE_END);

        m_logFileList.insert(std::make_pair(m_nFileTimetamp, m_strFilePath));
    }

    CNamedPipe::CNamedPipe() :
        m_pBuffer(nullptr),
        m_hNamedPipe(INVALID_HANDLE_VALUE),
        m_hReadEvent(NULL),
        m_hWriteEvent(NULL),
        m_bConnected(false),
        m_bInit(false)
    {
        //初始化读写缓冲与事件句柄
        Initialize();
    }

    CNamedPipe::~CNamedPipe()
    {
        //释放读写缓冲与事件句柄
        Uninitialize();
    }

    bool CNamedPipe::Create(LPCTSTR lpName)
    {
        TCHAR szPipeName[MAX_PATH];
        SECURITY_ATTRIBUTES sa = { 0 };
        SECURITY_DESCRIPTOR sd = { 0 };
        bool isSuccess = false;

        sa.nLength = sizeof(sa);
        sa.bInheritHandle = FALSE;
        sa.lpSecurityDescriptor = &sd;

        if (INVALID_HANDLE_VALUE != m_hNamedPipe)
        {
            return true;
        }

        //设置权限, 防止低权限进程不能打开高权限进程创建的管道
        (void)::InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
        (void)::SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
        (void)::StringCchPrintf(szPipeName, _countof(szPipeName), TEXT("%s%s"), PIPE_NAME_PREFIX, lpName);

        do
        {
            m_hNamedPipe = ::CreateNamedPipe(
                szPipeName,
                PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
                PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                PIPE_MAX_CONNECT,
                PIPE_BUF_MAX_SIZE,
                PIPE_BUF_MAX_SIZE,
                PIPE_MAX_TIMEOUT,
                &sa
            );

            if (INVALID_HANDLE_VALUE == m_hNamedPipe)
            {
                break;
            }

            isSuccess = true;

        } while (false);

        if (!isSuccess)
        {
            this->Close();
        }

        return isSuccess;
    }

    bool CNamedPipe::Open(LPCTSTR lpName, DWORD nTimeOut/* = INFINITE*/)
    {
        TCHAR szPipeName[MAX_PATH] = { 0 };
        bool isSuccess = false;

        (void)::StringCchPrintf(szPipeName, _countof(szPipeName), TEXT("%s%s"), PIPE_NAME_PREFIX, lpName);

        if (INVALID_HANDLE_VALUE != m_hNamedPipe)
        {
            return true;
        }

        ULONGLONG ullCurTick = ::GetTickCount64();

        do
        {
            m_hNamedPipe = ::CreateFile(
                szPipeName,
                GENERIC_READ | GENERIC_WRITE,
                0,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                NULL
            );

            //管道句柄有效则终止循环
            if (INVALID_HANDLE_VALUE != m_hNamedPipe)
            {
                isSuccess = true;
                break;
            }

            //若错误原因不是因为所有管道范例都在使用中, 则退出循环
            if (ERROR_PIPE_BUSY != ::GetLastError())
            {
                break;
            }

            //等待命名管道的实例可用于连接
            if (::WaitNamedPipe(szPipeName, 1000))
            {
                continue;
            }

            //无限等待则不需要检查超时
            if (INFINITE == nTimeOut)
            {
                continue;
            }

            //执行操作超时则退出循环
            if (::GetTickCount64() - ullCurTick > nTimeOut)
            {
                break;
            }

        } while (INVALID_HANDLE_VALUE == m_hNamedPipe);

        if (!isSuccess)
        {
            this->Close();
        }

        return isSuccess;
    }

    bool CNamedPipe::WaitConnect(DWORD nTimeOut)
    {
        OVERLAPPED Overlapped = { 0 };
        bool isConnected = false;

        if (INVALID_HANDLE_VALUE == m_hNamedPipe)
        {
            return false;
        }

        Overlapped.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
        if (NULL == Overlapped.hEvent)
        {
            return false;
        }

        isConnected = ::ConnectNamedPipe(m_hNamedPipe, &Overlapped);

        if (!isConnected)
        {
            DWORD dwError = ::GetLastError();

            //管道关闭中
            if (ERROR_NO_DATA == dwError)
            {
                isConnected = false;
            }

            //操作处于挂起状态
            if (ERROR_IO_PENDING == dwError)
            {
                if (WAIT_OBJECT_0 == ::WaitForSingleObject(Overlapped.hEvent, nTimeOut))
                {
                    isConnected = true;
                }
            }

            //管道已经连接
            if (ERROR_PIPE_CONNECTED == dwError)
            {
                isConnected = true;
            }
        }

        if (NULL != Overlapped.hEvent)
        {
            ::CloseHandle(Overlapped.hEvent);
        }

        m_bConnected = isConnected;

        return isConnected;
    }

    bool CNamedPipe::Disconnect()
    {
        if (INVALID_HANDLE_VALUE == m_hNamedPipe)
        {
            return false;
        }

        //参数句柄必须由 CreateNamedPipe 函数创建
        return ::DisconnectNamedPipe(m_hNamedPipe);
    }

    void CNamedPipe::Close()
    {
        if (INVALID_HANDLE_VALUE != m_hNamedPipe)
        {
            if (m_bConnected)
            {
                ::FlushFileBuffers(m_hNamedPipe);
                ::DisconnectNamedPipe(m_hNamedPipe);
                m_bConnected = false;
            }
            ::CloseHandle(m_hNamedPipe);
            m_hNamedPipe = INVALID_HANDLE_VALUE;
        }
    }

    bool CNamedPipe::IsValid()
    {
        return INVALID_HANDLE_VALUE != m_hNamedPipe;
    }

    bool CNamedPipe::Read(LPVOID lpData, DWORD nSize, LPDWORD lpBytesRead/* = nullptr*/, DWORD nTimeOut)
    {
        OVERLAPPED Overlapped = { 0 };
        Overlapped.hEvent = m_hReadEvent;
        DWORD dwBytesTransferred = 0;
        bool isSuccess = false;

        if (nullptr == m_pBuffer ||
            nullptr == lpData ||
            0 == nSize ||
            nSize > PIPE_BUF_MAX_SIZE
            )
        {
            return false;
        }

        PPIPE_DATA pData = (PPIPE_DATA)m_pBuffer;
        if (!::ReadFile(m_hNamedPipe, &pData->dwSize, sizeof(PIPE_DATA), NULL, &Overlapped))
        {
            //管道已结束
            if (ERROR_BROKEN_PIPE == ::GetLastError())
            {
                return false;
            }

            if (ERROR_IO_PENDING != ::GetLastError())
            {
                return false;
            }

            if (WAIT_OBJECT_0 != ::WaitForSingleObject(Overlapped.hEvent, nTimeOut))
            {
                return false;
            }
        }

        if (pData->dwSize > PIPE_BUF_MAX_SIZE)
        {
            return false;
        }

        if (!::ReadFile(m_hNamedPipe, pData->data, pData->dwSize, NULL, &Overlapped))
        {
            if (ERROR_IO_PENDING != ::GetLastError())
            {
                return false;
            }

            if (WAIT_OBJECT_0 != ::WaitForSingleObject(Overlapped.hEvent, nTimeOut))
            {
                return false;
            }
        }

        if (::GetOverlappedResult(m_hNamedPipe, &Overlapped, &dwBytesTransferred, true))
        {
            isSuccess = true;
            if (lpBytesRead)
            {
                *lpBytesRead = dwBytesTransferred;
            }
        }

        if (isSuccess)
        {
            if (nSize < pData->dwSize)
            {
                ::memcpy_s(lpData, nSize, pData->data, nSize);
            }
            else
            {
                ::memcpy_s(lpData, nSize, pData->data, pData->dwSize);
            }
        }

        return isSuccess;
    }

    bool CNamedPipe::Write(LPCVOID lpData, DWORD nSize, LPDWORD lpBytesWritten/* = nullptr*/, DWORD nTimeOut)
    {
        OVERLAPPED Overlapped = { 0 };
        Overlapped.hEvent = m_hWriteEvent;
        DWORD dwBytesTransferred = 0;
        bool isSuccess = false;

        if (nullptr == m_pBuffer ||
            nullptr == lpData ||
            0 == nSize ||
            nSize > PIPE_BUF_MAX_SIZE
            )
        {
            return false;
        }

        PPIPE_DATA pData = (PPIPE_DATA)m_pBuffer;
        DWORD dwBytesToWrite = nSize + sizeof(PIPE_DATA);
        pData->dwSize = nSize;
        ::memcpy_s(pData->data, PIPE_BUF_MAX_SIZE, lpData, nSize);

        if (::WriteFile(m_hNamedPipe, pData, dwBytesToWrite, NULL, &Overlapped))
        {
            return true;
        }

        //管道正在被关闭
        if (ERROR_NO_DATA == ::GetLastError())
        {
            return false;
        }

        //管道已结束
        if (ERROR_BROKEN_PIPE == ::GetLastError())
        {
            return false;
        }

        //重叠
        if (ERROR_IO_PENDING != ::GetLastError())
        {
            return false;
        }

        if (WAIT_OBJECT_0 != ::WaitForSingleObject(Overlapped.hEvent, nTimeOut))
        {
            return false;
        }

        if (::GetOverlappedResult(m_hNamedPipe, &Overlapped, &dwBytesTransferred, true))
        {
            isSuccess = true;
            if (lpBytesWritten)
            {
                *lpBytesWritten = dwBytesTransferred;
            }
        }

        return isSuccess;
    }

    bool CNamedPipe::Initialize()
    {
        bool isSuccess = false;

        if (m_bInit)
        {
            return true;
        }

        do
        {
            if (nullptr == m_pBuffer)
            {
                m_pBuffer = ::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, PIPE_BUF_MAX_SIZE + sizeof(PIPE_DATA));
            }

            if (nullptr == m_pBuffer)
            {
                break;
            }

            if (NULL == m_hReadEvent)
            {
                m_hReadEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
            }

            if (NULL == m_hReadEvent)
            {
                break;
            }

            if (NULL == m_hWriteEvent)
            {
                m_hWriteEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
            }

            if (NULL == m_hWriteEvent)
            {
                break;
            }

            isSuccess = true;

        } while (false);

        if (!isSuccess)
        {
            Uninitialize();
        }

        m_bInit = isSuccess;

        return m_bInit;
    }

    void CNamedPipe::Uninitialize()
    {
        if (!m_bInit)
        {
            return;
        }

        //关闭管道
        this->Close();

        //释放读写缓冲
        if (nullptr != m_pBuffer)
        {
            ::HeapFree(::GetProcessHeap(), 0, m_pBuffer);
            m_pBuffer = nullptr;
        }

        //关闭事件
        if (m_hReadEvent)
        {
            CloseHandle(m_hReadEvent);
            m_hReadEvent = NULL;
        }

        if (m_hWriteEvent)
        {
            CloseHandle(m_hWriteEvent);
            m_hWriteEvent = NULL;
        }

        m_bInit = false;
    }

    std::map<int64_t, _tstring> CLogHelper::_GetLogFileList(const _tstring& strDir)
    {
        std::map<int64_t, _tstring> fileList;

        WIN32_FIND_DATA findData = { 0 };
        HANDLE hFindHandle = INVALID_HANDLE_VALUE;
        hFindHandle = FindFirstFile((strDir + _T("\\*.*")).c_str(), &findData);
        if (INVALID_HANDLE_VALUE == hFindHandle)
        {
            return fileList;
        }

        do
        {
            _tstring strName = findData.cFileName;

            //非目录
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                //检查输入规则
                int nConverted = 0;
                SYSTEMTIME st = { 0 };

                _tstring strPath = strDir + _T("\\") + strName;
                _tstring strPrefix = Format(_T("%s_%%4hd-%%2hd-%%2hd_%%2hd-%%2hd-%%2hd-%%3hd.log"), m_strSaveName.c_str());
                nConverted = _stscanf_s(findData.cFileName, strPrefix.c_str(),
                    &st.wYear, &st.wMonth, &st.wDay, &st.wHour,
                    &st.wMinute, &st.wSecond, &st.wMilliseconds);

                //检查文件名规则是否符合要求
                if (7 == nConverted)
                {
                    FILETIME ftFile = { 0 };
                    FILETIME ftLocal = { 0 };
                    int64_t timestamp = 0;

                    ::SystemTimeToFileTime(&st, &ftLocal);
                    ::LocalFileTimeToFileTime(&ftLocal, &ftFile);

                    timestamp = ((int64_t)ftFile.dwHighDateTime << 32) | ftFile.dwLowDateTime;
                    timestamp = (timestamp - 116444736000000000) / 10000;

                    fileList.insert(std::make_pair(timestamp, strPath));
                }
            }

            //上一级目录与当前目录跳过
            if (0 == _tcscmp(findData.cFileName, _T(".")) || 0 == _tcscmp(findData.cFileName, _T("..")))
            {
                continue;
            }

        } while (::FindNextFile(hFindHandle, &findData));

        ::FindClose(hFindHandle);

        return fileList;
    }

    _tstring CLogUtils::Format(LPCTSTR pstrFormat, ...)
    {
        _tstring strResult;
        LPTSTR lpBuf = nullptr;
        DWORD dwCchCount = MAX_PATH;

        if (nullptr == pstrFormat)
        {
            return strResult;
        }

        va_list args;
        va_start(args, pstrFormat);
        do
        {
            //分配缓冲
            lpBuf = (LPTSTR)::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, dwCchCount * sizeof(TCHAR));
            if (nullptr == lpBuf)
            {
                break;
            }

            //成功则赋值字符串并终止循环
            if (-1 != _vsntprintf_s(lpBuf, dwCchCount, _TRUNCATE, pstrFormat, args))
            {
                strResult = lpBuf;
                break;
            }

            //释放缓冲, 待下次重新分配
            ::HeapFree(::GetProcessHeap(), 0, lpBuf);
            lpBuf = nullptr;

            //缓冲字符数在合理范围内则扩大2倍
            if (dwCchCount < INT32_MAX)
            {
                dwCchCount *= 2;
            }
            else
            {
                //超限终止处理
                break;
            }

        } while (true);
        va_end(args);

        //释放缓冲
        if (nullptr != lpBuf)
        {
            ::HeapFree(::GetProcessHeap(), 0, lpBuf);
            lpBuf = nullptr;
        }

        return strResult;
    }

    _tstring CLogUtils::GetCurrentTimeString()
    {
        TCHAR szBuf[MAX_PATH] = { 0 };
        SYSTEMTIME st = { 0 };
        (void)::GetLocalTime(&st);

        ::StringCchPrintf(
            szBuf,
            _countof(szBuf),
            _T("%04d-%02d-%02d %02d:%02d:%02d.%d"),
            st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds
        );

        return szBuf;
    }

    int64_t CLogUtils::GetCurrentTimestamp()
    {
        int64_t timeStamp = 0;
        (void)::GetSystemTimeAsFileTime((FILETIME*)&timeStamp);
        return (timeStamp - 116444736000000000) / 10000;
    }

    _tstring CLogUtils::TimestampToString(int64_t timestamp, const _tstring& strFormat)
    {
        TCHAR szBuf[MAX_PATH] = { 0 };
        SYSTEMTIME st = { 0 };
        FILETIME ftFile = { 0 };
        FILETIME ftLocal = { 0 };

        timestamp = timestamp * 10000 + 116444736000000000;
        ftFile.dwLowDateTime = timestamp & 0xFFFFFFFF;
        ftFile.dwHighDateTime = timestamp >> 32;

        ::FileTimeToLocalFileTime(&ftFile, &ftLocal);
        ::FileTimeToSystemTime(&ftLocal, &st);

        ::StringCchPrintf(
            szBuf,
            _countof(szBuf),
            strFormat.c_str(),
            st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds
        );

        return szBuf;
    }

    _tstring CLogUtils::GetCurrentModulePath()
    {
        TCHAR szCurPath[MAX_PATH] = { 0 };
        ::GetModuleFileName(NULL, szCurPath, _countof(szCurPath));

        _tstring strResult = szCurPath;
        return strResult;
    }

    _tstring CLogUtils::GetCurrentModuleDir()
    {
        return GetFileDir(GetCurrentModulePath());
    }

    _tstring CLogUtils::GetCurrentModuleName(bool bHasExt/* = true*/)
    {
        return GetFileName(GetCurrentModulePath(), bHasExt);
    }

    _tstring CLogUtils::GetFileDir(const _tstring& strPath)
    {
        _tstring strResult;
        size_t nIndex = strPath.find_last_of(_T('\\'));
        if (nIndex != _tstring::npos)
        {
            strResult = strPath.substr(0, nIndex);
        }

        return strResult;
    }

    _tstring CLogUtils::GetFileName(const _tstring& strPath, bool bHasExt/* = true*/)
    {
        _tstring strResult = strPath;
        size_t nIndex = strResult.find_last_of(_T('\\'));
        if (nIndex != _tstring::npos)
        {
            strResult = strResult.substr(nIndex + 1);
        }

        if (!bHasExt)
        {
            nIndex = strResult.find_last_of(_T('.'));
            if (nIndex != _tstring::npos)
            {
                return strResult.substr(0, nIndex);
            }
        }

        return strResult;
    }

    bool CLogUtils::IsDirectory(const _tstring& strPath)
    {
        WIN32_FILE_ATTRIBUTE_DATA attr = { 0 };
        if (!::GetFileAttributesEx(strPath.c_str(), GetFileExInfoStandard, &attr))
        {
            return false;
        }

        return attr.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
    }

    bool CLogUtils::CreateDir(const _tstring& strPath)
    {
        _tstring strDriver;              //驱动器号, 如 D:
        _tstring strSubPath = strPath;   //路径, 如 Test\1\2\3

        if (strPath.empty())
        {
            return false;
        }

        //获取盘符
        do
        {
            size_t nFindIndex = strPath.find_first_of(':');  //检查是否有驱动器号
            if (nFindIndex == _tstring::npos)
            {
                break;
            }

            strDriver = strPath.substr(0, nFindIndex + 1); //得到驱动器号, 如 D:
            nFindIndex = strPath.find(_T("\\"), nFindIndex);
            if (nFindIndex == _tstring::npos)
            {
                break;
            }

            strSubPath = strPath.substr(nFindIndex + 1); //得到路径, 如 Test\1\2\3
        } while (false);

        _tstring strDestDir;
        size_t nFindBegin = 0;
        size_t nFindIndex = 0;
        do
        {
            nFindIndex = strSubPath.find(_T("\\"), nFindBegin);
            if (nFindIndex != _tstring::npos)
            {
                strDestDir = strSubPath.substr(0, nFindIndex);
                nFindBegin = nFindIndex + 1;
            }
            else
            {
                strDestDir = strSubPath;
            }

            if (!strDriver.empty())
            {
                strDestDir = strDriver + _T("\\") + strDestDir;
            }

            if (!::CreateDirectory(strDestDir.c_str(), NULL) && ERROR_ALREADY_EXISTS != ::GetLastError())
            {
                return false;
            }

        } while (nFindIndex != _tstring::npos);

        return true;
    }

    bool CLogUtils::DeleteArchive(const _tstring& strPath)
    {
        if (strPath.empty())
        {
            return false;
        }

        return ::DeleteFile(strPath.c_str());
    }

    unsigned long long CLogUtils::GetFileSize(const _tstring& strPath)
    {
        unsigned long long ullSize = 0;
        WIN32_FILE_ATTRIBUTE_DATA attr = { 0 };
        if (!::GetFileAttributesEx(strPath.c_str(), GetFileExInfoStandard, &attr))
        {
            return 0;
        }

        ullSize = (unsigned long long)attr.nFileSizeHigh << 32 | attr.nFileSizeLow;

        return ullSize;
    }
}
