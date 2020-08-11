#include <deque>
#include <mutex>

#ifndef JSCORE_BLOCK_QUEUE_H
#define JSCORE_BLOCK_QUEUE_H

template<typename T, typename Container = std::deque<T>>
class block_queue {
public:
    using value_type = typename Container::value_type;
    using container_type = Container;
    using size_type = typename Container::size_type;
private:
    Container queue;
    mutable std::mutex mu;
    std::condition_variable cv;
public:
    block_queue() = default;
    block_queue(const block_queue&) = delete;
    block_queue& operator=(const block_queue&) = delete;

    value_type pop() {
        std::unique_lock<std::mutex> guard(mu);

        cv.wait(guard, [this]() -> bool {
            return !queue.empty();
        });
        value_type val = std::move(queue.front());
        queue.pop_front();
        return std::move(val);
    }

    void push(const value_type& val) {
        {
            const std::unique_lock<std::mutex> guard(mu);
            queue.push_back(val);
        }
        cv.notify_one();
    }

    bool empty() const {
        const std::unique_lock<std::mutex> guard(mu);
        return queue.empty();
    }

    size_type size() const {
        const std::unique_lock<std::mutex> guard(mu);
        return queue.size();
    }
};

#endif //JSCORE_BLOCK_QUEUE_H
