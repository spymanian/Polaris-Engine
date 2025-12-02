# Polaris-Engine

**A high-performance C++ agent-based simulation engine with neural network AI, evolutionary learning, and real-time visualization.**

![Version](https://img.shields.io/badge/version-3.0--AI-blue)
![C++](https://img.shields.io/badge/C++-20-orange)
![AI](https://img.shields.io/badge/AI-Neural_Networks-purple)
![License](https://img.shields.io/badge/license-MIT-green)

---

## ✨ Features

### 🧠 **NEW: AI & Evolution**
- 🤖 **Neural Network Agents** - Each agent has an evolving brain (8-12-2 feedforward network)
- 🧬 **Evolutionary Learning** - Successful behaviors inherited and mutated across generations
- 📈 **Fitness Tracking** - Age, kills, energy-based selection
- 🔄 **AI Toggle** - Switch between evolved and scripted behaviors in real-time
- 🎯 **Emergent Intelligence** - Watch complex strategies evolve from random weights

### Core Simulation
- 🦊 **Predator-Prey Ecosystem** - Emergent population dynamics with rebalanced parameters
- ⚡ **Energy System** - Agents consume energy, hunt for food, and reproduce
- 🎯 **Spatial Partitioning** - Efficient grid-based collision detection (scales to 1000+ agents)
- 🧬 **Reproduction** - Energy-based birth mechanics with neural network inheritance
- 💀 **Starvation** - Death when energy depletes

### Configuration & Control
- ⚙️ **JSON Configuration** - Tweak all parameters including AI settings
- 🎮 **Real-time Controls** - Adjust mutation rates, population, energy on the fly
- 🎨 **ImGui Interface** - Live parameter adjustment with AI controls and balance indicators
- 📊 **Statistics Logging** - CSV export with evolutionary metrics
- 🌈 **Agent Trails** - Visualize movement patterns with fading trails
- 🚀 **Spawn Buttons** - Emergency population injection to prevent ecosystem collapse

### Visualization
- 🖼️ **SDL2 Rendering** - Smooth 60 FPS visualization with dynamic viewport
- 🌈 **Energy-based Colors** - Visual feedback for agent health
- 📏 **Dynamic Sizing** - Agent size reflects energy level
- 📊 **Live Graphs** - Population dynamics with balance indicators

### Python Integration
- 🐍 **pybind11 Bindings** - Use as Python module
- 🤖 **ML-Ready** - Compatible with reinforcement learning frameworks
- 📸 **Frame Export** - Render simulation frames to PNG

---

## 🚀 Quick Start

### Prerequisites
```bash
# Ubuntu/Debian
sudo apt install build-essential cmake libsdl2-dev python3-dev

# macOS
brew install cmake sdl2 pybind11

# Arch Linux
sudo pacman -S cmake sdl2 pybind11
```

### Build & Run
```bash
# Clone and build
cd Polaris-Engine
mkdir build && cd build
cmake ..
make -j4

# Run simulation with AI
./polaris
```

**First run**: Press `G` to see the AI control panel and statistics!

---

## 🎮 Controls

| Key | Action |
|-----|--------|
| `SPACE` | Pause/Resume simulation |
| `ESC` | Quit |
| `R` | Reload config.json |
| `T` | Toggle agent trails visualization |
| `G` | Toggle all UI panels (AI + Stats) |
| `C` | Toggle configuration panel (with AI controls) |
| `S` | Toggle statistics panel (with balance indicator) |

---

## 🧠 AI Configuration

Edit `config.json` to customize AI and ecosystem:

```json
{
  "num_agents": 50,
  "predator_chance": 0.25,
  
  "enable_ai": true,
  "neural_input_size": 8,
  "neural_hidden_size": 12,
  "neural_output_size": 2,
  "mutation_rate": 0.1,
  "mutation_strength": 0.2,
  
  "energy_consumption_rate": 0.3,
  "energy_gain_from_prey": 80.0,
  "reproduction_energy_threshold": 130.0,
  "prey_flee_strength": 0.035
}
```

**Key AI Parameters**:
- `enable_ai`: Toggle neural network control
- `mutation_rate`: How often weights mutate (0.0-0.5)
- `mutation_strength`: How much weights change (0.0-1.0)
- `neural_hidden_size`: Brain complexity (8-20 neurons)

---

## 📊 Watching Evolution

### Real-time Monitoring
1. Start simulation: `./build/polaris`
2. Press `S` for statistics panel
3. Watch these indicators:
   - **Balance Indicator**: Green (healthy), Yellow (warning), Red (collapse)
   - **Population Graphs**: Predator vs Prey over time
   - **Prey:Predator Ratio**: Target 3:1 to 6:1

### Preventing Collapse
If all prey die:
1. Press `C` → Click "Spawn 10 Prey"
2. Adjust sliders:
   - "Energy from Prey" → 100-120
   - "Flee Strength" → 0.04-0.05

### Observing Evolved Behaviors
After 500-1000 steps with AI enabled:
- **Predators**: Learn efficient chasing patterns
- **Prey**: Develop evasion strategies
- **Both**: Energy-conserving movement

---

## 🔬 Example Experiments

### 1. AI vs Scripted Behavior
```json
// Start with enable_ai: false
{"enable_ai": false}
// Toggle to true in UI, observe difference
```

### 2. Rapid Evolution
```json
{
  "mutation_rate": 0.3,
  "mutation_strength": 0.4,
  "num_agents": 100
}
```
High mutation + large population = visible evolution in 100 steps.

### 3. Stable Ecosystem
```json
{
  "energy_consumption_rate": 0.3,
  "energy_gain_from_prey": 80.0,
  "predator_chance": 0.25,
  "prey_flee_strength": 0.035
}
```
Balanced parameters for long-term stability.

### 4. Complex Brains
```json
{
  "neural_hidden_size": 20,
  "mutation_rate": 0.05
}
```
Larger brains + slower evolution = sophisticated strategies.

---

## 🐍 Python API

Use Polaris as a Python module:

```python
import simulon

# Create environment
env = simulon.SimulonEnv(n_agents=50, seed=42, dt=0.1)

# Run simulation
for _ in range(1000):
    env.step()
    state = env.get_state()  # Get agent positions, velocities
    
# Render frame
env.render_frame("output.png", size=800)
```

---

## 🏗️ Project Structure

```
Polaris-Engine/
├── src/
│   ├── main.cpp              # Main simulation loop
│   ├── world.cpp/hpp         # World state & AI control
│   ├── agent.cpp/hpp         # Agent structure with brain
│   ├── neural_network.cpp/hpp # Feedforward neural network
│   ├── config.cpp/hpp        # Configuration with AI parameters
│   ├── statistics.cpp/hpp    # Evolution tracking
│   ├── imgui_panel.cpp/hpp   # UI with AI controls
│   └── ...
├── external/
│   ├── imgui*.cpp/h          # Dear ImGui v1.91.6
│   ├── json.hpp              # nlohmann/json v3.11.3
│   └── stb_image_write.h     # Image export
├── config.json               # AI + ecosystem parameters
├── AI_FEATURES.md            # Complete AI documentation
├── AI_SUMMARY.md             # Quick AI overview
├── BALANCE_GUIDE.md          # Ecosystem tuning reference
└── CMakeLists.txt            # Build with neural_network.cpp
```

---

## 🛠️ Advanced Features

### Custom Neural Network Architectures
Modify `neural_network.hpp` to experiment with:
- Different activation functions (ReLU, sigmoid)
- Recurrent connections (LSTM-like memory)
- Attention mechanisms

### Reinforcement Learning Integration
Train agents with explicit rewards:
- Energy gain → positive reward
- Death → negative reward
- Integrate with PyTorch/TensorFlow

### GPU Acceleration
For 10,000+ agents:
- CUDA kernels for neural network forward pass
- GPU-based spatial hashing
- Parallel evolution

---

## 📄 TODO / Roadmap

- [x] Full Dear ImGui integration
- [x] nlohmann/json parser
- [x] Agent trails visualization
- [x] Neural network AI with evolution
- [x] Ecosystem rebalancing
- [ ] Multi-species ecosystem (herbivores, plants)
- [ ] OpenAI Gym wrapper
- [ ] Save/load evolved brains
- [ ] Neural network visualization
- [ ] Genetic algorithm crossover
- [ ] Video export (MP4)

---

## 📄 License

MIT License - see [LICENSE](LICENSE) file.

---

## 🙏 Acknowledgments

- [SDL2](https://www.libsdl.org/) - Graphics rendering
- [Dear ImGui](https://github.com/ocornut/imgui) v1.91.6 - UI framework
- [pybind11](https://github.com/pybind/pybind11) - Python bindings
- [nlohmann/json](https://github.com/nlohmann/json) v3.11.3 - JSON parsing
- [stb_image_write](https://github.com/nothings/stb) - Image export

---
