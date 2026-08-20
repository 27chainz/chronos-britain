import urllib.request
import ssl
import json
import csv
import re
import os

print("Starting ONS Normalization Pipeline...")

url = "https://www.ons.gov.uk/search?topics=9731,6646,3845,9497,4262,4128,7755,4994,6885,9724,7367&filter=datasets&page=2"

ctx = ssl.create_default_context()
ctx.check_hostname = False
ctx.verify_mode = ssl.CERT_NONE

req = urllib.request.Request(url, headers={'User-Agent': 'Mozilla/5.0'})
datasets = []

try:
    with urllib.request.urlopen(req, context=ctx) as response:
        html = response.read().decode('utf-8')
        pattern = r'href="(/datasets/[^"]+)"[^>]*>([^<]+)'
        matches = re.findall(pattern, html)
        for link, title in matches:
            t_clean = title.strip().replace('\n', ' ')
            if t_clean and not any(d['title'] == t_clean for d in datasets):
                datasets.append({"title": t_clean, "url": f"https://www.ons.gov.uk{link}"})
except Exception as e:
    print(f"Warning fetching ONS index: {e}")

os.makedirs("data", exist_ok=True)

# Save datasets index
with open("data/ons_datasets.json", "w") as f:
    json.dump(datasets, f, indent=2)

# Generate Normalized Schema CSV for C++ DataEngine
schema_rows = [
    ["metric", "category", "weight", "modifier"],
    # Ethnicity breakdown & median incomes (from Census/ONS HBAI)
    ["ethnicity", "White", "0.817", "32000"],
    ["ethnicity", "Asian", "0.093", "34000"],
    ["ethnicity", "Black", "0.040", "25000"],
    ["ethnicity", "MixedOther", "0.050", "28000"],
    # Regional breakdown & wage multiplier
    ["region", "London", "0.13", "1.30"],
    ["region", "South", "0.23", "1.10"],
    ["region", "Midlands", "0.16", "1.00"],
    ["region", "North", "0.24", "0.90"],
    ["region", "Scotland", "0.08", "0.95"],
    ["region", "Wales", "0.05", "0.90"],
    ["region", "NI", "0.03", "0.90"],
    # Housing ratio by age
    ["homeownership", "under_40", "0.35", "0.0"],
    ["homeownership", "over_40", "0.70", "0.0"]
]

csv_path = "data/uk_agent_schema.csv"
with open(csv_path, "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerows(schema_rows)

print(f"Successfully generated normalized agent schema: {csv_path}")
