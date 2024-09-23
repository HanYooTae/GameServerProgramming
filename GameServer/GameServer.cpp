#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <windows.h>
#include <mutex>
#include <future>

// atomic ������ ��� �����尡 ���� ��ü�� ���ؼ� ������ ���� ������ �����Ѵ�.

atomic<int64> num;
// ������ ���� ����
// ����Ǵ� ������ ������� ������ �Ǵ� ���� �ǹ��Ѵ�. �ð��� �帧�� �Ž��� �� ����.
// ������ ���� 0 -> 2 -> 1 -> 5 ������� ����ȴٰ� �غ���.
//���� ������ ���� 1�� ������ �Ǿ, �ٸ� ������ ���忡���� ĳ�� �� ���� ���� ������ �����ֱ� ������ ���� 1�̶�� ���� ���� ���ϰ� ���� ���� 2�� ������ ���� �ִ�.


// CPU�� �� ���� ó���� �� �ִ� ������ �������� �����̶�� �Ѵ�.
// ���� �ܰ�� �ɰ��� �����ؾ� �ϴ� ������ ���������� ���� �����̶�� �Ѵ�.
// atomic�� is_lock_free() ����Լ��� ���� ���������� ó���� �����ϵ��� �����ϴ��� �ƴ����� üũ�� �� �ִ�. �������� ������ atomic Ŭ������ ���� ���ڼ��� �ο��޴´�.

atomic<int64> num1;
atomic<int64> num2;
// ������ ��ü
// num1�� num2�� ���ο��� �ƹ��� ������ ��ġ�� �ʴ´�.
// num1�� num2�� ���� ������ ��쿡, ������ ��ġ�� ��ġ���� �� �ִ�.

atomic<bool> flag;


int main()
{
    //flag = true;
    flag.store(true, memory_order::memory_order_seq_cst);

    //bool val = flag;
    bool val = flag.load(memory_order::memory_order_seq_cst);

    // ���� flag���� prev�� �ְ�, flag���� ����
    {
        // flag�� ���� ���� ������ prev�� �������� �� �ִٰ� ������ �� ������,
        // ���� �ٸ� �����忡�� flag�� �����ؼ� �����ϴ� ��Ȳ�� �ִٸ� prev�� ���� ���̻� ��ȿ���� �ʴ´�.
        //bool prev = flag;
        //flag = true;

        // exchange���� �������ִ� ���� flag�� ���� ���� �ǹ�
        bool prev = flag.exchange(true);
    }

    // CAS (Compare-And-Swap) ���Ǻ� ����
    {
        bool expected = false;
        bool desired = true;

        flag.compare_exchange_strong(expected, desired);
        
        if (flag == expected)
        {
            flag = desired;
            return true;
        }
        else
        {
            expected = flag;
            return false;
        }

        // compare_exchange_strong�� ����������, Spirious Failure��� ��Ȳ�� �߻��� �� �ִٴ� �������� �����Ѵ�.
        // �ٸ� �������� Interruption�� �޾Ƽ� �߰��� ������ �� ����.
        // ������ compare_exchange_weak�� �Ϲ������� ���������, �ϵ��� ���� Spirious Failure�� �߻��� �� �ִ�.

        // compare_exchange_strong�� Spirious Failure�� �Ͼ���� �ٽ� �������� ���� �����ϴ� ��Ȳ�� �� ������ �ݺ��� ���ش�.
        // ����ϴ� �ӽſ� ���� compare_exchange_strong�� ���� �� ���ſ� �� �ִ�.

        // compare_exchange_weak�� ����Ϸ��� Spirious Failure�� �߻��ϴ���, �� ������ ���ѷ����� �����ִ°� �Ϲ����̴�.
        /*while (true)
        {
            bool expected = false;
            bool desired = true;
            flag.compare_exchange_weak(expected, desired);
        }*/
    }

    return 0;
}