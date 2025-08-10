# Start the backend server on Windows with the following command:
# `.\run.ps1` in the `backend\` folder

$ErrorActionPreference = 'Stop'

Set-Location $PSScriptRoot

Write-Host "Cleaning any leftover rs_mod artifacts"
Remove-Item -Recurse -Force .venv

Write-Host "Syncing environment (creates .venv if missing)"
uv sync

Write-Host "Running cargo clean"
cargo clean

Write-Host "Ensuring maturin is available"
uv pip install maturin

Write-Host "Building Rust extension (release)"
uv run maturin develop --release

Write-Host "DEBUG: Verify that next_move is present in rs_mod"
uv run python -c "import rs_mod, pathlib, sys; print('rs_mod loaded from', pathlib.Path(rs_mod.__file__)); print('top-level names:', [n for n in dir(rs_mod) if not n.startswith('_')]); print('inner names:', [n for n in dir(rs_mod.rs_mod) if not n.startswith('_')])"

Write-Host "Starting FastAPI on port 3000"
uv run uvicorn app.main:app --reload --host 0.0.0.0 --port 3000