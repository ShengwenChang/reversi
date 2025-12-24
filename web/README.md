# Overview

## Local Deployment on Windows

To run the frontend:

```bash
cd frontend
npm run dev
```

To run the backend on windows:

```bash
cd backend
.\run.ps1
```

## Linux Server Deployment

Before proceeding, please ensure [docker](https://docs.docker.com/engine/install/ubuntu/), [rust](https://www.rust-lang.org/tools/install), [uv](https://docs.astral.sh/uv/getting-started/installation/) and [npm](https://docs.npmjs.com/downloading-and-installing-node-js-and-npm) are already installed.

**Step 1: Building the frontend.**

To start with, we will build the frontend as static files that could be automatically served by FastAPI. Run the following command:

```bash
cd frontend; npm run build
```

You should see a `dist/` folder appear in current directory.

**Step 2: Setting up the backend.**

Copy the following at the end of the `reversi/backend/app/main.py` file to enable FastAPI to serve the static files built from last step.

```python
# Serve static files
from fastapi.staticfiles import StaticFiles
app.mount(
    "/", StaticFiles(directory="dist", html=True), name="frontend"
)
```

Also, in the `frontend/.env` file, change the `VITE_SERVER_URL` into `""`.

**Step 3: Run the service.**

In a new bash terminal, run the following:

```bash
docker build -t reversi . ; docker run -p 3000:3000 reversi
```

Now, you will be able to visit your game website through `<your server ip>:3000/`
