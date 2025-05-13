#ifndef BUF_H
#define BUF_H

#include <cassert>
#include <cstring>
#include <type_traits>

template <typename T> class Buf {
    static_assert(std::is_trivially_copyable<T>::value, "Buf only supports trivially copyable types");

    T *_buf;                // the buffer 
    int _buf_size;          // 缓冲区大小
    volatile int _head = 0; // 写指针（中断/生产者）
    volatile int _tail = 0; // 读指针（主循环/消费者）

  public:
    // 构造函数，接受缓冲区大小参数
    explicit Buf(int buf_size) : _buf_size(buf_size) {
        assert(buf_size > 1); // 缓冲区大小必须大于1
        _buf = new T[buf_size];
    }

    // 析构函数，释放动态分配的内存
    ~Buf() { delete[] _buf; }

    // 移动构造函数
    Buf(Buf &&other) noexcept
        : _buf(other._buf), _buf_size(other._buf_size), _head(other._head), _tail(other._tail) {
        other._buf = nullptr;
    }

    // 移动赋值运算符
    Buf &operator=(Buf &&other) noexcept {
        if (this != &other) {
            delete[] _buf;
            _buf = other._buf;
            _buf_size = other._buf_size;
            _head = other._head;
            _tail = other._tail;
            other._buf = nullptr;
        }
        return *this;
    }

    // 禁止拷贝
    Buf(const Buf &) = delete;
    Buf &operator=(const Buf &) = delete;
    
    inline const T *buffer() const { return _buf; }
    inline T *buffer() { return _buf; }

    inline int buf_size() const { return _buf_size - 1; }
    inline int size() const {
        int h = _head, t = _tail;
        if (h >= t)
            return h - t;
        return _buf_size - t + h;
    }
    inline void reset() {
        _head = 0;
        _tail = 0;
    }
    inline bool is_empty() const { return _tail == _head; }
    inline bool is_full() const { return ((_head + 1) % _buf_size) == _tail; }

    inline T &front() {
        assert(!is_empty());
        return _buf[_tail];
    }
    inline const T &front() const {
        assert(!is_empty());
        return _buf[_tail];
    }
    inline void pop() {
        if(is_empty())
            return;
        _tail = (_tail + 1) % _buf_size;
    }
    inline int pop(int n) {
        for (int i = 0; i < n; i++) {
            if (is_empty())
                return i;
            _tail = (_tail + 1) % _buf_size;
        }
        return n;
    }
    inline bool push(const T &v) {
        int next = (_head + 1) % _buf_size;
        if (next == _tail)
            return false; // 满
        _buf[_head] = v;
        _head = next;
        return true;
    }
    inline int push(const T *many, int n) {
        for (int i = 0; i < n; i++) {
            if (!push_back(many[i]))
                return i; // 满
        }
        return n;
    }

    // 检查当前数据是否连续（适合memcpy）
    inline bool is_continuous() const {
        // 空时视为连续
        if (is_empty())
            return true;
        // _tail < _head 时数据在buffer中是连续的
        return _tail < _head;
    }

    // 拷贝n个数据到dest，返回实际拷贝的元素数量(<=n)
    inline int copy_to(T *dest, int n) const {
        if (n > size())
            n = size();
        if (n == 0)
            return 0;
        if (is_continuous()) {
            // 一段连续
            memcpy(dest, _buf + _tail, n * sizeof(T));
        } else {
            // 分两段
            int first = _buf_size - _tail;
            memcpy(dest, _buf + _tail, first * sizeof(T));
            memcpy(dest + first, _buf, (n - first) * sizeof(T));
        }
        return n;
    }
};

extern void _test_buf();

#endif // BUF_H
