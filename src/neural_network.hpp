#pragma once
#include <vector>
#include <random>
#include <cmath>

// Simple feedforward neural network for agent control
class NeuralNetwork {
public:
    NeuralNetwork(int input_size, int hidden_size, int output_size, unsigned seed = 42);
    
    // Forward pass: inputs -> outputs
    std::vector<double> forward(const std::vector<double>& inputs);
    
    // Mutate weights for evolution
    void mutate(double mutation_rate, double mutation_strength);
    
    // Copy network (for reproduction)
    NeuralNetwork clone() const;
    
    // Get/set weights for serialization
    std::vector<double> get_weights() const;
    void set_weights(const std::vector<double>& weights);

private:
    int input_size_;
    int hidden_size_;
    int output_size_;
    
    std::vector<std::vector<double>> weights_input_hidden_;
    std::vector<double> bias_hidden_;
    std::vector<std::vector<double>> weights_hidden_output_;
    std::vector<double> bias_output_;
    
    double activation(double x) const;
    double tanh_activation(double x) const;
};
