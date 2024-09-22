#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <windows.h>
#include <mutex>
#include <future>

// 캐시가 개입
/*
    Thread_1과 Thread_2함수 안에서 r1과 r2가 메모리에 있는 값을 불러왔다는 보장이 없다.
*/

/*
    (상황1) 한쪽에서 y의 값을 수정 (y = 1)
    (상황2) 다른 한쪽에서 y의 값을 꺼내서 읽음 (r2 = y)

    1. 캐시가 개입
    2. y의 값을 RAM에 가서 수정했다는 보장이 없음
    3. r2가 y의 메모리를 불러왔다는 보장도 없음
*/

/*
    ** 가시성 문제 **
    
    1. 멀티 쓰레드 프로그래밍이 아닌 경우라면 전혀 상관이 없다.
    2. 단일 쓰레드라면 값을 수정했을 때, 사용하던 캐시에 수정된 값이 저장이 되어있기 때문에 상관이 없다.
    3. 다수의 쓰레드가 개입을 하는 순간, 문제가 발생할 가능성이 생긴다.
     ㄴ y에 1을 대입했는데, r2는 수정된 y가 아닌 수정되기 전의 y를 읽을 수도 있음.(가시성)
     ㄴ 만약 C++이 아니라 C#이었다면, 공용 변수들 모두 volatile 키워드를 붙이면 컴파일러 최적화도 막고 가시성 문제도 해결이 되긴 하지만, 코드 재배치의 문제는 해결이 되지 않는다.

     ** 코드 재배치 문제 **
     
     1. 코드를 작성했는데, 컴파일러가 같은 결과물이면서 코드의 순서를 뒤바꾸는게 더 효율적이라고 판단한다면 코드의 순서를 임의로 변경할 수도 있다.
     2. 컴파일러가 순서를 뒤집지 않더라도, CPU가 순서를 뒤집을 수도 있다.
     2-1. CPU의 파이프라인 : Fetch(명령어 가져오기), Decode(명령어 해석), Execute(명령어 실행), Write-back(결과물을 가져다주기)
     2-2. 명령어를 해석하고 실행하는 과정에서 순서를 뒤바꾸는 것이 더 효율적이라고 컴파일러 또는 CPU가 인식은 하게 된다면 순서를 뒤바꾸는 것이다.
*/

// 가시성, 코드 재배치
int32 x = 0;
int32 y = 0;
int32 r1 = 0;
int32 r2 = 0;

volatile bool ready;

void Thread_1()
{
    while (!ready);

    y = 1;  // Store y
    r1 = x; // Load x
}

void Thread_2()
{
    while (!ready);

    x = 1;  // Store x
    r2 = y; // Load y
}

int main()
{
    int32 count = 0;

    while (true)
    {
        ready = false;
        count++;

        x = y = r1 = r2 = 0;

        thread t1(Thread_1);
        thread t2(Thread_2);

        ready = true;

        t1.join();
        t2.join();

        if (r1 == 0 && r2 == 0) break;

        //cout << count << '\n';
    }
    
    cout << count << "번 만에 빠져나옴" << '\n';

    return 0;
}