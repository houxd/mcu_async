#ifndef STREAM_H
#define STREAM_H

#include <poll.h>
#include <buf.h>

template <typename T> class Stream {

public:
    struct Consumer {
        Buf<T> buf;
        bool finished;
        Consumer(int size): buf(size), finished(false){}
    };
    struct Producer {
        Func<void(T)> send;
        Func<void()> finish;
    };

    Stream(int size, const Func<void(Producer)> &init) {
        _priv = std::make_shared<Consumer>(size);
        auto weak_p = std::weak_ptr<Consumer>(_priv);
        Producer producer;
        producer.send = [weak_p](T value) {
            if(auto p = weak_p.lock()) {
                p->buf.push(value);
            }
        };
        producer.finish = [weak_p]() {
            if(auto p = weak_p.lock()) {
                p->finished = true;
            }
        };
        init(producer);
    }
    Stream(const Stream &other) {
        _priv = other._priv;
    }

    Stream<T> recv(const Func<void(Consumer&)> &recv) const {
        Shared<Consumer> p = _priv;
        set_poll([p, recv](Poll poll) {
            if(!p->buf.is_empty())
                recv(*p);
            if (p->finished)
                poll.remove();
        });
        return *this;
    }
private:
    Shared<Consumer> _priv;
};

extern void test_stream();

#endif // STREAM_H
