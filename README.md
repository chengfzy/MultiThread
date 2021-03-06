# MultiThread
Study Code for MultiThread in C++

## Content
Eg01 - Eg05: Windows的多线程方法, Eg06-: C++ thread的方法
1. Eg01_CreateThread: 创建线程: CreateThread(), _beginthreadex().原子操作: InterlockedXXX()
2. Eg02_ThreadSync: 线程同步, Critical Section, Event, PulseEvent, Mutex, Semaphore
3. Eg03_ProducerConsumer: 生产者消费者问题, 1个消费者或多个消费者的情况
4. Eg04_ReaderWriter: 读者写者问题
5. Eg05_SRWLock: 读写销SRWLock，专门处理读者写者问题
6. Eg06_ThreadBasic: Basic use of multithread
7. Eg07_ThreadVector: Thread Vector,使用thread组成线程池，操作同一个对象
8. Eg08_StdProducerConsumer: 生产者消费者问题, 1个消费者或多个消费者的情况，Eg03_ProducerConsumer使用Std C++方法实现


## NOTE
### Eg01: 创建线程,原子操作
创建线程: CreateThread(), _beginthreadex().

原子操作: InterlockedXXX(), InterlockedIncrement(), InterlockedAdd(), InterlockedDecrement().

_beginthreadex()函数在创建新线程时会分配并初始化一个_tiddata块。这个_tiddata块自然是用来存放一些需要线程独享的数据。事实上新线程运行时会首先将_tiddata块与自己进一步关联起来。然后新线程调用标准C运行库函数如strtok()时就会先取得_tiddata块的地址再将需要保护的数据存入_tiddata块中。这样每个线程就只会访问和修改自己的数据而不会去篡改其它线程的数据了。因此，如果在代码中有使用标准C运行库中的函数时，尽量使用_beginthreadex()来代替CreateThread()。

## Eg02: 线程同步

Critical Section: InitializeCriticalSection, DeleteCriticalSection, EnterCriticalSection, LeaveCriticalSection

Event: CreateEvent, CloseHandle, WaitForSingleObject, SetEvent, ResetEvent

Mutex: CreateMutex, CloseHandle, WaitForSingleObject, ReleaseMutex

Semaphore: CreateSemaphore, CloseHandle, WaitForSingleObject, ReleaseSemaphore

PulseEvent: 将事件触发后立即将事件设置为未触发，相当于触发一个事件脉冲.

1. 对于手动置位事件，所有正处于等待状态下线程都变为可调度状态
2. 对于自动置位事件，所有正处于等待状态下线程只有一个可变成可调度状态

## Eg05: 读写销SRWLock: 不用删除，系统自动清理

SRWLock: InitializeSRWLock,  不用删除，系统自动清理

AcquireSRWLockExclusive, ReleaseSRWLockExclusive, AcquireSRWLockShared, ReleaseSRWLockShared
