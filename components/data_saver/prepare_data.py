import json
import subprocess
from pathlib import Path

# === Load config files ===
def load_json(path):
    try:
        with open(path, 'r') as f:
            return json.load(f)
    except FileNotFoundError:
        raise FileNotFoundError(f"Config file not found: {path}")
    except json.JSONDecodeError as e:
        raise ValueError(f"Invalid JSON in {path}: {e}")

# Paths
API_CONFIG_PATH = Path('./config/api.json')
ASSET_CONFIG_PATH = Path('./config/asset.json')
OUTPUT_CONFIG_PATH = Path('./config/output_data_location.json')
RUST_EXECUTABLE_PATH = Path('./components/rust_data_fetcher/target/release/data_fetcher')

# Load configuration
API_KEY = load_json(API_CONFIG_PATH)['api']['key']
ASSETS = load_json(ASSET_CONFIG_PATH)['assets']
OUTPUT_DATAPATH = Path(load_json(OUTPUT_CONFIG_PATH)['output_path'])

SYMBOLS = [asset.get('symbol') for asset in ASSETS if 'symbol' in asset]

def main():
    # Ensure output directory exists
    OUTPUT_DATAPATH.mkdir(parents=True, exist_ok=True)

    for symbol in SYMBOLS:
        output_path = OUTPUT_DATAPATH / f"{symbol}.bin"
        print(f"--- Downloading data for {symbol} ---")

        command = [
            str(RUST_EXECUTABLE_PATH),
            "--symbol", symbol,
            "--api-key", API_KEY,
            "--output-path", str(output_path)
        ]

        try:
            subprocess.run(command, check=True)
        except subprocess.CalledProcessError as e:
            print(f"Error fetching data for {symbol}: {e}")

if __name__ == "__main__":
    main()
