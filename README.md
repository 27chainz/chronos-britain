# Chronos-Britain: UK Socioeconomic Agent-Based Simulation 🇬🇧

[![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://isocpp.org/)
[![Python Pipeline](https://img.shields.io/badge/Data%20Pipeline-Python%203-green.svg)](https://www.python.org/)
[![Data Source](https://img.shields.io/badge/Data%20Source-ONS%202021%20Census-orange.svg)](https://www.ons.gov.uk/)
[![License](https://img.shields.io/badge/License-MIT-purple.svg)](LICENSE)


This was one of my first attempts at programming and represents some of my earliest work with C++ and data-driven modeling, which is why you se the heavy AI usage here. C++ is not an easy language to learn ahahahah. This is the first of many. I hope that if your a recruiter seeing this, you see a potential candidate, and not just someone who used AI. I hope you see the potential. I hope that you consider hiring me and giving me the opportunity to prove myself. 

---

## 🚀 Key Highlights

- ⚡ **6.7M Agents in ~3 Seconds**: Built in C++ to simulate 1:10 of the UK population over 12 months in seconds.
- 📊 **Real UK Census Data**: Powered by live data scraped from the Office for National Statistics (ONS).
- 🕸️ **Social Network Contagion**: Uses Small-World graph networks to model how news and protests spread.
- 💷 **Realistic UK Taxes & Savings**: Tracks real UK tax bands (£12.5k allowance, 20%, 40%), inflation, and monthly savings.
- 🎛️ **Interactive Policy Console**: Test tax changes, inflation, energy caps, and welfare boosts live in your terminal.

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
A high-performance, empirical C++ agent-based simulation modeling the social and economic dynamics of 6.7 million synthetic agents (1:10 scale of the UK population). The model evaluates how national fiscal shocks and policy interventions propagate through complex social networks over multi-month timelines.

---

## 📄 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
