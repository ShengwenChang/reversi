from pydantic import BaseModel
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

import rs_mod
import importlib
importlib.reload(rs_mod)

app = FastAPI()

# allow all origins
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

SIZE = 8

class NextMove(BaseModel):
    idx: int

class BoardState(BaseModel):
    board: list[str]
    current_player: str

@app.post("/next-move", response_model=NextMove)
async def get_next_move(board_state: BoardState) -> NextMove:
    (row, col) = rs_mod.next_move(board_state.board, board_state.current_player)
    idx = row * SIZE + col
    return NextMove(idx=idx)


