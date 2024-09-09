/*
 * Baund Studio Engine
 *
 * TaskQueue.cpp
 * v1.0.5
 *
 * mega.winter0424@gmail.com
 */

#include "TaskQueue.hpp"

namespace Baund::StudioEngine {
    TaskQueue::~TaskQueue() noexcept {
        try {
            {
                auto lock = this->getLock();
                n.stop  = true;
                n.pause = false;
            }
            n.cv.notify_all();
            n.thread.join();
        } catch (...) {
            // 강제 해제.
        }
    }
    auto TaskQueue::reset() -> void {
        auto lock = this->getLock();
        this->n.stop  = false;
        this->n.pause = false;
        this->n.queue = Queue();
    }
    auto TaskQueue::enqueue(Task task) -> void {
        {
            auto lock = this->getLock();
            n.queue.push(task);
        }
        if (not n.pause) n.cv.notify_all();
    }
    auto TaskQueue::isEmpty() -> Size {
        auto lock = this->getLock();
        return n.queue.empty();
    }
    auto TaskQueue::processing() -> void {
        while (true) {
            auto task = Task();
            {
                auto lock = std::unique_lock<std::mutex>(this->getMutex());
                n.cv.wait(lock,[this]{ return n.stop or (not n.queue.empty() and not n.pause); });
                if (n.stop and n.queue.empty()) return;
                n.running = true;
                task      = n.queue.front();
                n.queue.pop();
            }
            if (task) task();
            n.running = false;
        }
    }
}
