#ifndef HTHREAD_H
#define HTHREAD_H
#include <atomic>
#include <thread>
class HThread
{
public:
    HThread();

public:
    enum Status{
        STOP,
        RUNNING,
        PAUSE
    };

    virtual int start(){
        if(status==STOP){
            thread = std::thread([this]{
                if(!doPrepare())return;

            });
        }
        return 0;
    }

    virtual bool doPrepare(){
        return true;
    }

    std::thread thread;
    std::atomic<Status> status;
};

#endif // HTHREAD_H
