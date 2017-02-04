# MultiThread
Study Code for MultiThread in C++

## Content
1. Eg01_CreateThread: 创建线程: CreateThread(), _beginthreadex().原子操作: InterlockedXXX()
2. Eg02_ThreadSync:

## NOTE
### Eg01: 创建线程,原子操作
创建线程: CreateThread(), _beginthreadex().

原子操作: InterlockedXXX(), InterlockedIncrement(), InterlockedAdd(), InterlockedDecrement().

_beginthreadex()函数在创建新线程时会分配并初始化一个_tiddata块。这个_tiddata块自然是用来存放一些需要线程独享的数据。事实上新线程运行时会首先将_tiddata块与自己进一步关联起来。然后新线程调用标准C运行库函数如strtok()时就会先取得_tiddata块的地址再将需要保护的数据存入_tiddata块中。这样每个线程就只会访问和修改自己的数据而不会去篡改其它线程的数据了。因此，如果在代码中有使用标准C运行库中的函数时，尽量使用_beginthreadex()来代替CreateThread()。
