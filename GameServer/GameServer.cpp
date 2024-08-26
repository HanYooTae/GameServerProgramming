#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

// STL���� �����ϴ� �����̳ʵ��� �⺻������ ��Ƽ������ ȯ�濡�� �������� �ʴ´ٰ� �����ؾ� ��


vector<int32> v;

// Mutual Exclusive (��ȣ��Ÿ��)
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
        // ���������δ� �̱� ������� �����ϴ� ���̶�� �� �� ����

        // �ڹ��� ��ױ�
        //m.lock();
        //std::lock_guard<std::mutex> lockguard(m);

        // ���� lock�� ���� �ʰ�, �������̽��� ������
        std::unique_lock<std::mutex> uniqueLock(m, std::defer_lock);

        // uniqueLock�� �ᱸ�� ������ lockguard�� �����ϰ� ���� (���� ������ �ڷ� �̷� �� ����)
        uniqueLock.lock();

        v.push_back(i);

        if (i == 5000)
        {
            //m.unlock();
            break;
        }

        // �ڹ��� Ǯ��
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