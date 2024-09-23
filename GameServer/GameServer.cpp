#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <windows.h>
#include <mutex>
#include <future>

/*
    Memory Model (정책)

    1) Sequentially Consistent (seq_cst)
    2) Acquire-Release (consume, acquire, release, acq_rel)
    3) Relaxed (relaxed)

    1) seq_cst (가장 엄격 = 컴파일러 입장에서 최적화 여지가 적다 = 직관적 = 코드 재배치가 일어날 가능성이 낮은 보수적인 정책)
        ㄴ 가시성 문제와 코드 재배치 문제 해결

    2) consume - 문제가 많아서 안쓴다.

    3, 4) acquire와 release
        ㄴ release 명령 이전의 메모리 명령들이 해당 명령 이후로 재배치되는 것을 금지시킴. -> 해당 명령 전의 명령들은 재배치가 되는 것에 관여하지 않음.
        ㄴ 다른 쓰레드에서 acquire로 위와 같은 변수를 읽는다면, release 이전의 명령들이 acquire하는 순간에 관찰함 (가시성 보장)

    5) acq_rel - acquire과 release를 합친 것

    6) relaxed (자유롭다 = 컴파일러 최적화 여지가 많음 = 직관적이지 않은 끔찍한 코드 = 코드 재배치가 일어날 가능성이 높은 정책)
        ㄴ 멀티 쓰레드 환경의 재앙
        ㄴ 코드 재배치가 멋대로 되며 가시성도 해결되지 않는다.
        ㄴ 동일 객체에 대해서 동일한 수정 순서만 보장함


    atomic의 멤버함수들(store, load 등)의 디폴트 파라미터로 memory_order::memory_order_seq_cst가 생략되어있으며, 인텔, AMD의 경우 애초에 순차적 일관성을 보장하기 때문에 seq_cst를 사용해도 별다른 부하가 없다.
    ARM은 꽤 차이가 있다고 한다.
*/

/*
    메모리 모델을 사용하기 위해서 반드시 atomic을 사용해야 하는 것은 아니다.
    - CPU에서 Momory Barrier라는 기능을 제공하긴 하지만, 굳이 사용하지 않아도 된다.
        ㄴ std::atomic_thread_fence(메모리 모델);
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