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
        // �����͸� �о�ְ� ����
        unique_lock<mutex> lock(m);
        q.push(100);
    }

    // ���� �ĺ��ڿ� �ش��ϴ� Ŀ�� ������Ʈ�� Signal ���·� ����(���ִ� ��)
    ::SetEvent(handle);
    this_thread::sleep_for(100000ms);
}

void Consumer()
{
    // �����͸� ���� ���� ����
    while (true)
    {
        // Ŀ�� ������Ʈ�� ���� ����ϵ���
        // Signal ���¶�� ��� ����
        // Non - Signal ���¶�� ���
        // �� 3�ڸ� ���� ���ǹ��ϰ� ����ϴ� ���� �������� ���
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
    // Ŀ�� ������Ʈ�� �����ϱ� ���ؼ� �Ҵ�Ǵ� �޸�

    // Ŀ�� ������Ʈ�� �Ӽ�
    // 1. Usage Count : ������Ʈ�� ����� ����ϰ� �ִ���
    // 2. Signal(�Ķ���) / Non - Signal(������) : boolean�� ����Ͽ� ���������� Signal, ���������� Non - Signal
    // 3. Auto / Manual : �ڵ����� ��������
    // CreateEvent�� Ŀ�� ������Ʈ�� �߿��� ��������� ������ ���̴�.
    handle = ::CreateEvent(NULL/*���ȼӼ�*/, FALSE/*bManualReset*/, FALSE/*bInitialState*/, NULL);

    // handle�� � �̺�Ʈ���� �����ϴ� �ĺ��ڰ� ������ ��.
    // Ŀ�� ������ � �̺�Ʈ�� ����ϰ� ���� ��, ��� �ĺ����� �̺�Ʈ�� ������ ���ּ��� ��� ����� �� �� handle�� �����

    thread t1(Producer);
    thread t2(Consumer);

    t1.join();
    t2.join();

    // �ڵ��� �ݾ��ִ� �Լ�
    ::CloseHandle(handle);
}