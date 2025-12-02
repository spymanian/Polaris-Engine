#pragma once
#include <deque>
#include <memory>

struct Vec2 { double x, y; };

class NeuralNetwork;  // Forward declaration

struct Agent {
    Vec2 pos, vel;
    bool predator = false;
    double energy = 100.0;
    bool alive = true;
    std::deque<Vec2> trail;  // Movement history for visualization
    
    // AI components
    std::unique_ptr<NeuralNetwork> brain;  // Neural network for decision making
    double fitness = 0.0;  // Fitness score (lifespan + energy gained)
    int age = 0;  // How many steps this agent has survived
    int kills = 0;  // For predators: number of prey eaten
    int generation = 0;  // Which generation this agent belongs to
    
    // Default constructor
    Agent() = default;
    
    // Copy constructor (for Python bindings and other uses)
    Agent(const Agent& other);
    
    // Move constructor and assignment
    Agent(Agent&&) = default;
    Agent& operator=(Agent&&) = default;
    
    // Copy assignment
    Agent& operator=(const Agent& other);
    
    // Destructor (must be defined in .cpp where NeuralNetwork is complete)
    ~Agent();
};
