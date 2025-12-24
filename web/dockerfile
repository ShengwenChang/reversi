FROM ubuntu:24.04

# Install system dependencies
RUN apt-get update && apt-get install -y \
    curl \
    build-essential \
    pkg-config \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

# Install Rust
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
ENV PATH="/root/.cargo/bin:${PATH}"

# Install uv
RUN curl -LsSf https://astral.sh/uv/install.sh | sh
ENV PATH="/root/.local/bin:${PATH}"

# Set working directory  
WORKDIR /myapp

# Copy backend source
COPY backend/ ./

# Copy frontend build (you'll run npm run build before docker build)
COPY frontend/dist/ ./dist/

# Sync Python environment
RUN uv sync

# Install maturin and build Rust extension
RUN uv pip install maturin
RUN uv run maturin develop --release

# Run the service
EXPOSE 3000
CMD ["uv", "run", "uvicorn", "app.main:app", "--host", "0.0.0.0", "--port", "3000"]