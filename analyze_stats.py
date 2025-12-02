#!/usr/bin/env python3
"""
Analyze Polaris simulation statistics
"""
import pandas as pd
import matplotlib.pyplot as plt
import sys

def plot_population(csv_file='stats.csv'):
    """Plot population dynamics over time"""
    try:
        df = pd.read_csv(csv_file)
    except FileNotFoundError:
        print(f"Error: {csv_file} not found. Run the simulation first!")
        return

    fig, axes = plt.subplots(2, 2, figsize=(14, 10))
    
    # Population over time
    ax = axes[0, 0]
    ax.plot(df['step'], df['total_agents'], label='Total', linewidth=2, color='black')
    ax.plot(df['step'], df['predators'], label='Predators', linewidth=2, color='red', alpha=0.7)
    ax.plot(df['step'], df['prey'], label='Prey', linewidth=2, color='blue', alpha=0.7)
    ax.set_xlabel('Simulation Step')
    ax.set_ylabel('Population Count')
    ax.set_title('Population Dynamics')
    ax.legend()
    ax.grid(True, alpha=0.3)
    
    # Energy over time
    ax = axes[0, 1]
    ax.plot(df['step'], df['avg_energy'], label='Average', linewidth=2, color='green')
    ax.plot(df['step'], df['avg_predator_energy'], label='Predators', linewidth=2, color='red', alpha=0.7)
    ax.plot(df['step'], df['avg_prey_energy'], label='Prey', linewidth=2, color='blue', alpha=0.7)
    ax.set_xlabel('Simulation Step')
    ax.set_ylabel('Average Energy')
    ax.set_title('Energy Levels')
    ax.legend()
    ax.grid(True, alpha=0.3)
    
    # Births and deaths
    ax = axes[1, 0]
    ax.plot(df['step'], df['births'].cumsum(), label='Cumulative Births', linewidth=2, color='green')
    ax.plot(df['step'], df['deaths'].cumsum(), label='Cumulative Deaths', linewidth=2, color='red')
    ax.set_xlabel('Simulation Step')
    ax.set_ylabel('Count')
    ax.set_title('Births and Deaths (Cumulative)')
    ax.legend()
    ax.grid(True, alpha=0.3)
    
    # Predator-prey ratio
    ax = axes[1, 1]
    ratio = df['predators'] / (df['prey'] + 1)  # Avoid division by zero
    ax.plot(df['step'], ratio, linewidth=2, color='purple')
    ax.axhline(y=df['predators'][0] / (df['prey'][0] + 1), 
               color='gray', linestyle='--', alpha=0.5, label='Initial Ratio')
    ax.set_xlabel('Simulation Step')
    ax.set_ylabel('Predator/Prey Ratio')
    ax.set_title('Predator-Prey Ratio Over Time')
    ax.legend()
    ax.grid(True, alpha=0.3)
    
    plt.tight_layout()
    
    # Save figure
    output_file = csv_file.replace('.csv', '_analysis.png')
    plt.savefig(output_file, dpi=150)
    print(f"✅ Analysis saved to {output_file}")
    
    plt.show()

def print_summary(csv_file='stats.csv'):
    """Print statistical summary"""
    try:
        df = pd.read_csv(csv_file)
    except FileNotFoundError:
        print(f"Error: {csv_file} not found. Run the simulation first!")
        return
    
    print("\n" + "="*60)
    print("POLARIS SIMULATION SUMMARY")
    print("="*60)
    
    print(f"\n📊 Simulation Duration: {len(df)} steps ({df['time'].max():.1f}s)")
    
    print(f"\n👥 Population:")
    print(f"   Initial: {df['total_agents'].iloc[0]} ({df['predators'].iloc[0]} predators, {df['prey'].iloc[0]} prey)")
    print(f"   Final:   {df['total_agents'].iloc[-1]} ({df['predators'].iloc[-1]} predators, {df['prey'].iloc[-1]} prey)")
    print(f"   Peak:    {df['total_agents'].max()} at step {df['total_agents'].idxmax()}")
    print(f"   Min:     {df['total_agents'].min()} at step {df['total_agents'].idxmin()}")
    
    print(f"\n🔄 Lifecycle:")
    print(f"   Total Births: {df['births'].sum()}")
    print(f"   Total Deaths: {df['deaths'].sum()}")
    print(f"   Net Change:   {int(df['births'].sum() - df['deaths'].sum())}")
    
    print(f"\n⚡ Energy:")
    print(f"   Avg (final):      {df['avg_energy'].iloc[-1]:.2f}")
    print(f"   Avg Predator:     {df['avg_predator_energy'].mean():.2f}")
    print(f"   Avg Prey:         {df['avg_prey_energy'].mean():.2f}")
    
    # Extinction check
    if df['total_agents'].iloc[-1] == 0:
        print(f"\n💀 EXTINCTION occurred at step {df[df['total_agents'] == 0].index[0]}")
    elif df['predators'].iloc[-1] == 0:
        print(f"\n🦊 Predators went extinct at step {df[df['predators'] == 0].index[0] if 0 in df['predators'].values else 'N/A'}")
    elif df['prey'].iloc[-1] == 0:
        print(f"\n🐰 Prey went extinct at step {df[df['prey'] == 0].index[0] if 0 in df['prey'].values else 'N/A'}")
    else:
        print(f"\n✅ Ecosystem stable - both populations survived!")
    
    print("\n" + "="*60 + "\n")

if __name__ == '__main__':
    csv_file = sys.argv[1] if len(sys.argv) > 1 else 'stats.csv'
    
    print_summary(csv_file)
    
    try:
        plot_population(csv_file)
    except ImportError:
        print("⚠️  matplotlib not installed. Install with: pip install matplotlib pandas")
