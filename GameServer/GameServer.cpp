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

// mutex�� ¦���� ����
// User-Level Object (Ŀ�� ������ƮX)
// �̺�Ʈó�� ȣ��
condition_variable cv;

// mutex �Ӹ� �ƴ϶� �Ϲ����� ��Ȳ���α��� Ȯ���ϰ� �ʹٸ� ���
//#include <condition_variable>
//condition_variable_any cv;

/*
    Kelnel Object
    - �ٸ� ���α׷����� �̺�Ʈ�� �̿��Ͽ� ����ȭ ����

    User-Level Object
    - ������ ���α׷� ���ο����� ��� ����
*/


void Producer()
{
    while (true)
    {
        // 1) Lock�� ���
        // 2) ���� ���� ���� ����
        // 3) Lock�� Ǯ��
        // 4) ���Ǻ����� ���� �ٸ� �����忡�� �뺸
        {
            unique_lock<mutex> lock(m);
            q.push(100);
        }

        cv.notify_one();    // wait ���� �����尡 ������ 1���� �����.
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
        // 1) Lock�� ���(�����ִٸ� ��ŵ)
        // 2) ���� Ȯ��
        // - ������ �����ϸ� �������ͼ� �̾ �ڵ带 ����
        // - ������ �������� ������ Lock�� Ǯ���ְ� ��� ���·� ��ȯ

        // Producer�Լ����� notify_one�� ������ �׻� ���ǽ��� �����ϴ°� �ƴѰ�?
        // Producer�Լ����� notify_one�� lock�� �����̱� ������ lock�� ��� �ִ� ���� �ƴϴ�.(Spurious Wakeup)
        // ���� �ٸ� �����忡�� q�� pop�ߴٸ�? q�� ������� ���� �ֱ� ������, �׷� ���� lock�� �ɾ���� ��.

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
    handle = ::CreateEvent(NULL/*���ȼӼ�*/, FALSE/*bManualReset*/, FALSE/*bInitialState*/, NULL);

    thread t1(Producer);
    thread t2(Consumer);

    t1.join();
    t2.join();

    ::CloseHandle(handle);
}