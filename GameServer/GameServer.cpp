#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>

mutex m;
queue<int32> q;
HANDLE handle;

void Producer()
{
    while (true)
    {
        // 데이터를 밀어넣고 있음
        unique_lock<mutex> lock(m);
        q.push(100);
    }

    // 같은 식별자에 해당하는 커널 오브젝트를 Signal 상태로 변경(켜주는 것)
    ::SetEvent(handle);
    this_thread::sleep_for(100000ms);
}

void Consumer()
{
    // 데이터를 꺼내 쓰고 있음
    while (true)
    {
        // 커널 오브젝트가 무한 대기하도록
        // Signal 상태라면 계속 진행
        // Non - Signal 상태라면 대기
        // 제 3자를 통해 무의미하게 대기하는 것을 방지해준 모습
        ::WaitForSingleObject(handle, INFINITE);

        unique_lock<mutex> lock(m);
        if (!q.empty())
        {
            int32 data = q.front();
            q.pop();
            cout << data << '\n';
        }
    }
}

int main()
{
    // Kernel Object
    // 커널 오브젝트를 관리하기 위해서 할당되는 메모리

    // 커널 오브젝트의 속성
    // 1. Usage Count : 오브젝트를 몇명이 사용하고 있는지
    // 2. Signal(파란불) / Non - Signal(빨간불) : boolean을 사용하여 켜져있으면 Signal, 꺼져있으면 Non - Signal
    // 3. Auto / Manual : 자동인지 수동인지
    // CreateEvent는 커널 오브젝트들 중에서 상대적으로 가벼운 편이다.
    handle = ::CreateEvent(NULL/*보안속성*/, FALSE/*bManualReset*/, FALSE/*bInitialState*/, NULL);

    // handle은 어떤 이벤트인지 구별하는 식별자가 저장이 됨.
    // 커널 딴에서 어떤 이벤트를 사용하고 싶을 때, 몇번 식별자의 이벤트는 무엇을 해주세요 라고 명령을 할 때 handle을 사용함

    thread t1(Producer);
    thread t2(Consumer);

    t1.join();
    t2.join();

    // 핸들을 닫아주는 함수
    ::CloseHandle(handle);
}