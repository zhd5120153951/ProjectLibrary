#include "CLogUtils.h"
#include <strsafe.h>
#include <tchar.h>
#include <future>

namespace CLogUtils
{
#define DEFAULT_LOG_FILE_COUNT_MIN                  (4)                     //Ĭ��������־�ļ���ʷ����
#define DEFAULT_LOG_TIMEOUT_MIN                     (1000 * 5)              //Ĭ���Զ����泬ʱʱ��(����)
#define DEFAULT_LOG_FILE_SIZE_MIN                   (1024 * 1024 * 1)       //Ĭ�ϵ�����־�ļ���С��ֵ(�ֽ�)
#define DEFAULT_LOG_BUF_COUNT_MIN                   (256)                   //Ĭ����־����������ֵ

#define DEFAULT_LOG_FILE_COUNT_MAX                  (256)                   //Ĭ�������־�ļ���ʷ����
#define DEFAULT_LOG_TIMEOUT_MAX                     (1000 * 60)             //Ĭ���Զ����泬ʱʱ��(����)
#define DEFAULT_LOG_FILE_SIZE_MAX                   (1024 * 1024 * 256)     //Ĭ�ϵ�����־�ļ���С��ֵ(�ֽ�)
#define DEFAULT_LOG_BUF_COUNT_MAX                   (1024 * 4)              //Ĭ����־����������ֵ

#define PIPE_NAME_PREFIX                            TEXT(R"(\\.\pipe\)")    //�ܵ�ǰ׺��
#define PIPE_MAX_TIMEOUT                            (3000)                  //�ܵ��򿪳�ʱ
#define PIPE_BUF_MAX_SIZE                           (1024 * 1024)           //�ܵ����ͻ����С(�ֽ�)
#define PIPE_MAX_CONNECT                            (1)                     //�ܵ����ʵ������

    typedef struct _PIPE_DATA
    {
        DWORD dwSize = 0;
        BYTE data[0];
    }PIPE_DATA, * PPIPE_DATA;

    //ȫ��ʵ������
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
        //��С��������
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

        //����������
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

        //Ĭ��Ŀ¼Ϊ��ǰ����Ŀ¼
        if (m_strSaveDir.empty())
        {
            m_strSaveDir = GetCurrentModuleDir();
        }

        //Ĭ���ļ���Ϊ��ǰ������
        if (m_strSaveName.empty())
        {
            m_strSaveName = GetCurrentModuleName(true);
        }

        //Ŀ¼�����ھʹ���Ŀ¼
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

            //�ȴ��߳��������
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

            //�ȴ��߳��������
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

        //д����־���ݵ��ܵ�, ������һ�˴���(���ÿ����)
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
        //û����Ҫд�����־
        if (m_strLogContent.empty())
        {
            return true;
        }

        bSuccess = ::WriteFile(m_hFile, m_strLogContent.c_str(), (DWORD)(m_strLogContent.size() * sizeof(TCHAR)), &dwNumberOfBytesWrite, NULL);
        m_strLogContent.clear();
#else
        //û����Ҫд�����־
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
        //��ȡ������־���� + �̶�ǰ׺���� + ��ʵ����
        m_nNextItemSize = (DWORD)(strLogContent.size() * sizeof(TCHAR));

        std::lock_guard<std::mutex> lock(m_Lock);

        //�״�����ʱ, ���ô�Сͳ��
        if (!m_bFirst)
        {
            _InitLogFile();
            _AdjustLogFile();
            m_nCurFileSize = (DWORD)GetFileSize(m_strFilePath);
            m_bFirst = true;
        }

        //������־�ļ���С�����ﵽ�򳬹���ֵ��������ļ�, �����µ��ļ��洢
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

        //�ѻ�����Ŀ�ﵽ��ֵ��������ļ�
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

        //�ۼ�ͳ�Ƶ�����־�ļ���С
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

        //��־��ʽǰ׺ [ʱ��] [�ȼ�] [ʮ���ƽ���ID:ʮ�����߳�ID] [Դ��λ��:����] [������]
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
            //���仺��
            lpBuf = (LPTSTR)::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, dwCchCount * sizeof(TCHAR));
            if (nullptr == lpBuf)
            {
                break;
            }

            //�ɹ���ֵ�ַ�������ֹѭ��
            if (-1 != _vsntprintf_s(lpBuf, dwCchCount, _TRUNCATE, pstrFormat, args))
            {
                strLogContent += lpBuf;
                break;
            }

            //�ͷŻ���, ���´����·���
            ::HeapFree(::GetProcessHeap(), 0, lpBuf);
            lpBuf = nullptr;

            //�����ַ����ں���Χ��������2��
            if (dwCchCount < INT32_MAX)
            {
                dwCchCount *= 2;
            }
            else
            {
                //������ֹ����
                break;
            }

        } while (true);
        va_end(args);

        //�ͷŻ���
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
        //����ļ������Ƿ񵽴���ֵ, ����Ļ�ɾ��ǰ����ļ�
        if (m_logFileList.size() > m_nFileCount)
        {
            size_t nDeleteCount = m_logFileList.size() - m_nFileCount;

            //����־�ļ���¼�б���ɾ��
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
        //����ϴ����һ����־�ļ���С���ܴ洢��־, �������ϴε���־�ļ�
        m_logFileList = _GetLogFileList(m_strSaveDir);
        if (!m_logFileList.empty())
        {
            auto itLast = m_logFileList.end();
            itLast--;

            m_nFileTimetamp = itLast->first;
            m_strFilePath = itLast->second;

            //�ϴ����һ����־�ļ����ܴ洢������־, �������µ���־�ļ�·��
            unsigned long long ullFileSize = (DWORD)GetFileSize(m_strFilePath);
            if ((ullFileSize + m_nNextItemSize) >= m_nFileSize)
            {
                (void)_GenerateLogFilePath();
            }
            else
            {
                //���ļ�����д��־
                m_hFile = CreateFile(
                    m_strFilePath.c_str(),
                    GENERIC_WRITE,
                    FILE_SHARE_READ,
                    NULL,
                    OPEN_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL
                );

                //���ļ�ĩβ׷������
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
    // @brief: ��ȡ��־�ܵ���
    // @param: void
    // @ret: _tstring �ܵ���
    _tstring CLogHelper::_GetDefaultPipeName() const
    {
        return Format(_T("%s_%s"), m_strSaveDir.c_str(), m_strSaveName.c_str());
    }

    void CLogHelper::_GenerateLogFilePath()
    {
        //�õ���־�ļ�ʱ���
        m_nFileTimetamp = GetCurrentTimestamp();

        //�õ���־�ļ�·��
        m_strFilePath = Format(_T("%s\\%s_%s.log"),
            m_strSaveDir.c_str(),
            m_strSaveName.c_str(),
            TimestampToString(m_nFileTimetamp, _T("%04d-%02d-%02d_%02d-%02d-%02d-%03d")).c_str()
        );

        //����һ���ļ�(��ֹ����Դ�������п������µ���־�ļ�)
        m_hFile = CreateFile(
            m_strFilePath.c_str(),
            GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

        //���ļ�ĩβ׷������
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
        //��ʼ����д�������¼����
        Initialize();
    }

    CNamedPipe::~CNamedPipe()
    {
        //�ͷŶ�д�������¼����
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

        //����Ȩ��, ��ֹ��Ȩ�޽��̲��ܴ򿪸�Ȩ�޽��̴����Ĺܵ�
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

            //�ܵ������Ч����ֹѭ��
            if (INVALID_HANDLE_VALUE != m_hNamedPipe)
            {
                isSuccess = true;
                break;
            }

            //������ԭ������Ϊ���йܵ���������ʹ����, ���˳�ѭ��
            if (ERROR_PIPE_BUSY != ::GetLastError())
            {
                break;
            }

            //�ȴ������ܵ���ʵ������������
            if (::WaitNamedPipe(szPipeName, 1000))
            {
                continue;
            }

            //���޵ȴ�����Ҫ��鳬ʱ
            if (INFINITE == nTimeOut)
            {
                continue;
            }

            //ִ�в�����ʱ���˳�ѭ��
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

            //�ܵ��ر���
            if (ERROR_NO_DATA == dwError)
            {
                isConnected = false;
            }

            //�������ڹ���״̬
            if (ERROR_IO_PENDING == dwError)
            {
                if (WAIT_OBJECT_0 == ::WaitForSingleObject(Overlapped.hEvent, nTimeOut))
                {
                    isConnected = true;
                }
            }

            //�ܵ��Ѿ�����
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

        //������������� CreateNamedPipe ��������
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
            //�ܵ��ѽ���
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

        //�ܵ����ڱ��ر�
        if (ERROR_NO_DATA == ::GetLastError())
        {
            return false;
        }

        //�ܵ��ѽ���
        if (ERROR_BROKEN_PIPE == ::GetLastError())
        {
            return false;
        }

        //�ص�
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

        //�رչܵ�
        this->Close();

        //�ͷŶ�д����
        if (nullptr != m_pBuffer)
        {
            ::HeapFree(::GetProcessHeap(), 0, m_pBuffer);
            m_pBuffer = nullptr;
        }

        //�ر��¼�
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

            //��Ŀ¼
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                //����������
                int nConverted = 0;
                SYSTEMTIME st = { 0 };

                _tstring strPath = strDir + _T("\\") + strName;
                _tstring strPrefix = Format(_T("%s_%%4hd-%%2hd-%%2hd_%%2hd-%%2hd-%%2hd-%%3hd.log"), m_strSaveName.c_str());
                nConverted = _stscanf_s(findData.cFileName, strPrefix.c_str(),
                    &st.wYear, &st.wMonth, &st.wDay, &st.wHour,
                    &st.wMinute, &st.wSecond, &st.wMilliseconds);

                //����ļ��������Ƿ����Ҫ��
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

            //��һ��Ŀ¼�뵱ǰĿ¼����
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
            //���仺��
            lpBuf = (LPTSTR)::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, dwCchCount * sizeof(TCHAR));
            if (nullptr == lpBuf)
            {
                break;
            }

            //�ɹ���ֵ�ַ�������ֹѭ��
            if (-1 != _vsntprintf_s(lpBuf, dwCchCount, _TRUNCATE, pstrFormat, args))
            {
                strResult = lpBuf;
                break;
            }

            //�ͷŻ���, ���´����·���
            ::HeapFree(::GetProcessHeap(), 0, lpBuf);
            lpBuf = nullptr;

            //�����ַ����ں���Χ��������2��
            if (dwCchCount < INT32_MAX)
            {
                dwCchCount *= 2;
            }
            else
            {
                //������ֹ����
                break;
            }

        } while (true);
        va_end(args);

        //�ͷŻ���
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
        _tstring strDriver;              //��������, �� D:
        _tstring strSubPath = strPath;   //·��, �� Test\1\2\3

        if (strPath.empty())
        {
            return false;
        }

        //��ȡ�̷�
        do
        {
            size_t nFindIndex = strPath.find_first_of(':');  //����Ƿ�����������
            if (nFindIndex == _tstring::npos)
            {
                break;
            }

            strDriver = strPath.substr(0, nFindIndex + 1); //�õ���������, �� D:
            nFindIndex = strPath.find(_T("\\"), nFindIndex);
            if (nFindIndex == _tstring::npos)
            {
                break;
            }

            strSubPath = strPath.substr(nFindIndex + 1); //�õ�·��, �� Test\1\2\3
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
