#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include "AccountManager.h"
#include "UserManager.h"


/*
    Lock 구현 이론
    1. Spine Lock(앞에서 계속 기다리기 -> 나 들어가도 돼? 계속 물어봄 like tick함수)
    2. Sleep 함수(기다리기 시간아까우니 잠시 돌아갔다가 다시 오는 방법) -> 왔다갔다하는 비용이 발생(컨텍스트 스위칭)
    3. 이벤트를 사용하는 방법(다른 프로세스에게 부탁해서 lock이 풀리면 알려달라 -> 메모리 더 많이 씀) -> 왔다갔다하는 비용이 발생(컨텍스트 스위칭)

    컨텍스트 스위칭 : 유저레벨에서 커널모드, 커널모드에서 유저레벨로 왔다갔다 하는 것
*/

/*
    ** DeadLock이 발생하는 상황
    
    1. NULL포인터를 잘못 참조
    2. 교착 상태

    ** DeadLock 해결법
    1. 미연에 예방을 하는 개념이 아니기 때문에 조심하면서 사용해야 함
    2. 문제가 발생하면 고치기 쉽다.
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

    //std::lock(m1, m2);      // m1과 m2의 일관적인 순서를 정해줘서 순서대로 잠궈줌

    // adopt_lock : 이미 lock된 상태니까 나중에 소멸될 때 unlock을 해라
    lock_guard<mutex> g1(m1, std::adopt_lock);
    lock_guard<mutex> g2(m2, std::adopt_lock);
}