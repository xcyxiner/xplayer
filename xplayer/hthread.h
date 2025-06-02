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
                setStatus(RUNNING);
                run();
                setStatus(STOP);
            });
        }
        return 0;
    }

    void setStatus(Status stat){
        status=stat;
    }

    virtual void run(){
        // while(status!= STOP){
            doTask();
        // }
    }

    virtual bool doPrepare(){
        return true;
    }
    virtual void doTask(){};

    virtual int stop() {
        if (status != STOP) {
            setStatus(STOP);
        }
        if (thread.joinable()) {
            thread.join();  // wait thread exit
        }
        return 0;
    }

    virtual int pause() {
        if (status == RUNNING) {
            setStatus(PAUSE);
        }
        return 0;
    }

    virtual int resume() {
        if (status == PAUSE) {
            setStatus(RUNNING);
        }
        return 0;
    }


    std::thread thread;
    std::atomic<Status> status;
};

#endif // HTHREAD_H
