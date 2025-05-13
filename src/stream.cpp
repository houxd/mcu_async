#include "stream.h"
#include <poll.h>
#include <timeout.h>

static auto start_stream() {
    // 创建128
    return Stream<int>(128, [](Stream<int>::Producer producer){
        // 每秒产生一个数据
        set_interval(1000, [=](Timeout timer){
            static int n = 0;
            producer.send(++n);
            if(n>10) {
                producer.finish();
                timer.stop();
            }
        });
    });
}

void test_stream()
{
    printf("start\n");
    start_stream().recv([](Stream<int>::Consumer& consumer){
        // 接收所有数据
        int n = consumer.buf.size();        // 获取当前元素个数
        int *data = new int[n];             // 创建接收数组
        int m = consumer.buf.copy_to(data, n); // 拷贝, 由于拷贝数量可能少于n(当有多个接收端时), 获取返回的拷贝数量m(真正拷贝的数量)
        consumer.buf.pop(m);                // 弹出拷贝的元素个数
        for(int i = 0; i < m; i++) {        // 打印数据
            printf("%d\n", data[i]);
        }
        delete[] data;                      // 释放接收数据

        // 检查是否结束
        if(consumer.finished) {             // 检查流关闭
            printf("finished\n");
        }
    });
}
