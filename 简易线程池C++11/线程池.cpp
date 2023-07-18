#include <iostream>
#include <thread>    //线程库
#include <mutex> //互斥锁
#include <chrono>
#include <time.h>
#include <vector>    //STL容器
#include <queue>
#include <future>    //获取任务执行结果
#include <functional>
#include <utility>
#include <condition_variable>   //条件变量，负责唤醒和挂起线程
#include <string>
#include <shared_mutex> //读写锁，实现读写分离操作
using namespace std;

template<typename T>
struct safe_queue {     //任务队列
    queue<T>que;
    shared_mutex _m;        //读写锁
    bool empty() {
        shared_lock<shared_mutex>lc(_m);
        return que.empty();
    }
    auto size() {
        shared_lock<shared_mutex>lc(_m);
        return que.size();
    }
    void push(T& t) {
        unique_lock<shared_mutex> lc(_m);
        que.push(t);
    }
    bool pop(T& t) {
        unique_lock<shared_mutex> lc(_m);
        if (que.empty())return false;
        t = move(que.front());
        que.pop();
        return true;
    }
};


class ThreadPool {  //线程池
private:
    class worker {  //工作线程
    public:
        ThreadPool* pool;
        worker(ThreadPool* _pool) : pool{ _pool } {}
        void operator ()() {
            while (!pool->is_shut_down) {
                {
                    unique_lock<mutex> lock(pool->_m);
                    pool->cv.wait(lock, [this]() {
                        return this->pool->is_shut_down || !this->pool->que.empty();
                        });
                }
                function<void()>func;
                bool flag = pool->que.pop(func);
                if (flag) {
                    func();
                }
            }
        }
    };  //——————————工作线程区域——————————
public:
    bool is_shut_down;
    safe_queue<std::function<void()>> que;
    vector<std::thread>threads;
    mutex _m;
    condition_variable cv;
    ThreadPool(int n) : threads(n), is_shut_down{ false } {
        for (auto& t : threads)t = thread{ worker(this) };
    }
    ThreadPool(const ThreadPool&) = delete;     //拷贝构造函数
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    template <typename F, typename... Args>
    auto submit(F&& f, Args &&...args) -> std::future<decltype(f(args...))> {
        function<decltype(f(args...))()> func = [&f, args...]() {return f(args...); };
        auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
        std::function<void()> warpper_func = [task_ptr]() {
            (*task_ptr)();
        };
        que.push(warpper_func);
        cv.notify_one();
        return task_ptr->get_future();
    }
    ~ThreadPool() {
        auto f = submit([]() {});
        f.get();
        is_shut_down = true;
        cv.notify_all(); // 通知，唤醒所有工作线程
        for (auto& t : threads) {
            if (t.joinable()) t.join();
        }
    }
};
mutex _m;
int main()
{

    ThreadPool pool(8);
    int n = 20;
    for (int i = 1; i <= n; i++) {
        pool.submit([](int id) {
            if (id % 2 == 1) {
                this_thread::sleep_for(0.2s);
            }
        unique_lock<mutex> lc(_m);
        cout << "id : " << id << endl;
            }, i);
    }
}