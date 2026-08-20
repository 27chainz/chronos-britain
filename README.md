# Chronos-Britain: UK Socioeconomic Agent-Based Simulation 🇬🇧

[![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://isocpp.org/)
[![Python Pipeline](https://img.shields.io/badge/Data%20Pipeline-Python%203-green.svg)](https://www.python.org/)
[![Data Source](https://img.shields.io/badge/Data%20Source-ONS%202021%20Census-orange.svg)](https://www.ons.gov.uk/)
[![License](https://img.shields.io/badge/License-MIT-purple.svg)](LICENSE)

A high-performance, empirical C++ agent-based simulation modeling the social and economic dynamics of 6.7 million synthetic agents (1:10 scale of the UK population). The model evaluates how national fiscal shocks and policy interventions propagate through complex social networks over multi-month timelines.

---

## 🚀 Key Highlights

- ⚡ **Massive Scale & Sub-3 Second Performance**: Simulates **6.7 million agents** over 12 monthly time steps in **~3 seconds** using C++ memory reservation and vectorized structures.
- 📊 **Empirical ONS Census Integration**: Driven by data scraped from the Office for National Statistics (ONS), covering demographics, 7 UK regions, income distributions, and homeownership ratios.
- 🕸️ **Watts-Strogatz Small-World Topology**: Models social contagion using local lattice neighborhoods ($k=4$) combined with random long-range shortcut edges to simulate viral news propagation.
- 💷 **Realistic UK Fiscal Micro-Simulation**: Implements UK progressive income tax brackets (£12.5k personal allowance, 20% basic, 40% higher rate) and inelastic living cost inflation.
- 🎛️ **Interactive Policy Console**: Test custom macroeconomic shocks (Tax Adjustments, Inflation) alongside relief policies (Universal Credit top-ups, Energy Bill Caps) in real-time.

---

## 📐 System Architecture

```
 ┌──────────────────────────────────────────────┐
 │ 1. ONS Data Pipeline (`fetch_ons.py`)        │
 │  Scrapes ONS Census & ASHE Earnings Tables    │
 └──────────────────────┬───────────────────────┘
                        │ Generates `data/uk_agent_schema.csv`
                        ▼
 ┌──────────────────────────────────────────────┐
 │ 2. Dynamic C++ Data Engine (`DataEngine.hpp`) │
 │  Loads demographic & income matrices in RAM  │
 └──────────────────────┬───────────────────────┘
                        │ Initializes 6.7M Synthetic Agents
                        ▼
 ┌──────────────────────────────────────────────┐
 │ 3. Small-World Network (`main.cpp`)          │
 │  Watts-Strogatz Graph (Lattice + Shortcuts)  │
 └──────────────────────┬───────────────────────┘
                        │ Runs Policy REPL Shell
                        ▼
 ┌──────────────────────────────────────────────┐
 │ 4. Multi-Month Cashflow & Contagion Engine   │
 │  Calculates savings depletion & protest state│
 └──────────────────────────────────────────────┘
```

---

## 💻 Quickstart

### Prerequisites
- C++17 compliant compiler (`g++` or `clang++`)
- Python 3.x

### Build and Run

```bash
# 1. Clone the repository
git clone https://github.com/27chainz/chronos-britain.git
cd chronos-britain

# 2. Run the ONS Data Normalization Pipeline
python3 fetch_ons.py

# 3. Compile the C++ Engine with O3 Optimization
g++ -std=c++17 -O3 main.cpp -o sim

# 4. Launch the Interactive Policy Console
./sim
```

---

## 🎛️ Example Interactive Session

```text
Loaded normalized schema from data/uk_agent_schema.csv successfully.
Allocating memory and generating Watts-Strogatz Small-World network for 6700000 UK citizens...
Population & Small-World Graph loaded in 2.35 seconds.

=========================================================
  UK SOCIOECONOMIC SIMULATION - ADVANCED POLICY ENGINE
=========================================================

Enter Tax Adjustment % (+/- e.g. 0 for baseline, 5 for +5%): 2
Enter Inflation Rate % (e.g., 8 for 8%): 6
Enter Monthly Universal Credit / Welfare Support (£/mo): 50
Enter Monthly Energy Bill Cap Subsidy (£/mo): 25
Enter Simulation Timeline (Months): 12

Simulating 12-month timeline under proposed policy...
Month | Total Protesters | % Pop  | London  | Rural
---------------------------------------------------
    1 |            44214 |   0.7% |   23302 |   9483
    6 |           213702 |   3.2% |   54831 |  63979
   12 |           353214 |   5.3% |   67725 | 103119

Timeline Complete! Computation time: 3.08s
```

---

## 📄 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
