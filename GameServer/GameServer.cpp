#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <windows.h>
#include <mutex>
#include <future>

// atomic 연산은 모든 쓰레드가 동일 객체에 대해서 동일한 수정 순서를 관찰한다.

atomic<int64> num;
// 동일한 수정 순서
// 변경되는 값들의 순서대로 진행이 되는 것을 의미한다. 시간의 흐름을 거스를 순 없다.
// 변수의 값이 0 -> 2 -> 1 -> 5 순서대로 변경된다고 해보자.
//현재 변수의 값이 1로 수정이 되어도, 다른 쓰레드 입장에서는 캐시 등 여러 가지 문제가 얽혀있기 때문에 당장 1이라는 값을 보지 못하고 이전 값인 2를 관찰할 수도 있다.


// CPU가 한 번에 처리할 수 있는 연산을 원자적인 연산이라고 한다.
// 여러 단계로 쪼개서 실행해야 하는 연산을 원자적이지 않은 연산이라고 한다.
// atomic의 is_lock_free() 멤버함수를 통해 원자적으로 처리가 가능하도록 지원하는지 아닌지를 체크할 수 있다. 지원하지 않으면 atomic 클래스에 의해 원자성을 부여받는다.

atomic<int64> num1;
atomic<int64> num2;
// 동일한 객체
// num1과 num2은 서로에게 아무런 영향을 미치지 않는다.
// num1과 num2를 같이 수정할 경우에, 순서가 엎치락 뒤치락할 수 있다.

atomic<bool> flag;


int main()
{
    //flag = true;
    flag.store(true, memory_order::memory_order_seq_cst);

    //bool val = flag;
    bool val = flag.load(memory_order::memory_order_seq_cst);

    // 이전 flag값을 prev에 넣고, flag값을 수정
    {
        // flag의 이전 값을 꺼내서 prev에 저장했을 수 있다고 생각할 수 있지만,
        // 만약 다른 쓰레드에서 flag에 접근해서 수정하는 상황이 있다면 prev의 값이 더이상 유효하지 않는다.
        //bool prev = flag;
        //flag = true;

        // exchange에서 리턴해주는 값이 flag의 이전 값을 의미
        bool prev = flag.exchange(true);
    }

    // CAS (Compare-And-Swap) 조건부 수정
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

        // compare_exchange_strong과 유사하지만, Spirious Failure라는 상황이 발생할 수 있다는 차이점이 존재한다.
        // 다른 쓰레드의 Interruption을 받아서 중간에 실패할 수 있음.
        // 원래는 compare_exchange_weak를 일반적으로 사용하지만, 하드웨어에 따라 Spirious Failure가 발생할 수 있다.

        // compare_exchange_strong는 Spirious Failure가 일어나더라도 다시 정상적인 값을 리턴하는 상황이 될 때까지 반복을 해준다.
        // 사용하는 머신에 따라서 compare_exchange_strong이 조금 더 무거울 수 있다.

        // compare_exchange_weak를 사용하려면 Spirious Failure가 발생하더라도, 될 때까지 무한루프를 돌려주는게 일반적이다.
        /*while (true)
        {
            bool expected = false;
            bool desired = true;
            flag.compare_exchange_weak(expected, desired);
        }*/
    }

    return 0;
}