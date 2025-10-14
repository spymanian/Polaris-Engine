#pragma once
#include <functional>
#include <chrono>
#include <iostream>
#include <random>
#include <optional>

/**
 * @brief A deterministic tick scheduler with timing, callbacks, and optional profiling.
 * 
 * Example usage:
 * 
 * Scheduler sched(0.1, 1000);
 * sched.set_on_step([](int step, double dt) {
 *     std::cout << "Step " << step << " Δt=" << dt << "\n";
 * });
 * sched.run([](double dt, int step) {
 *     // world.update(dt);
 * });
 */
class Scheduler {
    double dt_;
    int maxSteps_;
    bool paused_ = false;
    unsigned long seed_;
    std::optional<std::function<void(int, double)>> onStep_;
    std::optional<std::function<void()>> onStart_;
    std::optional<std::function<void()>> onEnd_;
    bool enableProfiling_ = false;

public:
    Scheduler(double dt, int maxSteps, unsigned long seed = 42)
        : dt_(dt), maxSteps_(maxSteps), seed_(seed) {}

    void set_on_start(std::function<void()> fn) { onStart_ = std::move(fn); }
    void set_on_step(std::function<void(int, double)> fn) { onStep_ = std::move(fn); }
    void set_on_end(std::function<void()> fn) { onEnd_ = std::move(fn); }
    void enable_profiling(bool enable) { enableProfiling_ = enable; }
    void pause() { paused_ = true; }
    void resume() { paused_ = false; }
    void reset_seed(unsigned long seed) { seed_ = seed; }

    template <typename UpdateFunc>
    void run(UpdateFunc update) {
        if (onStart_) (*onStart_)();

        std::mt19937 rng(seed_);
        for (int i = 0; i < maxSteps_; ++i) {
            if (paused_) break;

            auto startTime = std::chrono::high_resolution_clock::now();
            update(dt_, i);
            if (onStep_) (*onStep_)(i, dt_);

            if (enableProfiling_) {
                auto endTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> stepDuration = endTime - startTime;
                std::cout << "[Profiler] Step " << i << " took "
                          << stepDuration.count() << " ms\n";
            }
        }

        if (onEnd_) (*onEnd_)();
    }

    [[nodiscard]] double dt() const { return dt_; }
    [[nodiscard]] int max_steps() const { return maxSteps_; }
    [[nodiscard]] unsigned long seed() const { return seed_; }
};
