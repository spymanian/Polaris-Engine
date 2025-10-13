#pragma once
#include <functional>

class Scheduler {
    double dt_;
    int maxSteps_;
public:
    Scheduler(double dt, int maxSteps)
        : dt_(dt), maxSteps_(maxSteps) {}

    template <typename UpdateFunc>
    void run(UpdateFunc update) {
        for (int i = 0; i < maxSteps_; ++i)
            update(dt_, i);
    }
};
