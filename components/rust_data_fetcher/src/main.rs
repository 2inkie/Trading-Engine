// main.rs

use clap::Parser;
use serde::{de, Deserialize, Deserializer, Serialize};
use std::fs;
use bincode::{config, Decode, Encode};
use serde::de::{Visitor};

// --- Command-Line Argument Parsing ---
#[derive(Parser, Debug)]
#[command(version, about, long_about = None)]
struct Args {
    /// The stock symbol to fetch (eg AAPL)
    #[arg(short, long)]
    symbol: String,

    /// AlphaVantage API key
    #[arg(short, long)]
    api_key: String,

    /// The path to save the output .bin file
    #[arg(short, long)]
    output_path: String,
}

// --- Data Structures & Deserialization ---

fn deserialize_string_as_f64<'de, D>(deserializer: D) -> Result<f64, D::Error>
where
    D: Deserializer<'de>,
{
    struct StringToF64Visitor;

    impl<'de> Visitor<'de> for StringToF64Visitor {
        type Value = f64;
        fn expecting(&self, formatter: &mut std::fmt::Formatter) -> std::fmt::Result {
            formatter.write_str("a string that can be parsed as an f64")
        }
        fn visit_str<E>(self, v: &str) -> Result<Self::Value, E>
        where
            E: de::Error,
        {
            v.parse::<f64>().map_err(de::Error::custom)
        }
    }
    deserializer.deserialize_str(StringToF64Visitor)
}

#[derive(Serialize, Deserialize, Debug, Encode, Decode)]
pub struct PriceBar {
    #[serde(default)]
    pub date: String,
    #[serde(rename = "1. open", deserialize_with = "deserialize_string_as_f64")]
    pub open: f64,
    #[serde(rename = "2. high", deserialize_with = "deserialize_string_as_f64")]
    pub high: f64,
    #[serde(rename = "3. low", deserialize_with = "deserialize_string_as_f64")]
    pub low: f64,
    #[serde(rename = "4. close", deserialize_with = "deserialize_string_as_f64")]
    pub close: f64,
    #[serde(rename = "5. volume", deserialize_with = "deserialize_string_as_f64")]
    pub volume: f64,
}

#[derive(Deserialize, Debug)]
struct APIResponse {
    #[serde(rename = "Time Series (Daily)")]
    time_series: std::collections::HashMap<String, PriceBar>,
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    // 1. Parse arguments from the command line
    let args = Args::parse();

    println!("Fetching data for symbol: {}", args.symbol);

    // 2. Fetch data from the API
    let url = format!(
        "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol={}&apikey={}",
        args.symbol, args.api_key
    );

    let response_text = reqwest::get(&url).await?.text().await?;

    // 3. Parse and clean the data
    let parsed_response: APIResponse = serde_json::from_str(&response_text)?;
    let mut price_bars: Vec<PriceBar> = parsed_response
        .time_series
        .into_iter()
        .map(|(date, mut bar)| {
            bar.date = date; // Assign the date from the map key to the struct field
            bar
        })
        .collect();

    // Sort the bars by date to ensure they are in chronological order
    price_bars.sort_by(|a, b| a.date.cmp(&b.date));

    // 4. Save the data to a binary file
    let config = config::standard();
    let encoded: Vec<u8> = bincode::encode_to_vec(&price_bars, config)?;
    fs::write(&args.output_path, encoded)?;

    println!("Successfully saved data to: {}", args.output_path);

    Ok(())
}
