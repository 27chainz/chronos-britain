# Chronos-Britain 🇬🇧

[![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://isocpp.org/)
[![Python Pipeline](https://img.shields.io/badge/Data%20Pipeline-Python%203-green.svg)](https://www.python.org/)
[![Data Source](https://img.shields.io/badge/Data%20Source-ONS%202021%20Census-orange.svg)](https://www.ons.gov.uk/)
[![License](https://img.shields.io/badge/License-MIT-purple.svg)](LICENSE)

This was one of my first attempts at programming and represents some of my earliest work with C++ and data-driven modeling, which is why you see the heavy AI usage here. C++ is not an easy language to learn ahahahah. This is the first of many. I hope that if you are a recruiter seeing this, you see a potential candidate, and not just someone who used AI. I hope you see the potential. I hope that you consider hiring me and giving me the opportunity to prove myself.

---

## 🚀 Key Highlights

- ⚡ **6.7M Agents in ~3 Seconds**: Built in C++ to simulate 1:10 of the UK population over 12 months in seconds.
- 📊 **Real UK Census Data**: Powered by live data scraped from the Office for National Statistics (ONS).
- 🕸️ **Social Network Contagion**: Uses Small-World graph networks to model how news and protests spread.
- 💷 **Realistic UK Taxes & Savings**: Tracks real UK tax bands (£12.5k allowance, 20%, 40%), inflation, and monthly savings.
- 🎛️ **Interactive Policy Console**: Test tax changes, inflation, energy caps, and welfare boosts live in your terminal.

---

## ⚙️ How It Works

1. **Scrape ONS Data**: Python script (`fetch_ons.py`) pulls Census statistics into a normalized schema.
2. **Load C++ Engine**: C++ engine initializes 6.7M agents in memory with realistic income, region, and age profiles.
3. **Run Policy Shock**: Enter custom tax rates, inflation, or subsidies to see monthly social unrest reactions.

---

## 💻 Quickstart

```bash
python3 fetch_ons.py               # 1. Fetch ONS Data
g++ -std=c++17 -O3 main.cpp -o sim # 2. Compile C++ Simulation
./sim                              # 3. Launch Policy Console
```

---

## 🎛️ Example Output

```text
Enter Tax Adjustment %: 2
Enter Inflation Rate %: 6
Enter Monthly Welfare Support (£/mo): 50
Enter Monthly Energy Cap Subsidy (£/mo): 25
Enter Simulation Timeline (Months): 12

Month | Total Protesters | % Pop  | London  | Rural
---------------------------------------------------
    1 |            44214 |   0.7% |   23302 |   9483
    6 |           213702 |   3.2% |   54831 |  63979
   12 |           353214 |   5.3% |   67725 | 103119

Timeline Complete! Computation time: 3.08s
```

---

## 📄 License
MIT License - see [LICENSE](LICENSE) for details.
