#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include "AccountManager.h"
#include "UserManager.h"


/*
    Lock ���� �̷�
    1. Spine Lock(�տ��� ��� ��ٸ��� -> �� ���� ��? ��� ��� like tick�Լ�)
    2. Sleep �Լ�(��ٸ��� �ð��Ʊ��� ��� ���ư��ٰ� �ٽ� ���� ���) -> �Դٰ����ϴ� ����� �߻�(���ؽ�Ʈ ����Ī)
    3. �̺�Ʈ�� ����ϴ� ���(�ٸ� ���μ������� ��Ź�ؼ� lock�� Ǯ���� �˷��޶� -> �޸� �� ���� ��) -> �Դٰ����ϴ� ����� �߻�(���ؽ�Ʈ ����Ī)

    ���ؽ�Ʈ ����Ī : ������������ Ŀ�θ��, Ŀ�θ�忡�� ���������� �Դٰ��� �ϴ� ��
*/

/*
    ** DeadLock�� �߻��ϴ� ��Ȳ
    
    1. NULL�����͸� �߸� ����
    2. ���� ����

    ** DeadLock �ذ��
    1. �̿��� ������ �ϴ� ������ �ƴϱ� ������ �����ϸ鼭 ����ؾ� ��
    2. ������ �߻��ϸ� ��ġ�� ����.
*/

void Func1()
{
    for (int32 i = 0; i < 100; i++)
    {
        UserManager::Instance()->ProcessSave();
    }
}

void Func2()
{
    for (int32 i = 0; i < 100; i++)
    {
        AccountManager::Instance()->ProcessLogin();
    }
}

int main()
{
    thread t1(Func1);
    thread t2(Func2);

    t1.join();
    t2.join();

    cout << "Jobs Done" << '\n';

    mutex m1;
    mutex m2;

    //std::lock(m1, m2);      // m1�� m2�� �ϰ����� ������ �����༭ ������� �����

    // adopt_lock : �̹� lock�� ���´ϱ� ���߿� �Ҹ�� �� unlock�� �ض�
    lock_guard<mutex> g1(m1, std::adopt_lock);
    lock_guard<mutex> g2(m2, std::adopt_lock);
}