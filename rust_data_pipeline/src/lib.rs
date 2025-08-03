use pyo3::prelude::*;
use serde::{de, Deserialize, Serialize, de::Visitor, de::Deserializer};
use std::fs;
use bincode::{config, Decode, Encode};

fn deserialize_string_as_f64<'de, D>(deserializer: D) -> Result<f64, D::Error>
where
    D: Deserializer<'de>,
{
    // A little helper struct to tell Serde how to visit the strings coming from AlphaVantage
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

/// A function to fetch from teh API, parse the raw JSON, clean the data, and save to bin
#[pyfunction]
fn fetch_and_clean(symbol: &str, api_key: &str, output_path: &str) -> PyResult<()> {
    let rt = tokio::runtime::Builder::new_current_thread().enable_all().build()?;

    rt.block_on(async {
        let url = format!(
            "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol={}&apikey={}",
            symbol, api_key
        );

        let response_text = reqwest::get(&url).await?.text().await?;

        let parsed_response: APIResponse = serde_json::from_str(&response_text)?;
        let price_bars: Vec<PriceBar> = parsed_response.time_series.into_values().collect();
        let config = config::standard();
        let encoded: Vec<u8> = bincode::encode_to_vec(&price_bars, config)?;
        fs::write(output_path, encoded)?;

        Ok(()) // Return success
    }).map_err(|e: Box<dyn std::error::Error + Send + Sync>| PyErr::new::<pyo3::exceptions::PyRuntimeError, _>(e.to_string()))?;

    Ok(())
}

/// A Python module implemented in Rust
#[pymodule]
fn rust_data_pipeline(_py: Python, m: &Bound<'_, PyModule>) -> PyResult<()> {
    m.add_function(wrap_pyfunction!(fetch_and_clean, m)?)?;
    Ok(())
}