#ifndef LOGGER_H
#define LOGGER_H

#include <QtWidgets/QApplication>
#include <QString>
#include <QtSql/QSqlError>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <irrlicht.h>
#ifdef __linux__
    #include <sys/time.h>
#elif _WIN32
    #include <windows.h>
    #include <ctime>
#endif

/**
 * @brief Класс TLogger
 * @details Предназначен для логгирования информации. Обеспечивает выходной
 * поток для отладки и должен использоваться всякий раз, когда необходимо
 * отслеживаемую информацию записать в файл, БД, или вывести в консоль. Класс
 * QDebug обеспечивает выходной поток для отладки информации. Имеет 5 уровней
 * логгирования, включающие в себя: Debug, Trace, Info, Warning и Error.
 * Каждый уровень логгирования обладает собственным цветом вывода в консоль.
 * Структура лога имеет следующий вид:
 * [dd.MM.yy HH:mm:ss.zzz] Текст сообщения
 *
 */
class TDebug;

class TLogger {
public:
    static TLogger& get() {
        static TLogger instance;
        return instance;
    }

    void init()
    {
//        m_dateFormat = "%d.%m.%y %H:%M:%S";
        m_dateFormat = "%H:%M:%S";
        m_colors.debug = "36";
        m_colors.trace = "35";
        m_colors.info = "32";
        m_colors.warning = "33";
        m_colors.error = "31";
    }

    // Forbid copying and assigning
    TLogger(const TLogger& s) = delete;
    void operator=(const TLogger& s) = delete;

    friend class TDebug;
private:
    TLogger(){}
    ~TLogger(){}
    std::string m_dateFormat;
    struct
    {
        std::string debug;
        std::string trace;
        std::string info;
        std::string warning;
        std::string error;
    }m_colors;
    std::ofstream f;
};


class TDebug
{
public:
    // Конструкторы
    inline TDebug()
    {
        m_space = true;
        m_dateFormat="[dd.MM.yy HH:mm:ss.zzz] ";
    }
    ~TDebug()
    {
        std::string curTimeString = curTimeC();
        std::string str = m_stream.str();
        size_t start_pos = 0;
        std::string from = "\n";
        std::string to = "";
        for(uint i=0;i<curTimeString.size();i++){to += ' ';}
        while((start_pos = str.find(from, start_pos)) != std::string::npos)
        {
            str.insert(start_pos+1, to);
            str.replace(start_pos+1, to.length(), to);
            start_pos += to.length();
        }
//        std::cout << "\033[" << m_color << "m"<< curTimeString << str << "\033[0m" << std::endl;
#ifdef __linux__
        std::cout << "\033[" << m_color << "m"<< curTimeString << str << "\033[0m" << std::endl;
#elif _WIN32
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handle,m_color);
        std::cout << curTimeString << str << std::endl;
        SetConsoleTextAttribute(handle,7);
#endif
    }

    // Методы
    inline std::string curTimeC()
    {
        int millisec;
#ifdef __linux__
        struct timeval tv;
        gettimeofday(&tv, NULL);
        millisec = lrint(tv.tv_usec/1000.0); // Round to nearest millisec
#elif _WIN32
        SYSTEMTIME st;
        GetSystemTime(&st);
        millisec = st.wMilliseconds;
#endif
        time_t current_time;
        struct tm * time_info;
        char timeString[24];
        time(&current_time);
        time_info = localtime(&current_time);
        std::string dateFormat = TLogger::get().m_dateFormat;
        strftime(timeString, 24, dateFormat.c_str(), time_info);
        std::stringstream ss;
        std::string zeroString;
        if(millisec < 100) zeroString = "0";
        if(millisec < 10) zeroString = "00";
        ss << '[' << timeString << '.' << zeroString << millisec << "] ";
        return ss.str();
    }

#ifdef __linux__
    inline TDebug& debug()   {m_color = TLogger::get().m_colors.debug; return *this;}
    inline TDebug& trace()   {m_color = TLogger::get().m_colors.trace; return *this;}
    inline TDebug& info()    {m_color = TLogger::get().m_colors.info; return *this;}
    inline TDebug& warning() {m_color = TLogger::get().m_colors.warning; return *this;}
    inline TDebug& error()   {m_color = TLogger::get().m_colors.error; return *this;}
#elif _WIN32
    inline TDebug& debug()   {m_color = 11; return *this;}
    inline TDebug& trace()   {m_color = 13; return *this;}
    inline TDebug& info()    {m_color = 10; return *this;}
    inline TDebug& warning() {m_color = 14; return *this;}
    inline TDebug& error()   {m_color = 12; return *this;}
#endif

    inline TDebug &space() { m_space = true; m_stream << ' '; return *this; }
    inline TDebug &nospace() { m_space = false; return *this; }
    inline TDebug &maybeSpace() { if (m_space) m_stream << ' '; return *this; }

    // Операторы перегрузки вывода
    TDebug& operator<<(const std::string& str) {m_stream << str; return maybeSpace();}
    TDebug& operator<<(const char* str) {m_stream << str; return maybeSpace();}
    TDebug& operator<<(char str) {m_stream << str; return maybeSpace();}
    TDebug& operator<<(int str) {m_stream << str;return maybeSpace();}
    TDebug& operator<<(float str) {m_stream << str;return maybeSpace();}
    template<typename T>
    TDebug& operator<<(T str) {m_stream << str;return maybeSpace();}
    TDebug& operator<<(const irr::core::vector3df& str) {m_stream << '(' <<
                                                                     str.X << ',' <<
                                                                     str.Y << ',' <<
                                                                     str.Z << ')';return maybeSpace();}

private:
    std::ostringstream m_stream;
#ifdef __linux__
    std::string m_color;
#elif _WIN32
    DWORD m_color;
#endif
    bool m_space;
    QString m_dateFormat;
};

#define TDEBUG TDebug().debug
#define TTRACE  TDebug().trace
#define TINFO  TDebug().info
#define TWARNING  TDebug().warning
#define TERROR  TDebug().error

#endif // LOGGER_H
