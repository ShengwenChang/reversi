#!/usr/bin/env bash
# Start the backend server on Linux with the following command:
# `./run.sh` in the `backend/` folder

set -euo pipefail

# Change to the directory containing this script
cd "$(dirname "${BASH_SOURCE[0]}")"

# Install uv if not already available
if ! command -v uv &> /dev/null; then
    echo "Installing uv..."
    curl -LsSf https://astral.sh/uv/install.sh | sh
    export PATH="$HOME/.cargo/bin:$PATH"
else
    echo "uv is already installed"
fi

echo "Cleaning any leftover rs_mod artifacts"
rm -rf .venv

echo "Syncing environment (creates .venv if missing)"
uv sync

echo "Running cargo clean"
cargo clean

echo "Ensuring maturin is available"
uv pip install maturin

echo "Building Rust extension (release)"
uv run maturin develop --release

echo "DEBUG: Verify that next_move is present in rs_mod"
uv run python - <<'PY'
import pathlib, rs_mod
print("rs_mod loaded from", pathlib.Path(rs_mod.__file__))
print("top-level names:", [n for n in dir(rs_mod) if not n.startswith('_')])
print("inner names:", [n for n in dir(rs_mod.rs_mod) if not n.startswith('_')])
PY

echo "Starting FastAPI on port 3000"
uv run uvicorn app.main:app --reload --host 0.0.0.0 --port 3000 