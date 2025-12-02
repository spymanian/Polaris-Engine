#pragma once
#include <string>
#include <vector>
#include <fstream>

struct StatsSnapshot {
    int step;
    double time;
    int total_agents;
    int predators;
    int prey;
    double avg_energy;
    double avg_predator_energy;
    double avg_prey_energy;
    int births;
    int deaths;
};

class Statistics {
public:
    Statistics(const std::string& output_file, bool enabled = true);
    ~Statistics();

    void record_step(int step, double time, const class World& world);
    void record_birth();
    void record_death();
    void flush();
    void reset();

    const std::vector<StatsSnapshot>& get_history() const { return history_; }
    int total_births() const { return total_births_; }
    int total_deaths() const { return total_deaths_; }

private:
    std::string output_file_;
    bool enabled_;
    std::ofstream csv_file_;
    std::vector<StatsSnapshot> history_;
    
    // Cumulative counters
    int total_births_ = 0;
    int total_deaths_ = 0;
    
    // Per-step counters (reset each snapshot)
    int births_this_step_ = 0;
    int deaths_this_step_ = 0;

    void write_header();
    void write_snapshot(const StatsSnapshot& snap);
};
