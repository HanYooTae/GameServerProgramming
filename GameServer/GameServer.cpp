#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

class SpinLock
{
public:
    void lock()
    {
        // CAS (Compare-And-Swap) : �ѹ濡 �����ϰԲ� atomic���� �����ִ� �Ϸ��� �Լ�

        bool expected = false;  // _locked�� �������� ����
        bool desired = true;

        // CAS �ǻ��ڵ�
        /*if (_locked == expected)
        {
            expected = _locked;
            _locked = desired;
            return true;
        }
        else
        {
            expected = _locked;
            return false;
        }*/

        // �����ߴٸ� ������ ������ �����ϰڴ�.
        while (_locked.compare_exchange_strong(expected, desired) == false)
        {
            // compare_exchange_strong�� ù ��° �Ķ���ʹ� ���۷����� �ޱ� ������ ���� ����ؼ� �ٲ�. �׷��� ù ��° �Ķ���ʹ� �ʱ�ȭ�� ������� ��.
            expected = false;
        }

        /*while (_locked)
        {

        }

        _locked = true;*/
    }

    void unlock()
    {
        //_locked = false;
        _locked.store(false);
    }

private:
    //volatile bool _locked = false;
    atomic<bool> _locked = false;
};

int32 sum = 0;
SpinLock spinLock;

void Add()
{
    for (int32 i = 0; i < 10'0000; i++)
    {
        lock_guard<SpinLock> guard(spinLock);
        sum++;
    }
}

void Sub()
{
    for (int32 i = 0; i < 10'0000; i++)
    {
        lock_guard<SpinLock> guard(spinLock);
        sum--;
    }
}

int main()
{
    thread t1(Add);
    thread t2(Sub);

    t1.join();
    t2.join();

    cout << sum << '\n';
}