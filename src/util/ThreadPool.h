#ifndef __UTILS_THREADPOOL__
#define __UTILS_THREADPOOL__

#include <define.h>

#include <mutex>
#include <queue>
#include <future>
#include <functional>
#include <condition_variable>

#include <util/List.h>

class ThreadPool final : public Object {
private:
    List<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    
    std::mutex queueLock;
    std::condition_variable taskWait;
    bool stop = false;

public:
    ThreadPool(size_t size){
        for (size_t i = 0; i < size; i++){
            workers.Add(std::thread(
                [this]{
                    for (;;){
                        std::function<void()> task;

                        if (this->stop)
                            return;
                        
                        this->queueLock.lock();
                        this->taskWait.wait(this->queueLock,
                            [this]{ return this->stop || !this->tasks.empty(); });
                        if (this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                        this->queueLock.unlock();

                        task();
                    }
                }
            ));
        }
    }

    template<class F, class... Args>
    auto Enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
                std::bind(std::forward<F>(f), std::forward<Args>(args)...)
            );
            
        std::future<return_type> res = task->get_future();

        queueLock.lock();
        tasks.emplace([task](){ (*task)(); });
        queueLock.unlock();

        taskWait.notify_one();

        return res;
    }
    
    ~ThreadPool(){
        queueLock.lock();
        stop = true;
        queueLock.unlock();

        taskWait.notify_all();

        for (size_t i = 0; i < workers.Size(); i++)
            workers[i].join();
    }
};

#endif