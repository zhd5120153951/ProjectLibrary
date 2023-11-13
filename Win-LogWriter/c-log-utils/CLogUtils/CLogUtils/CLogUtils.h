#pragma once

#include <string>
#include <windows.h>
#include <vector>
#include <map>
#include <mutex>
#include <tchar.h>
#include <thread>

#ifdef _UNICODE
using _tstring = std::wstring;
#else
using _tstring = std::string;
#endif

#pragma warning(disable:4200)

namespace CLogUtils
{
    class CNamedPipe;

#define USR_STRING_BUFFER_CONTENT       (1)                 //ʹ���ַ�����Ϊ��־����

#define LOG_FILE_COUNT                  (16)                //�����־�ļ���ʷ����
#define LOG_TIMEOUT                     (1000 * 60)         //�Զ����泬ʱʱ��(����)
#define LOG_FILE_SIZE                   (1024 * 1024 * 16)  //������־�ļ���С��ֵ(�ֽ�)
#define LOG_BUF_COUNT                   (10000)             //��־�����С��ֵ

#define LOG_INFO(format, ...)\
    global_logger.Logging(_T(" INFO"), _T(__FILE__), _T(__FUNCTION__), __LINE__, format, ##__VA_ARGS__);

#ifdef _DEBUG
#define LOG_DEBUG(format, ...)\
    global_logger.Logging(_T("DEBUG"), _T(__FILE__), _T(__FUNCTION__), __LINE__, format, ##__VA_ARGS__);
#else
#define LOG_DEBUG(format, ...)\
    global_logger.DoNothing();
#endif

#define LOG_WARN(format, ...)\
    global_logger.Logging(_T(" WARN"), _T(__FILE__), _T(__FUNCTION__), __LINE__, format, ##__VA_ARGS__);

#define LOG_ERROR(format, ...)\
    global_logger.Logging(_T("ERROR"), _T(__FILE__), _T(__FUNCTION__), __LINE__, format, ##__VA_ARGS__);

    class CLogHelper
    {

    public:

#define Info(format, ...)\
    Logging(_T(" INFO"), _T(__FILE__), _T(__FUNCTION__), __LINE__, format, ##__VA_ARGS__);

#ifdef _DEBUG
#define Debug(format, ...)\
    Logging(_T("DEBUG"), _T(__FILE__), _T(__FUNCTION__), __LINE__, format, ##__VA_ARGS__);
#else
#define Debug(format, ...)\
    DoNothing();
#endif

#define Warn(format, ...)\
    Logging(_T(" WARN"), _T(__FILE__), _T(__FUNCTION__), __LINE__, format, ##__VA_ARGS__);

#define Error(format, ...)\
    Logging(_T("ERROR"), _T(__FILE__), _T(__FUNCTION__), __LINE__, format, ##__VA_ARGS__);

    public:

        //
        // @brief: ����
        // @param: bPipeMode    �ܵ�ģʽ true: �ܵ�ģʽ false: Ĭ��ģʽ
        // @param: strPipeName  �ܵ���
        // @param: strDir       ���Ŀ¼
        // @param: strPreName   �ļ���ǰ׺
        // @param: nFileSize    ������־�ļ���С��ֵ(�ֽ�), ��������ת������һ���ļ�
        // @param: nTempCount   ��־������Ŀ����(��������ת�����ļ�)
        // @param: nTimeout     �Զ��洢ʱ����(����), ��������ת�����ļ�
        // @param: nFileCount   ��־�ļ���������, ��������ɾ����ɵ���־�ļ�
        // @ret: void
        CLogHelper(
            bool bPipeMode = false,
            const _tstring& strPipeName = _T(""),
            const _tstring& strDir = _T(""),
            const _tstring& strPreName = _T(""),
            DWORD nFileSize = LOG_FILE_SIZE,
            DWORD nTempCount = LOG_BUF_COUNT,
            DWORD nTimeout = LOG_TIMEOUT,
            DWORD nFileCount = LOG_FILE_COUNT
        );

        //ɾ�����������븳ֵ����
        CLogHelper(const CLogHelper&) = delete;
        CLogHelper& operator = (const CLogHelper&) = delete;

        ~CLogHelper();

        //
        // @brief: ��¼һ����־
        // @param: pstrLevel    ��־�ȼ�
        // @param: pstrFile     Դ���ļ�
        // @param: pstrFunc     Դ�뺯��
        // @param: nLine        ����
        // @param: pstrFormat   ��ʽ���ַ���
        // @param: ...          �ɱ����
        // @ret: void
        void Logging(
            LPCTSTR pstrLevel,
            LPCTSTR pstrFile,
            LPCTSTR pstrFunc,
            UINT nLine,
            LPCTSTR pstrFormat,
            ...
        );

        //
        // @brief: ����Ѿ��洢����־�ļ�
        // @ret: void
        void Clear();

        //
        // @brief: ˢ����־����(�����־���ļ�)
        // @ret: bool ִ�н��
        bool FlushBuffers();

        //
        // @brief: ɶҲ����
        // @ret: void
        void DoNothing();

    private:

        //
        // @brief: ��ȡĿ¼���ļ�·��
        // @ret: std::vector<_tstring> ��־�ļ��б�
        std::map<int64_t, _tstring> _GetLogFileList(const _tstring& strDir);

        //
        // @brief: ������־�ļ�����
        // @param: void
        // @ret: void
        void _AdjustLogFile();

        //
        // @brief: ͨ���ܵ�������־
        // @ret: bool ִ�н��
        bool _ProcessByPipe();

        //
        // @brief: ��ʱ���洦��
        // @ret: bool ִ�н��
        bool _ProcessTimeoutSave();

        //
        // @brief: �ܵ���ʽ��¼��־
        // @ret: bool ִ�н��
        bool _LoggingByPipe(const _tstring& strLogContent);

        //
        // @brief: ˢ����־����(�����־���ļ�)
        // @ret: bool ִ�н��
        bool _FlushLogBuffers();

        //
        // @brief: ��¼��־
        // @ret: bool ִ�н��
        bool _LoggingContent(const _tstring& strLogContent);

        //
        // @brief: ��ʼ��
        // @param: void
        // @ret: bool ִ�н��
        bool _Initialize();

        //
        // @brief: ȡ����ʼ��
        // @param: void
        // @ret: void
        void _Uninitialize();

        //
        // @brief: ��ʼ����־�ļ�
        // @param: void
        // @ret: int ��־�ļ�����
        void _InitLogFile();

        //
        // @brief: ������־ת���ļ�·��
        // @param: void
        // @ret: void
        void _GenerateLogFilePath();

        //
        // @brief: ��ȡĬ����־�ܵ���
        // @param: void
        // @ret: _tstring �ܵ���
        _tstring _GetDefaultPipeName() const;

    private:

        std::vector<_tstring> m_logList;            //��־��¼����
        std::map<int64_t, _tstring> m_logFileList;  //��־�ļ���¼, ����ʱ�������
        std::thread m_tAutoSaveTask;                //��ʱ�Զ����������߳�
        std::thread m_tPipeRecvTask;                //�ܵ����������߳�
        std::mutex  m_Lock;                         //�̰߳�ȫ��
        CNamedPipe *m_pRecvPipe = nullptr;          //��־���չܵ�
        CNamedPipe* m_pSendPipe = nullptr;          //��־���͹ܵ�

        HANDLE m_hEvent = nullptr;                  //֪ͨ�¼�, ʹ���Զ�ת���ĳ�ʱ�ȴ�
        HANDLE m_hFile = INVALID_HANDLE_VALUE;      //�ļ����, ��־�ļ�д��ʹ��
        int64_t  m_nFileTimetamp = 0;               //��־�ļ�ʱ���
        _tstring m_strSaveDir;                      //��־���Ŀ¼
        _tstring m_strSaveName;                     //��־�ļ���
        _tstring m_strFilePath;                     //��ǰ��־�ļ�·��
        _tstring m_strLogContent;                   //��־����
        _tstring m_strPipeName;                     //�ܵ���
        bool m_bStop = false;                       //ֹͣ���
        bool m_bFirst = false;                      //�״μ�¼��־���
        bool m_bPipeMode = false;                   //�ܵ�ģʽ

        DWORD m_nFileSize = 0;                      //�ļ���С����(����������ת�����ļ�)
        DWORD m_nTempCount = 0;                     //������Ŀ����(����������ת�����ļ�)
        DWORD m_nFileCount = 0;                     //��ʷ�ļ���������(������ɾ�����ļ�)
        DWORD m_nTimeout = 0;                       //�Զ����泬ʱ����(��ʱ��ת�����ļ�)
        DWORD m_nCurFileSize = 0;                   //��־�ļ�ͳ��
        DWORD m_nNextItemSize = 0;                  //��һ����־��С
        DWORD m_nLogItemCount = 0;                  //��־����ͳ��
    };

    class CNamedPipe
    {
    public:
        CNamedPipe();

        ~CNamedPipe();

        CNamedPipe(const CNamedPipe& r) = delete;
        CNamedPipe& operator = (const CNamedPipe& r) = delete;

        //
        // @brief: ���������ܵ�
        // @param: lpName           �ܵ���
        // @ret: bool               true: �����ɹ� false: ����ʧ��
        bool Create(LPCTSTR lpName);

        //
        // @brief: �ȴ��ͻ������������ܵ�
        // @param: nTimeOut         ��ʱ�ȴ�(����)
        // @ret: bool               true: ���ӳɹ� false: ����ʧ��
        bool WaitConnect(DWORD nTimeOut = INFINITE);

        //
        // @brief: �ر���Create �����Ĺܵ�
        // @param: void
        // @ret: bool               true: �ر� �ɹ� false: �ر� ʧ��
        bool Disconnect();

        //
        // @brief: ���Ѵ��ڵ������ܵ�
        // @param: lpName           �ܵ���
        // @ret: bool               true: �򿪳ɹ� false: ��ʧ��
        bool Open(LPCTSTR lpName, DWORD nTimeOut = INFINITE);

        //
        // @brief: �ܵ��Ƿ���Ч
        // @param: void
        // @ret: bool               true: ���� false: ��Ч
        bool IsValid();

        //
        // @brief: �رչܵ�
        // @param: void
        // @ret: void
        void Close(void);

        //
        // @brief: �Ӷ�ȡ�ܵ�����
        // @param: lpData           ���ݴ�Ż���
        // @param: nSize            �����С(�ֽ�)
        // @param: lpBytesRead      ָ��ʵ�ʶ�ȡ��С(�ֽ�)��ָ��
        // @param: nTimeOut         ��ȡ��ʱ(����)
        // @ret: bool               true: ��ȡ�ɹ� false: ��ȡʧ��
        bool Read(LPVOID lpData, DWORD nSize, LPDWORD lpBytesRead = nullptr, DWORD nTimeOut = INFINITE);

        //
        // @brief: ��ܵ�д������
        // @param: lpData           д������ָ��
        // @param: nSize            д�����ݴ�С(�ֽ�)
        // @param: lpBytesWritten   ָ��ʵ��д���С(�ֽ�)��ָ��
        // @param: nTimeOut         д�볬ʱ(����)
        // @ret: bool               true: д��ɹ� false: д��ʧ��
        bool Write(LPCVOID lpData, DWORD nSize, LPDWORD lpBytesWritten = nullptr, DWORD nTimeOut = INFINITE);

    private:

        //
        // @brief: ��ʼ������ռ��
        // @param: void
        // @ret: void
        bool Initialize();

        //
        // @brief: �ͷŶ���ռ��
        // @param: void
        // @ret: void
        void Uninitialize();

    private:
        HANDLE m_hNamedPipe = INVALID_HANDLE_VALUE;
        HANDLE m_hReadEvent = NULL;
        HANDLE m_hWriteEvent = NULL;
        LPVOID m_pBuffer = nullptr;
        bool m_bInit = false;
        bool m_bConnected = false;
    };

    //
    // @brief: ��ʽ���ַ���
    // @param: void
    // @ret: bool ִ�н��
    _tstring Format(LPCTSTR pstrFormat, ...);

    // 
    // @brief: ��ȡ��ǰ������ȫ·��
    // @ret: ��ǰ������ȫ·�� �� D:\Software\HxDPortableSetup.exe
    _tstring GetCurrentModulePath();

    // 
    // @brief: ��ȡ��ǰ��������Ŀ¼
    // @ret: ��ǰ��������Ŀ¼ �� D:\Software
    _tstring GetCurrentModuleDir();

    // 
    // @brief: ��ȡ��ǰ������
    // @ret: ��ǰ������ �� HxDPortableSetup.exe
    _tstring GetCurrentModuleName(bool bHasExt = false);

    // 
    // @brief: ��ȡ�ļ������ļ���
    // @param: strPath     �ļ���, ��: D:\Software\HxDPortableSetup.exe
    // @ret: �ļ���        �� D:\Software
    _tstring GetFileDir(const _tstring& strPath);

    // 
    // @brief: ��ȡ�ļ���
    // @param: strPath     �ļ���, ��: D:\Software\HxDPortableSetup.exe
    // @param: bHasExt     �Ƿ������չ��
    // @ret: �ļ���        �� HxDPortableSetup
    _tstring GetFileName(const _tstring& strPath, bool bHasExt = false);

    // 
    // @brief: ����ļ��Ƿ����
    // @param: strPath     �ļ���, ��: D:\Software\HxDPortableSetup.exe
    // @ret: �Ƿ����      ���ڷ��� true
    bool IsDirectory(const _tstring& strPath);

    //
    // @brief: ����Ŀ¼(�ݹ�)
    // @param: strPath      ·��
    // @ret: �ɹ�����true
    bool CreateDir(const _tstring& strPath);

    //
    // @brief: ɾ���ļ�
    // @param: strPath      ·��
    // @ret: �ɹ�����true
    bool DeleteArchive(const _tstring& strPath);

    //
    // @brief: ��ȡ��ǰʱ����ַ���
    // @param: void
    // @ret: _tstring ʱ����ַ���          ��: 2023-10-11 17:43:00.617
    _tstring GetCurrentTimeString();

    //
    // @brief: ��ȡ��ǰʱ���
    // @param: void
    // @ret: ʱ���(��λ: ����)    ��: 1697017380617
    int64_t GetCurrentTimestamp();

    //
    // @brief: ʱ���ת�ַ���  
    // @param: strFormat    ��ʽ���ַ��� ��: "%04d-%02d-%02d %02d:%02d:%02d.%d"
    // @param: timestamp    ʱ��� ��: 1697017380617
    // @ret: ʱ���ַ���            ��: 2023-10-11 17:43:00.617
    _tstring TimestampToString(
        int64_t timestamp = 0,
        const _tstring& strFormat = _T("%04d-%02d-%02d %02d:%02d:%02d.%d")
    );

    //
    // @brief: ��ȡ�ļ���С
    // @param: strPath      ·��
    // @ret: �ļ���С
    unsigned long long GetFileSize(const _tstring& strPath);

    extern CLogHelper global_logger;                //ȫ�־�̬ʵ��
}