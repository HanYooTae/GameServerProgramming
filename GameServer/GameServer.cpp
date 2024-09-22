#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <windows.h>
#include <mutex>

/*
    CPU : 연산처리
    RAM : 메모리()
    캐시(cache) : 임시 저장소

    데이터를 연산하기 위해서 RAM에 있는 데이터를 꺼내서 CPU에서 연산처리해줌

    연산을 할 때마다 왔다갔다하는 것은 비효율적
    -> RAM에서 데이터를 CPU에 전송하는 비용이 CPU에서 연산하는 비용보다 훨씬 더 큼

    CPU의 코어는 ALU(연산 장치)와 캐시 장치로 이루어져 있음.
    -> 캐시 장치는 위에서부터 레지스터, L1캐시, L2캐시 순서로 이루어져있으며(L3캐시 이상으로도 존재할 수 있음), 여기까지 전부 찾아봐도 데이터가 없으면 RAM에서 데이터를 가져옴.

    RAM에서 데이터를 가져올 때 동일한 데이터가 또 필요할 수도 있으니 캐시에 저장하게 됨.

    CPU는 연산처리를 하기 위해서 캐시 장치에 저장된 메모리 중에서 해당 주소값을 가지고 있는지 체크를 해본 후에, 이미 가지고 있는 값이라면 캐시에 있는 데이터를 연산처리함. 아니라면 RAM에서 데이터를 가져옴.

    프로그램이 계속 실행되고 있을 때, 캐시 메모리에 용량이 꽉차서 더이상 저장할 수 없다면, 제일 오래되었으며 사용빈도가 가장 떨어지는 데이터를 삭제 후 새로운 데이터로 덮어씀.

    캐시 설계 철학(데이터 지역성)
     ㄴ 캐시 적중률(Cache Hit)을 높이기 위한 기법

    1. Temporal Locality(시간 지역성)
     ㄴ 방금 사용했던 데이터를 저장

    2. Spatial Locality(공간 지역성)
     ㄴ 방금 사용했던 데이터와 그 근처에 있는 데이터를 저장
*/

int32 buffer[10000][10000];

int main()
{
    memset(buffer, 0, sizeof(buffer));

    {
        uint64 start = GetTickCount64();

        int64 sum = 0;

        for (int32 i = 0; i < 10000; i++)
        {
            for (int32 j = 0; j < 10000; j++)
            {
                sum += buffer[i][j];
            }
        }

        uint64 end = GetTickCount64();

        cout << "Elapsed Tick " << (end - start) << '\n';
    }

    {
        uint64 start = GetTickCount64();

        int64 sum = 0;

        for (int32 i = 0; i < 10000; i++)
        {
            for (int32 j = 0; j < 10000; j++)
            {
                sum += buffer[j][i];
            }
        }

        uint64 end = GetTickCount64();

        cout << "Elapsed Tick " << (end - start) << '\n';
    }

    return 0;
}