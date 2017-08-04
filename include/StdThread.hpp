#pragma once

# include <thread>
# include <exception>

class StdThread {
public:
    StdThread() = default;
    StdThread(const StdThread &) = delete;
    StdThread(StdThread &&t) {
        swap(t);
    }

    template <typename Callable>
    explicit StdThread(Callable &&f) : _t(std::forward<Callable>(f)) {}

    template <typename Callable, typename... Args>
    StdThread(Callable &&f, Args &&... args) : _t(std::forward<Callable>(f), std::forward<Args>(args)...) {}

    ~StdThread() {}

    void swap(std::thread &t) {
        (void)(t);
        throw std::runtime_error("Cannot swap with Thread");
    }

    void swap(StdThread &t) {
        _t.swap(t.getNative());
    }

    bool joinable() const {
        return _t.joinable();
    }

    void join() {
        _t.join();
    }

    void detach() {
        _t.detach();
    }

protected:
    std::thread &getNative() {
        return _t;
    }

    std::thread _t;
};
