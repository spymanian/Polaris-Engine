#include "neural_network.hpp"
#include <algorithm>

NeuralNetwork::NeuralNetwork(int input_size, int hidden_size, int output_size, unsigned seed)
    : input_size_(input_size), hidden_size_(hidden_size), output_size_(output_size) {
    
    std::mt19937 rng(seed);
    std::normal_distribution<double> dist(0.0, 0.5);
    
    // Initialize weights with Xavier initialization
    double limit_ih = std::sqrt(6.0 / (input_size + hidden_size));
    double limit_ho = std::sqrt(6.0 / (hidden_size + output_size));
    
    std::uniform_real_distribution<double> dist_ih(-limit_ih, limit_ih);
    std::uniform_real_distribution<double> dist_ho(-limit_ho, limit_ho);
    
    // Input to hidden weights
    weights_input_hidden_.resize(input_size_);
    for (int i = 0; i < input_size_; ++i) {
        weights_input_hidden_[i].resize(hidden_size_);
        for (int j = 0; j < hidden_size_; ++j) {
            weights_input_hidden_[i][j] = dist_ih(rng);
        }
    }
    
    bias_hidden_.resize(hidden_size_, 0.0);
    
    // Hidden to output weights
    weights_hidden_output_.resize(hidden_size_);
    for (int i = 0; i < hidden_size_; ++i) {
        weights_hidden_output_[i].resize(output_size_);
        for (int j = 0; j < output_size_; ++j) {
            weights_hidden_output_[i][j] = dist_ho(rng);
        }
    }
    
    bias_output_.resize(output_size_, 0.0);
}

double NeuralNetwork::activation(double x) const {
    return std::tanh(x);
}

double NeuralNetwork::tanh_activation(double x) const {
    return std::tanh(x);
}

std::vector<double> NeuralNetwork::forward(const std::vector<double>& inputs) {
    // Hidden layer
    std::vector<double> hidden(hidden_size_, 0.0);
    for (int j = 0; j < hidden_size_; ++j) {
        double sum = bias_hidden_[j];
        for (int i = 0; i < input_size_; ++i) {
            sum += inputs[i] * weights_input_hidden_[i][j];
        }
        hidden[j] = activation(sum);
    }
    
    // Output layer
    std::vector<double> outputs(output_size_, 0.0);
    for (int j = 0; j < output_size_; ++j) {
        double sum = bias_output_[j];
        for (int i = 0; i < hidden_size_; ++i) {
            sum += hidden[i] * weights_hidden_output_[i][j];
        }
        outputs[j] = tanh_activation(sum);
    }
    
    return outputs;
}

void NeuralNetwork::mutate(double mutation_rate, double mutation_strength) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<double> prob(0.0, 1.0);
    std::normal_distribution<double> mutation(0.0, mutation_strength);
    
    // Mutate input-hidden weights
    for (auto& layer : weights_input_hidden_) {
        for (auto& w : layer) {
            if (prob(rng) < mutation_rate) {
                w += mutation(rng);
                w = std::clamp(w, -2.0, 2.0);
            }
        }
    }
    
    // Mutate hidden biases
    for (auto& b : bias_hidden_) {
        if (prob(rng) < mutation_rate) {
            b += mutation(rng);
            b = std::clamp(b, -2.0, 2.0);
        }
    }
    
    // Mutate hidden-output weights
    for (auto& layer : weights_hidden_output_) {
        for (auto& w : layer) {
            if (prob(rng) < mutation_rate) {
                w += mutation(rng);
                w = std::clamp(w, -2.0, 2.0);
            }
        }
    }
    
    // Mutate output biases
    for (auto& b : bias_output_) {
        if (prob(rng) < mutation_rate) {
            b += mutation(rng);
            b = std::clamp(b, -2.0, 2.0);
        }
    }
}

NeuralNetwork NeuralNetwork::clone() const {
    NeuralNetwork copy(input_size_, hidden_size_, output_size_, 0);
    copy.weights_input_hidden_ = weights_input_hidden_;
    copy.bias_hidden_ = bias_hidden_;
    copy.weights_hidden_output_ = weights_hidden_output_;
    copy.bias_output_ = bias_output_;
    return copy;
}

std::vector<double> NeuralNetwork::get_weights() const {
    std::vector<double> all_weights;
    
    for (const auto& layer : weights_input_hidden_) {
        all_weights.insert(all_weights.end(), layer.begin(), layer.end());
    }
    all_weights.insert(all_weights.end(), bias_hidden_.begin(), bias_hidden_.end());
    
    for (const auto& layer : weights_hidden_output_) {
        all_weights.insert(all_weights.end(), layer.begin(), layer.end());
    }
    all_weights.insert(all_weights.end(), bias_output_.begin(), bias_output_.end());
    
    return all_weights;
}

void NeuralNetwork::set_weights(const std::vector<double>& weights) {
    size_t idx = 0;
    
    for (auto& layer : weights_input_hidden_) {
        for (auto& w : layer) {
            w = weights[idx++];
        }
    }
    
    for (auto& b : bias_hidden_) {
        b = weights[idx++];
    }
    
    for (auto& layer : weights_hidden_output_) {
        for (auto& w : layer) {
            w = weights[idx++];
        }
    }
    
    for (auto& b : bias_output_) {
        b = weights[idx++];
    }
}
