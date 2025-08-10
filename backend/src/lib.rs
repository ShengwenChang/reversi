mod components;
use components::minimax::minimax_next_move;

use pyo3::prelude::*;

#[pymodule]
fn rs_mod(m: &Bound<'_, PyModule>) -> PyResult<()> {
    m.add_function(wrap_pyfunction!(next_move, m)?)?;
    Ok(())
}

#[pyfunction]
fn next_move(board: Vec<String>, color: String) -> PyResult<(i32, i32)> {
    let (r, c) = minimax_next_move(board, color);
    Ok((r, c))
}
