#ifndef Log_h
#define Log_h

#include "stdafx.h"
#define CURRENT_LEVEL 0

class Log
{
public:
    enum LogLevel {
        Debug, Info, Success, Warning, Error
    };

    static void LogDebug(std::string&& message){
        PushMessage(Debug, message);
    }

    static void LogInfo(std::string&& message){
        PushMessage(Info, message);
    }
    
    static void LogSuccess(std::string&& message){
        PushMessage(Success, message);
    }
    
    static void LogWarning(std::string&& message){
        PushMessage(Warning, message);
    }
    
    static void LogError(std::string&& message) {
        PushMessage(Error, message);
    }
    
    static void PushMessage(LogLevel level, std::string msg) {
        std::string prefix;
        if (level < CURRENT_LEVEL) return;
        switch (level) {
        case LogLevel::Debug:
            prefix = "[DEBUG]: ";
            break;
        case LogLevel::Info:
            prefix = "[INFO]: ";
            break;
        case LogLevel::Success:
            prefix = "[SUCCESS]: ";
            break;
        case LogLevel::Warning:
            prefix = "[WARNING]: ";
            break;
        case LogLevel::Error:
            prefix = "[ERROR]: ";
            break;
        }
        
        std::string message_;
        message_ = prefix + msg;
        std::cout << message_ << std::endl;
    }
};

#endif //Log_h
