#pragma once

#include <chrono>
#include <mutex>
#include <thread>
#include <atomic>
#include <queue>
#include <functional>

#include "Event.h"

namespace GoL {

using EventHandler = std::function<void(Event*)>;

class EventQueue {
private:
    std::queue<Event*> queue;
    std::mutex mu;
    std::thread eventHandlerThread;
    std::atomic<bool> run;
    EventHandler handler;

public:
    EventQueue() = delete;
    EventQueue(EventHandler handler)
    : queue() {
        this->handler = handler;
    }
    ~EventQueue() {
        run = false;
        if (this->eventHandlerThread.joinable()) {
            this->eventHandlerThread.join();
        }
    }

    void Push(Event* e) {
        mu.lock();
        queue.push(e);
        mu.unlock();
    }

    void Run() {
        if (!run) {
            run = true;
            if (!eventHandlerThread.joinable()) {
                eventHandlerThread = std::thread(&EventQueue::HandleEvents, this);
            }
        } else {
            mu.unlock();
        }
    }

    void Stop() {
        if (run) {
            mu.lock();
        }
    }

private:
    void HandleEvents() {
        while (run) {
            mu.lock();
            if (!queue.empty()) {
                handler(queue.front());
                delete queue.front();
                queue.pop();
            }
            mu.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    }
};

}