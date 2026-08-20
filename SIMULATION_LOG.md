# UK Simulation Project - Event & Feature Log

## Phase 8: Empirical ONS Ingestion & Interactive Policy Engine (Latest)
- **ONS Data Scraper (`fetch_ons.py`)**: Built an automated Python scraper to extract dataset listings and metadata directly from the Office for National Statistics (ONS) topic search page (`ons_datasets.json`).
- **Interactive Policy Shell**: Converted `main.cpp` into a policy engine where the user can input proposed Income Tax Rates and Inflation Rates to dynamically test macro-economic policies and analyze demographic reactions across London, rural areas, and the general UK population in real time.

## Phase 7: Dynamic "Cost of Living" Shocks
- **Inflation & Tax Modeling**: Replaced the flat 10% tax with a dual shock: an 8% inflation rate and a 5% wage tax.
- **Wealth-Based Impact**: Modeled how inflation disproportionately impacts renters compared to homeowners by escalating the cost of living penalty heavily for renters.

## Phase 6: The North/South Divide & Regionality
- **Regional Generation**: Assigned every agent to one of 7 regions based on UK population distribution (London, South, Midlands, North, Scotland, Wales, NI).
- **Regional Income Modifiers**: Applied income premiums to London (+30%) and the South (+10%), and penalties to the North, Wales, and NI (-10%).
- **Regional Approval Baselines**: Accounted for the stress of London living and historical political divides in the North/Scotland by adjusting baseline approval ratings.

## Phase 5: Age & Generational Divides
- **Age Distribution**: Sampled age using a normal distribution (mean 42, clamped between 18-90) to match the UK adult population.
- **Generational Stability**: Older demographics (65+) receive a starting stability buff to their approval, whereas younger demographics (under 30) receive a penalty.
- **Protest Thresholds**: Modeled the reality that younger demographics organize and protest at higher thresholds (0.35) than older citizens (0.30).
- **Homeownership (Wealth Proxy)**: Assigned homeownership based on age (70% for over 40s, 35% for under 40s). Homeowners gain stability and resistance to protesting.

## Phase 4: Demographics and Realism Updates
- **2021 Census Demographic Implementation**: Integrated real demographic distributions to accurately represent the UK population.
  - White (81.7%)
  - Asian / Asian British (9.3%)
  - Black / Black British (4.0%)
  - Mixed / Other (5.0%)
- **Demographic Income Variations**: Updated the lognormal income distributions to match median incomes by ethnicity (e.g., White ~£32k, Asian ~£34k, Black ~£25k).
- **Base Approval Calibration**: Tied starting government/policy approval ratings to demographic historical trends.
- **Rural Penalty**: Maintained the 15% rural representation and implemented a 20% rural income penalty modifier on top of demographic medians.

## Phase 3: Peer Network Graph
- **Ring Lattice Network Implementation**: Modeled social contagion and peer pressure. Agents check the protesting state of their 4 nearest physical/social neighbors (i-2, i-1, i+1, i+2).
- **Dynamic Memory Efficiency**: Utilized on-the-fly modulo-based neighbor checking instead of storing millions of integer adjacency lists, keeping simulation time under 2 seconds.
- **Social Contagion Logic**: If >= 50% (2 or more) neighbors are protesting, an agent's personal approval rating is dropped by a further 0.05.

## Phase 2: Income Distribution
- **Log-normal Income Distributions**: Replaced static salary values with continuous, log-normal distributed curves (`std::lognormal_distribution`) for much more realistic population modeling around median incomes.

## Phase 1: Core Foundation
- Generated 6.7 Million agents (1:10 scaling of the UK).
- Simulated a 10% flat policy tax shock on all incomes, reducing agent approval proportional to tax paid.
- Tally mechanism to identify protesting agents and tracking rural/urban sentiment differences.
