#ifndef COMMON_H
#define COMMON_H

#include <irrlicht.h>
#include <string>
#include <sstream>
#include <iostream>
#include <ctime>
#ifdef __linux__
    #include <sys/time.h>
#elif _WIN32
    #include <windows.h>
#endif

class Common
{
public:
    Common(){}
    inline static irr::core::stringc modelsPath() {return irr::core::stringc("../../media/models/");}
};



namespace Log {
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
        std::string dateFormat = "%d.%m.%y %H:%M:%S";
        strftime(timeString, 24, dateFormat.c_str(), time_info);
        std::stringstream ss;
        std::string zeroString;
        if(millisec < 100) zeroString = "0";
        if(millisec < 10) zeroString = "00";
        ss << '[' << timeString << '.' << zeroString << millisec << "] ";
        return ss.str();
    }

    template<typename T>
    void log(T string)
         {std::cout << curTimeC() << string << std::flush << std::endl;}

    template<typename T>
    void logVector(T vector)
        {std::cout << '(' << vector.X << ',' << vector.Y << ',' << vector.Z << ')' << std::flush << std::endl;}

}


#endif // COMMON_H
