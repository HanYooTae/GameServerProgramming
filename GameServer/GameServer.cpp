#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <windows.h>
#include <mutex>
#include <future>

/*
    Memory Model (��å)

    1) Sequentially Consistent (seq_cst)
    2) Acquire-Release (consume, acquire, release, acq_rel)
    3) Relaxed (relaxed)

    1) seq_cst (���� ���� = �����Ϸ� ���忡�� ����ȭ ������ ���� = ������ = �ڵ� ���ġ�� �Ͼ ���ɼ��� ���� �������� ��å)
        �� ���ü� ������ �ڵ� ���ġ ���� �ذ�

    2) consume - ������ ���Ƽ� �Ⱦ���.

    3, 4) acquire�� release
        �� release ��� ������ �޸� ��ɵ��� �ش� ��� ���ķ� ���ġ�Ǵ� ���� ������Ŵ. -> �ش� ��� ���� ��ɵ��� ���ġ�� �Ǵ� �Ϳ� �������� ����.
        �� �ٸ� �����忡�� acquire�� ���� ���� ������ �д´ٸ�, release ������ ��ɵ��� acquire�ϴ� ������ ������ (���ü� ����)

    5) acq_rel - acquire�� release�� ��ģ ��

    6) relaxed (�����Ӵ� = �����Ϸ� ����ȭ ������ ���� = ���������� ���� ������ �ڵ� = �ڵ� ���ġ�� �Ͼ ���ɼ��� ���� ��å)
        �� ��Ƽ ������ ȯ���� ���
        �� �ڵ� ���ġ�� �ڴ�� �Ǹ� ���ü��� �ذ���� �ʴ´�.
        �� ���� ��ü�� ���ؼ� ������ ���� ������ ������


    atomic�� ����Լ���(store, load ��)�� ����Ʈ �Ķ���ͷ� memory_order::memory_order_seq_cst�� �����Ǿ�������, ����, AMD�� ��� ���ʿ� ������ �ϰ����� �����ϱ� ������ seq_cst�� ����ص� ���ٸ� ���ϰ� ����.
    ARM�� �� ���̰� �ִٰ� �Ѵ�.
*/

/*
    �޸� ���� ����ϱ� ���ؼ� �ݵ�� atomic�� ����ؾ� �ϴ� ���� �ƴϴ�.
    - CPU���� Momory Barrier��� ����� �����ϱ� ������, ���� ������� �ʾƵ� �ȴ�.
        �� std::atomic_thread_fence(�޸� ��);
*/

atomic<bool> ready;
int32 value;

void Producer()
{
    value = 10;

    ready.store(true, memory_order::memory_order_seq_cst);
}

void Consumer()
{
    while (ready.load(memory_order::memory_order_seq_cst) == false);

    cout << value << '\n';
}

int main()
{
    ready = false;
    value = 0;
    thread t1(Producer);
    thread t2(Consumer);
    t1.join();
    t2.join();

    return 0;
}