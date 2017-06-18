#include <chrono>
#include <cstdio>
#include <string>
#include <unistd.h>
#include <unordered_set>

#include "actor.h"

class Simulation {
public:
    static constexpr int kBaseTickTimeout_us = 1000000 / 48;
    static constexpr int kExtraTickTimeout_us = 1000000 / 48;

    Simulation() : next_id_(0), actors_(), last_tick_(std::chrono::steady_clock::now()) {}

    void add(Actor *actor) {
        actors_.insert(actor);
    }

    void tick_all() {
        auto now = std::chrono::steady_clock::now();
        uint64_t delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick_).count();
        last_tick_ = now;
        for (Actor *actor : actors_) {
            actor->tick(delta);
        }
    }

    void run() {
        srand(time(nullptr));
        for (;;) {
            ::usleep(kBaseTickTimeout_us + rand() % kExtraTickTimeout_us);
            tick_all();
        }
    }

    int next_id() {
        return next_id_++;
    }

protected:
    int next_id_;
    std::unordered_set<Actor *> actors_;
    std::chrono::time_point<std::chrono::steady_clock> last_tick_;
};

int main() {
    Simulation sim;
    Actor a(sim.next_id());
    sim.add(&a);
    sim.run();
}
