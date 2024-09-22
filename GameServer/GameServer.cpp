#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>

// 단발성 이벤트는 조건변수보다 더 가벼운 방법이 존재.
// future(미래 객체)
/*
    하나의 함수만 비동기식으로 처리하고 싶을 때 사용
    명시적으로 쓰레드를 새로 생성하고 관리할 필요없음.
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
    // 동기(synchronous) 호출 방식
    /*int64 sum = Calculate();
    cout << sum << '\n';*/

    // std::async
    // 하나의 전용 쓰레드를 만들어서 결과물을 받아줌
    {
        /*
            1. std::launch::deffered
             ㄴ 지연해서 실행(lazy evaluation)
             ㄴ 실행 시점만 future.get()의 시점으로 바뀜.
             ㄴ 쉽게 말해, 요청하는 시점과 호출하는 시점이 나눠짐.

            2. std::launch::async
             ㄴ 별도의 쓰레드를 만들어서 실행
             ㄴ future.get()을 호출할 때 future에 등록되어있는 함수의 결과물을 받아온다.
             ㄴ 아직 함수의 결과를 도출해내지 못했다면, 기다렸다가 결과물을 받아온다.
        */
        std::future<int64> future = std::async(std::launch::async, Calculate);

        /*
            나머지 연산(코드)
        */

        // 일감이 끝났는지 안끝났는지 확인하고 싶다면
        // // wait 계열 함수를 실행하는지 여부에 따라 get을 호출하는 결과물이 달라지지는 않음.
        // future.wait() -> 무한정 기다림
        std::future_status status = future.wait_for(1ms);   // 1ms만큼만 기다려봄
        if (status == future_status::ready)
        {
            // 일감이 끝나고 처리할 내용
        }

        int64 sum = future.get();
    }

    {
        // future는 전역함수와 static함수 뿐만 아니라 멤버함수도 호출할 수 있다.
        class Knight
        {
        public:
            int64 GetHP() { return 100; }
        };

        Knight* knight;

        std::future<int64> future2 = std::async(std::launch::async, &Knight::GetHP, knight);
    }

    // std::promise
    // 미래(std::future)에 결과물을 반환해줄거라 약속
    // promise에 데이터를 넣어주고 future객체를 통해 받아주는 형태
    {
        // promise와 future가 1대1로 연동된 상태
        std::promise<string> promise;

        // future는 main thread가 소유권을 가지고 있음
        std::future<string> future = promise.get_future();

        // promise의 소유권은 thread t가 가지고 있음
        thread t(PromiseWorker, std::move(promise));

        // promise의 setValue를 받아옴
        string message = future.get();
        cout << message << '\n';

        t.join();
    }

    // std::packaged_task
    // 이미 존재하는 쓰레드를 대상으로 일감을 만들어서 넘겨준 다음에
    // 그 결과물을 future로 받아주는 방식
    {
        // int64(void) => output(input)
        // Calculate함수의 type은 int64기 때문에 output은 int64
        // 파라미터가 없기 때문에 input은 void
        std::packaged_task<int64(void)> task(Calculate);
        std::future<int64> future = task.get_future();

        std::thread t(TaskWorker, std::move(task));

        int64 sum = future.get();
        cout << sum << '\n';

        t.join();
    }

    // 결론
    // mutex, condition_variable까지 가지 않고, 단순한 연산을 처리할 수 있는역할
    // 특히나 한 번만 발생하는 이벤트에 유용
    // ex) 닭잡는데 소잡는 칼을 쓸 필요없다.
    
    // 1. async : 원하는 함수를 비동기적으로 실행
    // 2. promise : 결과물을 promise를 통해 future로 받아줌
    // 3. packaged_task : 원하는 함수의 실행 결과를 packaged_task를 통해 future로 받아줌

    return 0;
}