#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <windows.h>
#include <mutex>
#include <future>

/*
    쓰레드마다 존재하는 전역 메모리
    각각의 쓰레드가 접근할 수 있는 공간
*/

// thread_local : 쓰레드만 접근할 수 있는 TLS 영역으로 분류됨
// 쓰레드마다 고유하게 가지고 있어야 하는 정보를 정해줄 수 있음
thread_local int32 LThreadId = 0;

void ThreadMain(int32 threadId)
{
    LThreadId = threadId;

    while (true)
    {
        cout << "Hi!I'm Thread" << LThreadId << '\n';
        this_thread::sleep_for(1s);
    }
}

int main()
{
    vector<thread> threads;

    // 쓰레드들이 자신만의 아이디를 부여받음
    for (int32 i = 0; i < 10; i++)
    {
        int32 threadId = i + 1;
        threads.push_back(thread(ThreadMain, threadId));
    }

    for (thread& t : threads)
        t.join();

    return 0;
}