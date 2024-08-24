#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>

atomic<int32> sum = 0;

void Add()
{
    for (int32 i = 0; i < 100'0000; i++)
    {
        //sum.fetch_add(1);
        sum++;
    }
}

void Sub()
{
    for (int32 i = 0; i < 100'0000; i++)
    {
        //sum.fetch_sub(1);
        sum--;
    }
}

// atomic사용 : Add가 먼저 실행된다면 Add가 완료될 때까지 Sub는 잠시 대기
// 누가 먼저, 누가 대기하게 만들어주는 주체는 CPU
// 하나가 실행될 때 다른 애가 대기 중이므로 병목현상이 발생할 수도 있음.

int main()
{
    cout << sum << '\n';

    std::thread t1(Add);
    std::thread t2(Sub);
    t1.join();
    t2.join();
    cout << sum << '\n';
}