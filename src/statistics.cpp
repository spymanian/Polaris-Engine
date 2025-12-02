#include "statistics.hpp"
#include "world.hpp"
#include <iostream>
#include <iomanip>
#include <numeric>

Statistics::Statistics(const std::string& output_file, bool enabled)
    : output_file_(output_file), enabled_(enabled) {
    if (enabled_) {
        csv_file_.open(output_file_);
        if (csv_file_.is_open()) {
            write_header();
            std::cout << "[Statistics] Logging to " << output_file_ << std::endl;
        } else {
            std::cerr << "[Statistics] Failed to open " << output_file_ << std::endl;
            enabled_ = false;
        }
    }
}

Statistics::~Statistics() {
    flush();
    if (csv_file_.is_open()) {
        csv_file_.close();
    }
}

void Statistics::write_header() {
    csv_file_ << "step,time,total_agents,predators,prey,avg_energy,"
              << "avg_predator_energy,avg_prey_energy,births,deaths\n";
}

void Statistics::write_snapshot(const StatsSnapshot& snap) {
    csv_file_ << snap.step << ","
              << snap.time << ","
              << snap.total_agents << ","
              << snap.predators << ","
              << snap.prey << ","
              << snap.avg_energy << ","
              << snap.avg_predator_energy << ","
              << snap.avg_prey_energy << ","
              << snap.births << ","
              << snap.deaths << "\n";
}

void Statistics::record_step(int step, double time, const World& world) {
    if (!enabled_) return;

    StatsSnapshot snap;
    snap.step = step;
    snap.time = time;
    snap.total_agents = world.agents.size();
    snap.predators = 0;
    snap.prey = 0;
    
    double total_energy = 0.0;
    double predator_energy = 0.0;
    double prey_energy = 0.0;

    for (const auto& agent : world.agents) {
        if (agent.predator) {
            snap.predators++;
            predator_energy += agent.energy;
        } else {
            snap.prey++;
            prey_energy += agent.energy;
        }
        total_energy += agent.energy;
    }

    snap.avg_energy = snap.total_agents > 0 ? total_energy / snap.total_agents : 0.0;
    snap.avg_predator_energy = snap.predators > 0 ? predator_energy / snap.predators : 0.0;
    snap.avg_prey_energy = snap.prey > 0 ? prey_energy / snap.prey : 0.0;
    snap.births = births_this_step_;
    snap.deaths = deaths_this_step_;

    history_.push_back(snap);
    write_snapshot(snap);

    // Reset per-step counters
    births_this_step_ = 0;
    deaths_this_step_ = 0;
}

void Statistics::record_birth() {
    births_this_step_++;
    total_births_++;
}

void Statistics::record_death() {
    deaths_this_step_++;
    total_deaths_++;
}

void Statistics::flush() {
    if (csv_file_.is_open()) {
        csv_file_.flush();
    }
}

void Statistics::reset() {
    history_.clear();
    total_births_ = 0;
    total_deaths_ = 0;
    births_this_step_ = 0;
    deaths_this_step_ = 0;
}
