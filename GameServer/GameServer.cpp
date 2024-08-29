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
        // CAS (Compare-And-Swap) : 한방에 실행하게끔 atomic으로 묶어주는 일련의 함수

        bool expected = false;  // _locked가 무엇인지 예상
        bool desired = true;

        // CAS 의사코드
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

        // 실패했다면 성공할 때까지 실행하겠다.
        while (_locked.compare_exchange_strong(expected, desired) == false)
        {
            // compare_exchange_strong의 첫 번째 파라미터는 레퍼런스로 받기 때문에 값이 계속해서 바뀜. 그래서 첫 번째 파라미터는 초기화를 시켜줘야 함.
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