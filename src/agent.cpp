#include "agent.hpp"
#include "neural_network.hpp"

// Destructor - must be defined here where NeuralNetwork is complete
Agent::~Agent() = default;

// Copy constructor - deep copy the brain
Agent::Agent(const Agent& other)
    : pos(other.pos)
    , vel(other.vel)
    , predator(other.predator)
    , energy(other.energy)
    , alive(other.alive)
    , trail(other.trail)
    , fitness(other.fitness)
    , age(other.age)
    , kills(other.kills)
    , generation(other.generation)
{
    if (other.brain) {
        brain = std::make_unique<NeuralNetwork>(other.brain->clone());
    }
}

// Copy assignment
Agent& Agent::operator=(const Agent& other) {
    if (this != &other) {
        pos = other.pos;
        vel = other.vel;
        predator = other.predator;
        energy = other.energy;
        alive = other.alive;
        trail = other.trail;
        fitness = other.fitness;
        age = other.age;
        kills = other.kills;
        generation = other.generation;
        
        if (other.brain) {
            brain = std::make_unique<NeuralNetwork>(other.brain->clone());
        } else {
            brain.reset();
        }
    }
    return *this;
}

