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

// mutex와 짝지어 동작
// User-Level Object (커널 오브젝트X)
// 이벤트처럼 호출
condition_variable cv;

// mutex 뿐만 아니라 일반적인 상황으로까지 확장하고 싶다면 사용
//#include <condition_variable>
//condition_variable_any cv;

/*
    Kelnel Object
    - 다른 프로그램과도 이벤트를 이용하여 동기화 가능

    User-Level Object
    - 동일한 프로그램 내부에서만 사용 가능
*/


void Producer()
{
    while (true)
    {
        // 1) Lock을 잡고
        // 2) 공유 변수 값을 수정
        // 3) Lock을 풀고
        // 4) 조건변수를 통해 다른 쓰레드에게 통보
        {
            unique_lock<mutex> lock(m);
            q.push(100);
        }

        cv.notify_one();    // wait 중인 쓰레드가 있으면 1개만 깨운다.
        //::SetEvent(handle);
        this_thread::sleep_for(100ms);
    }
}

void Consumer()
{
    while (true)
    {
        //::WaitForSingleObject(handle, INFINITE);

        unique_lock<mutex> lock(m);
        cv.wait(lock, []() {return !q.empty(); });
        // 1) Lock을 잡고(잡혀있다면 스킵)
        // 2) 조건 확인
        // - 조건이 만족하면 빠져나와서 이어서 코드를 진행
        // - 조건이 만족하지 않으면 Lock을 풀어주고 대기 상태로 전환

        // Producer함수에서 notify_one을 했으면 항상 조건식을 만족하는거 아닌가?
        // Producer함수에서 notify_one은 lock과 별개이기 때문에 lock을 잡고 있는 것이 아니다.(Spurious Wakeup)
        // 만약 다른 쓰레드에서 q를 pop했다면? q가 비어있을 수도 있기 때문에, 그럴 경우는 lock을 걸어줘야 함.

        //while (!q.empty())
        {
            int32 data = q.front();
            q.pop();
            cout << q.size() << '\n';
        }
    }
}

int main()
{
    handle = ::CreateEvent(NULL/*보안속성*/, FALSE/*bManualReset*/, FALSE/*bInitialState*/, NULL);

    thread t1(Producer);
    thread t2(Consumer);

    t1.join();
    t2.join();

    ::CloseHandle(handle);
}