#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <windows.h>
#include <mutex>
#include <future>

/*
    �����帶�� �����ϴ� ���� �޸�
    ������ �����尡 ������ �� �ִ� ����
*/

// thread_local : �����常 ������ �� �ִ� TLS �������� �з���
// �����帶�� �����ϰ� ������ �־�� �ϴ� ������ ������ �� ����
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

    // ��������� �ڽŸ��� ���̵� �ο�����
    for (int32 i = 0; i < 10; i++)
    {
        int32 threadId = i + 1;
        threads.push_back(thread(ThreadMain, threadId));
    }

    for (thread& t : threads)
        t.join();

    return 0;
}