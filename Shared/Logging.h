#ifndef LOGGING_H
#define LOGGING_H

class ILogging
{
public:
    virtual void ShowMsg(const char *msg) = 0;
    virtual void ShowCriticalError(const char *err) = 0;
};

extern ILogging *loggingObj;

#endif // LOGGING_H
