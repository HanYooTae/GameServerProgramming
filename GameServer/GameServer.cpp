#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

// STL에서 제공하는 컨테이너들은 기본적으로 멀티쓰레드 환경에서 동작하지 않는다고 가정해야 함


vector<int32> v;

// Mutual Exclusive (상호배타적)
mutex m;

// RAII : (Resource Acquisition Is Initialization)
template<typename T>
class LockGuard
{
public:
    LockGuard(T& m)
    {
        _mutex = &m;
        _mutex->lock();
    }

    ~LockGuard()
    {
        _mutex->unlock();
    }

private:
    T* _mutex;
};


void Push()
{
    for (int32 i = 0; i < 10000; i++)
    {
        // 실질적으로는 싱글 쓰레드로 동작하는 것이라고 볼 수 있음

        // 자물쇠 잠그기
        //m.lock();
        //std::lock_guard<std::mutex> lockguard(m);

        // 당장 lock을 걸지 않고, 인터페이스만 만들어둠
        std::unique_lock<std::mutex> uniqueLock(m, std::defer_lock);

        // uniqueLock을 잠구는 순간에 lockguard와 동일하게 동작 (잠기는 시점을 뒤로 미룰 수 있음)
        uniqueLock.lock();

        v.push_back(i);

        if (i == 5000)
        {
            //m.unlock();
            break;
        }

        // 자물쇠 풀기
        //m.unlock();
    }
}

int main()
{
    //v.reserve(20000);
    std::thread t1(Push);
    std::thread t2(Push);

    t1.join();
    t2.join();

    cout << v.size() << '\n';
}