use pyo3::prelude::*;
use serde::{Deserialize, Serialize};
use std::fs;
use bincode::{config, Decode, Encode};

#[derive(Serialize, Deserialize, Debug, Encode, Decode)]
pub struct PriceBar {
    #[serde(rename = "1. open")]
    pub open: String,
    #[serde(rename = "2. high")]
    pub high: String,
    #[serde(rename = "3. low")]
    pub low: String,
    #[serde(rename = "4. close")]
    pub close: String,
    #[serde(rename = "5. volume")]
    pub volume: String,
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
        let mut price_bars: Vec<PriceBar> = parsed_response.time_series.into_values().collect();

        let cleaned_bars: Vec<_> = price_bars
            .into_iter()
            .filter_map(|bar| {
                let close_price = bar.close.parse::<f64>().ok()?;
                if close_price > 0.0 {
                    Some(bar)
                } else {
                    None
                }
            })
            .collect();
        let config = config::standard();
        let encoded: Vec<u8> = bincode::encode_to_vec(&cleaned_bars, config)?;
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