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

#define USR_STRING_BUFFER_CONTENT       (1)                 //使用字符串作为日志缓冲

#define LOG_FILE_COUNT                  (16)                //最多日志文件历史数量
#define LOG_TIMEOUT                     (1000 * 60)         //自动保存超时时间(毫秒)
#define LOG_FILE_SIZE                   (1024 * 1024 * 16)  //单个日志文件大小阈值(字节)
#define LOG_BUF_COUNT                   (10000)             //日志缓冲大小阈值

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
        // @brief: 构造
        // @param: bPipeMode    管道模式 true: 管道模式 false: 默认模式
        // @param: strPipeName  管道名
        // @param: strDir       存放目录
        // @param: strPreName   文件名前缀
        // @param: nFileSize    单个日志文件大小阈值(字节), 到达限制转储到下一个文件
        // @param: nTempCount   日志缓存条目限制(到达限制转储到文件)
        // @param: nTimeout     自动存储时间间隔(毫秒), 到达限制转储到文件
        // @param: nFileCount   日志文件数量限制, 到达限制删除最旧的日志文件
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

        //删除拷贝构造与赋值重载
        CLogHelper(const CLogHelper&) = delete;
        CLogHelper& operator = (const CLogHelper&) = delete;

        ~CLogHelper();

        //
        // @brief: 记录一条日志
        // @param: pstrLevel    日志等级
        // @param: pstrFile     源码文件
        // @param: pstrFunc     源码函数
        // @param: nLine        行数
        // @param: pstrFormat   格式化字符串
        // @param: ...          可变参数
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
        // @brief: 清空已经存储的日志文件
        // @ret: void
        void Clear();

        //
        // @brief: 刷新日志缓冲(输出日志到文件)
        // @ret: bool 执行结果
        bool FlushBuffers();

        //
        // @brief: 啥也不干
        // @ret: void
        void DoNothing();

    private:

        //
        // @brief: 获取目录下文件路径
        // @ret: std::vector<_tstring> 日志文件列表
        std::map<int64_t, _tstring> _GetLogFileList(const _tstring& strDir);

        //
        // @brief: 调整日志文件数量
        // @param: void
        // @ret: void
        void _AdjustLogFile();

        //
        // @brief: 通过管道处理日志
        // @ret: bool 执行结果
        bool _ProcessByPipe();

        //
        // @brief: 超时保存处理
        // @ret: bool 执行结果
        bool _ProcessTimeoutSave();

        //
        // @brief: 管道方式记录日志
        // @ret: bool 执行结果
        bool _LoggingByPipe(const _tstring& strLogContent);

        //
        // @brief: 刷新日志缓冲(输出日志到文件)
        // @ret: bool 执行结果
        bool _FlushLogBuffers();

        //
        // @brief: 记录日志
        // @ret: bool 执行结果
        bool _LoggingContent(const _tstring& strLogContent);

        //
        // @brief: 初始化
        // @param: void
        // @ret: bool 执行结果
        bool _Initialize();

        //
        // @brief: 取消初始化
        // @param: void
        // @ret: void
        void _Uninitialize();

        //
        // @brief: 初始化日志文件
        // @param: void
        // @ret: int 日志文件索引
        void _InitLogFile();

        //
        // @brief: 生成日志转储文件路径
        // @param: void
        // @ret: void
        void _GenerateLogFilePath();

        //
        // @brief: 获取默认日志管道名
        // @param: void
        // @ret: _tstring 管道名
        _tstring _GetDefaultPipeName() const;

    private:

        std::vector<_tstring> m_logList;            //日志记录缓冲
        std::map<int64_t, _tstring> m_logFileList;  //日志文件记录, 按照时间戳排序
        std::thread m_tAutoSaveTask;                //超时自动保存任务线程
        std::thread m_tPipeRecvTask;                //管道接收任务线程
        std::mutex  m_Lock;                         //线程安全锁
        CNamedPipe *m_pRecvPipe = nullptr;          //日志接收管道
        CNamedPipe* m_pSendPipe = nullptr;          //日志发送管道

        HANDLE m_hEvent = nullptr;                  //通知事件, 使用自动转储的超时等待
        HANDLE m_hFile = INVALID_HANDLE_VALUE;      //文件句柄, 日志文件写入使用
        int64_t  m_nFileTimetamp = 0;               //日志文件时间戳
        _tstring m_strSaveDir;                      //日志存放目录
        _tstring m_strSaveName;                     //日志文件名
        _tstring m_strFilePath;                     //当前日志文件路径
        _tstring m_strLogContent;                   //日志内容
        _tstring m_strPipeName;                     //管道名
        bool m_bStop = false;                       //停止标记
        bool m_bFirst = false;                      //首次记录日志标记
        bool m_bPipeMode = false;                   //管道模式

        DWORD m_nFileSize = 0;                      //文件大小限制(到达限制则转储到文件)
        DWORD m_nTempCount = 0;                     //缓存条目限制(到达限制则转储到文件)
        DWORD m_nFileCount = 0;                     //历史文件数量限制(超限则删除旧文件)
        DWORD m_nTimeout = 0;                       //自动保存超时限制(超时则转储到文件)
        DWORD m_nCurFileSize = 0;                   //日志文件统计
        DWORD m_nNextItemSize = 0;                  //下一条日志大小
        DWORD m_nLogItemCount = 0;                  //日志缓冲统计
    };

    class CNamedPipe
    {
    public:
        CNamedPipe();

        ~CNamedPipe();

        CNamedPipe(const CNamedPipe& r) = delete;
        CNamedPipe& operator = (const CNamedPipe& r) = delete;

        //
        // @brief: 创建命名管道
        // @param: lpName           管道名
        // @ret: bool               true: 创建成功 false: 创建失败
        bool Create(LPCTSTR lpName);

        //
        // @brief: 等待客户端连接命名管道
        // @param: nTimeOut         超时等待(毫秒)
        // @ret: bool               true: 连接成功 false: 连接失败
        bool WaitConnect(DWORD nTimeOut = INFINITE);

        //
        // @brief: 关闭由Create 创建的管道
        // @param: void
        // @ret: bool               true: 关闭 成功 false: 关闭 失败
        bool Disconnect();

        //
        // @brief: 打开已存在的命名管道
        // @param: lpName           管道名
        // @ret: bool               true: 打开成功 false: 打开失败
        bool Open(LPCTSTR lpName, DWORD nTimeOut = INFINITE);

        //
        // @brief: 管道是否有效
        // @param: void
        // @ret: bool               true: 可用 false: 无效
        bool IsValid();

        //
        // @brief: 关闭管道
        // @param: void
        // @ret: void
        void Close(void);

        //
        // @brief: 从读取管道数据
        // @param: lpData           数据存放缓冲
        // @param: nSize            缓冲大小(字节)
        // @param: lpBytesRead      指向实际读取大小(字节)的指针
        // @param: nTimeOut         读取超时(毫秒)
        // @ret: bool               true: 读取成功 false: 读取失败
        bool Read(LPVOID lpData, DWORD nSize, LPDWORD lpBytesRead = nullptr, DWORD nTimeOut = INFINITE);

        //
        // @brief: 向管道写入数据
        // @param: lpData           写入数据指针
        // @param: nSize            写入数据大小(字节)
        // @param: lpBytesWritten   指向实际写入大小(字节)的指针
        // @param: nTimeOut         写入超时(毫秒)
        // @ret: bool               true: 写入成功 false: 写入失败
        bool Write(LPCVOID lpData, DWORD nSize, LPDWORD lpBytesWritten = nullptr, DWORD nTimeOut = INFINITE);

    private:

        //
        // @brief: 初始化对象占用
        // @param: void
        // @ret: void
        bool Initialize();

        //
        // @brief: 释放对象占用
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
    // @brief: 格式化字符串
    // @param: void
    // @ret: bool 执行结果
    _tstring Format(LPCTSTR pstrFormat, ...);

    // 
    // @brief: 获取当前进程完全路径
    // @ret: 当前进程完全路径 如 D:\Software\HxDPortableSetup.exe
    _tstring GetCurrentModulePath();

    // 
    // @brief: 获取当前进程所在目录
    // @ret: 当前进程所在目录 如 D:\Software
    _tstring GetCurrentModuleDir();

    // 
    // @brief: 获取当前进程名
    // @ret: 当前进程名 如 HxDPortableSetup.exe
    _tstring GetCurrentModuleName(bool bHasExt = false);

    // 
    // @brief: 获取文件所在文件夹
    // @param: strPath     文件名, 如: D:\Software\HxDPortableSetup.exe
    // @ret: 文件夹        如 D:\Software
    _tstring GetFileDir(const _tstring& strPath);

    // 
    // @brief: 获取文件名
    // @param: strPath     文件名, 如: D:\Software\HxDPortableSetup.exe
    // @param: bHasExt     是否包含扩展名
    // @ret: 文件夹        如 HxDPortableSetup
    _tstring GetFileName(const _tstring& strPath, bool bHasExt = false);

    // 
    // @brief: 检查文件是否存在
    // @param: strPath     文件名, 如: D:\Software\HxDPortableSetup.exe
    // @ret: 是否存在      存在返回 true
    bool IsDirectory(const _tstring& strPath);

    //
    // @brief: 创建目录(递归)
    // @param: strPath      路径
    // @ret: 成功返回true
    bool CreateDir(const _tstring& strPath);

    //
    // @brief: 删除文件
    // @param: strPath      路径
    // @ret: 成功返回true
    bool DeleteArchive(const _tstring& strPath);

    //
    // @brief: 获取当前时间戳字符串
    // @param: void
    // @ret: _tstring 时间戳字符串          如: 2023-10-11 17:43:00.617
    _tstring GetCurrentTimeString();

    //
    // @brief: 获取当前时间戳
    // @param: void
    // @ret: 时间戳(单位: 毫秒)    如: 1697017380617
    int64_t GetCurrentTimestamp();

    //
    // @brief: 时间戳转字符串  
    // @param: strFormat    格式化字符串 如: "%04d-%02d-%02d %02d:%02d:%02d.%d"
    // @param: timestamp    时间戳 如: 1697017380617
    // @ret: 时间字符串            如: 2023-10-11 17:43:00.617
    _tstring TimestampToString(
        int64_t timestamp = 0,
        const _tstring& strFormat = _T("%04d-%02d-%02d %02d:%02d:%02d.%d")
    );

    //
    // @brief: 获取文件大小
    // @param: strPath      路径
    // @ret: 文件大小
    unsigned long long GetFileSize(const _tstring& strPath);

    extern CLogHelper global_logger;                //全局静态实例
}