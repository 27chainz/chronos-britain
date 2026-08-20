import urllib.request
import json
import re

import ssl

url = "https://www.ons.gov.uk/search?topics=9731,6646,3845,9497,4262,4128,7755,4994,6885,9724,7367&filter=datasets&page=2"

ctx = ssl.create_default_context()
ctx.check_hostname = False
ctx.verify_mode = ssl.CERT_NONE

req = urllib.request.Request(url, headers={'User-Agent': 'Mozilla/5.0'})
try:
    with urllib.request.urlopen(req, context=ctx) as response:
        html = response.read().decode('utf-8')
        
    # Extract dataset titles and URLs
    pattern = r'href="(/datasets/[^"]+)"[^>]*>([^<]+)'
    matches = re.findall(pattern, html)
    
    datasets = []
    for link, title in matches:
        title_clean = title.strip().replace('\n', ' ')
        if title_clean and not any(d['title'] == title_clean for d in datasets):
            datasets.append({
                "title": title_clean,
                "url": f"https://www.ons.gov.uk{link}"
            })
            
    output_path = "data/ons_datasets.json"
    with open(output_path, "w") as f:
        json.dump(datasets, f, indent=2)
        
    print(f"Successfully scraped {len(datasets)} ONS datasets to {output_path}")

except Exception as e:
    print(f"Error fetching ONS datasets: {e}")
