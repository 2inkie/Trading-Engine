use pyo3::prelude::*;

/// A dummy function that simulates preparing data
#[pyfunction]
fn fetch_and_clean(asset: &str, output_path: &str) -> PyResult<String> {
    println!("[Rust] Preparing data for asset: {} and saving to {}", asset, output_path);
    Ok(output_path.to_string())
}

/// A Python module implemented in Rust
#[pymodule]
fn rust_data_pipeline(m: &Bound<'_, PyModule>) -> PyResult<()> {
    m.add_function(wrap_pyfunction!(fetch_and_clean, m)?)?;
    Ok(())
}