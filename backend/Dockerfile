# syntax=docker/dockerfile:1.7
FROM python:3.11-slim AS builder

ENV DEBIAN_FRONTEND=noninteractive \
    PYTHONDONTWRITEBYTECODE=1 \
    PYTHONUNBUFFERED=1 \
    PATH="/root/.local/bin:/root/.cargo/bin:${PATH}"

# System deps for building Rust + Python extensions
RUN apt-get update && apt-get install -y --no-install-recommends \
    curl ca-certificates build-essential pkg-config libssl-dev git \
    && rm -rf /var/lib/apt/lists/*

# Install Rust toolchain
RUN curl https://sh.rustup.rs -sSf | sh -s -- -y --profile minimal && \
    rustup --version && rustc --version && cargo --version

# Install uv (package/deps manager) and maturin
RUN curl -LsSf https://astral.sh/uv/install.sh | sh && \
    uv --version

WORKDIR /app/backend

# Copy backend sources first to leverage Docker layer caching
# (adjust if your pyproject/Cargo.toml live elsewhere)
COPY backend/ /app/backend/

# Sync Python deps (will create .venv)
RUN uv sync

# Ensure maturin is available in the environment
RUN uv pip install maturin && uv run maturin --version

# Build wheel for the Rust extension (release)
# This produces a wheel under /app/backend/target/wheels; copy to /dist
RUN mkdir -p /dist && \
    uv run maturin build --release -o /dist && \
    ls -l /dist

FROM python:3.11-slim AS runtime

ENV DEBIAN_FRONTEND=noninteractive \
    PYTHONDONTWRITEBYTECODE=1 \
    PYTHONUNBUFFERED=1 \
    PATH="/root/.local/bin:/root/.cargo/bin:${PATH}"

# System runtime deps (no compilers)
RUN apt-get update && apt-get install -y --no-install-recommends \
    curl ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Install uv in runtime
RUN curl -LsSf https://astral.sh/uv/install.sh | sh && \
    uv --version

WORKDIR /app/backend

# Copy application code (FastAPI app and Python project files)
COPY backend/ /app/backend/

# Bring in the built wheels from builder
COPY --from=builder /dist /tmp/dist

# Install Python deps (no dev deps by default) and the Rust wheel
RUN uv sync && \
    uv pip install /tmp/dist/*.whl && \
    uv pip cache purge || true

EXPOSE 3000

# Start FastAPI with uvicorn on port 3000
CMD ["uv", "run", "uvicorn", "app.main:app", "--host", "0.0.0.0", "--port", "3000"]
