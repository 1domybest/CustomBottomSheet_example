/*
 * Baund Studio Engine
 *
 * TaskQueue.hpp
 * v1.0.5
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_BaseLibrary_TaskQueue_hpp
#define BaundStudioEngine_Sources_BaseLibrary_TaskQueue_hpp

#include "Sources/BaseLibrary/Definitions.hpp"
#include "Sources/BaseLibrary/Utility.hpp"
#include <queue>

namespace Baund::StudioEngine {
    class TaskQueue final
    : public virtual BaseObject {
    public:
        using Thread  = std::thread;
        using CV      = std::condition_variable;
        using Task    = std::function<void()>;
        using Queue   = std::queue<Task>;
        
        ~TaskQueue() noexcept override;
        TaskQueue() = default;
        TaskQueue(TaskQueue const&);
        TaskQueue(TaskQueue     &&);
        auto operator=(TaskQueue const&) -> TaskQueue&;
        auto operator=(TaskQueue     &&) -> TaskQueue&;
        
        auto reset  () -> void override;
        auto enqueue(Task task) -> void;
        auto isEmpty()          -> Size;
        auto pause  (bool)      -> void;
        auto isPause() const    -> bool;
        auto isValid() const -> bool override { return true; } // 임시. 제거예정.
    private:
        auto processing() -> void;
        struct NonCopyables {
            bool    stop    = false;
            bool    pause   = false;
            bool    running = false;
            Thread  thread;
            CV      cv;
            Queue   queue;
        } n{.thread = Thread(&TaskQueue::processing,this)};
    };
    
    inline TaskQueue::TaskQueue(TaskQueue const&) {
    }
    inline TaskQueue::TaskQueue(TaskQueue&&) {
    }
    inline auto TaskQueue::operator=(TaskQueue const&) -> TaskQueue& {
        return *this;
    }
    inline auto TaskQueue::operator=(TaskQueue&&) -> TaskQueue& {
        return *this;
    }
    inline auto TaskQueue::pause(bool in) -> void {
        {
            auto lock = this->getLock();
            n.pause = in;
        }
        if (not n.pause and not n.queue.empty()) n.cv.notify_all();
    }
    inline auto TaskQueue::isPause() const -> bool {
        auto lock = this->getLock();
        return n.pause and not n.running;
    }
}

#endif//BaundStudioEngine_Sources_BaseLibrary_TaskQueue_hpp
