#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>

// �ܹ߼� �̺�Ʈ�� ���Ǻ������� �� ������ ����� ����.
// future(�̷� ��ü)
/*
    �ϳ��� �Լ��� �񵿱������ ó���ϰ� ���� �� ���
    ��������� �����带 ���� �����ϰ� ������ �ʿ����.
*/

int64 Calculate()
{
    int64 sum = 0;

    for (int32 i = 0; i < 100'000; i++)
        sum += i;

    return sum;
}

void PromiseWorker(std::promise<string>&& promise)
{
    promise.set_value("Secret Message");
}

void TaskWorker(std::packaged_task<int64(void)>&& task)
{
    task();
}

int main()
{
    // ����(synchronous) ȣ�� ���
    /*int64 sum = Calculate();
    cout << sum << '\n';*/

    // std::async
    // �ϳ��� ���� �����带 ���� ������� �޾���
    {
        /*
            1. std::launch::deffered
             �� �����ؼ� ����(lazy evaluation)
             �� ���� ������ future.get()�� �������� �ٲ�.
             �� ���� ����, ��û�ϴ� ������ ȣ���ϴ� ������ ������.

            2. std::launch::async
             �� ������ �����带 ���� ����
             �� future.get()�� ȣ���� �� future�� ��ϵǾ��ִ� �Լ��� ������� �޾ƿ´�.
             �� ���� �Լ��� ����� �����س��� ���ߴٸ�, ��ٷȴٰ� ������� �޾ƿ´�.
        */
        std::future<int64> future = std::async(std::launch::async, Calculate);

        /*
            ������ ����(�ڵ�)
        */

        // �ϰ��� �������� �ȳ������� Ȯ���ϰ� �ʹٸ�
        // // wait �迭 �Լ��� �����ϴ��� ���ο� ���� get�� ȣ���ϴ� ������� �޶������� ����.
        // future.wait() -> ������ ��ٸ�
        std::future_status status = future.wait_for(1ms);   // 1ms��ŭ�� ��ٷ���
        if (status == future_status::ready)
        {
            // �ϰ��� ������ ó���� ����
        }

        int64 sum = future.get();
    }

    {
        // future�� �����Լ��� static�Լ� �Ӹ� �ƴ϶� ����Լ��� ȣ���� �� �ִ�.
        class Knight
        {
        public:
            int64 GetHP() { return 100; }
        };

        Knight* knight;

        std::future<int64> future2 = std::async(std::launch::async, &Knight::GetHP, knight);
    }

    // std::promise
    // �̷�(std::future)�� ������� ��ȯ���ٰŶ� ���
    // promise�� �����͸� �־��ְ� future��ü�� ���� �޾��ִ� ����
    {
        // promise�� future�� 1��1�� ������ ����
        std::promise<string> promise;

        // future�� main thread�� �������� ������ ����
        std::future<string> future = promise.get_future();

        // promise�� �������� thread t�� ������ ����
        thread t(PromiseWorker, std::move(promise));

        // promise�� setValue�� �޾ƿ�
        string message = future.get();
        cout << message << '\n';

        t.join();
    }

    // std::packaged_task
    // �̹� �����ϴ� �����带 ������� �ϰ��� ���� �Ѱ��� ������
    // �� ������� future�� �޾��ִ� ���
    {
        // int64(void) => output(input)
        // Calculate�Լ��� type�� int64�� ������ output�� int64
        // �Ķ���Ͱ� ���� ������ input�� void
        std::packaged_task<int64(void)> task(Calculate);
        std::future<int64> future = task.get_future();

        std::thread t(TaskWorker, std::move(task));

        int64 sum = future.get();
        cout << sum << '\n';

        t.join();
    }

    // ���
    // mutex, condition_variable���� ���� �ʰ�, �ܼ��� ������ ó���� �� �ִ¿���
    // Ư���� �� ���� �߻��ϴ� �̺�Ʈ�� ����
    // ex) ����µ� ����� Į�� �� �ʿ����.
    
    // 1. async : ���ϴ� �Լ��� �񵿱������� ����
    // 2. promise : ������� promise�� ���� future�� �޾���
    // 3. packaged_task : ���ϴ� �Լ��� ���� ����� packaged_task�� ���� future�� �޾���

    return 0;
}