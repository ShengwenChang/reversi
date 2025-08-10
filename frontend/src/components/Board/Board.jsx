import "./Board.css";
import { useState, useMemo, useEffect } from "react";
import Piece from "../Piece/Piece";
import PassToast from "../PassToast/PassToast";

// constants
const SIZE = 8;
const DIRS = [
  [-1, -1],
  [-1, 0],
  [-1, 1],
  [0, -1],
  [0, 1],
  [1, -1],
  [1, 0],
  [1, 1],
];

// delayS between pieces flipping in ms
const FLIP_DELAY = 500;

// basic helpers
const is_in_bound = (r, c) => r >= 0 && r < SIZE && c >= 0 && c < SIZE;
const get_idx_from_rowcol = (r, c) => r * SIZE + c;
const get_rowcol_from_idx = (idx) => [Math.floor(idx / SIZE), idx % SIZE];

/// returns the squares that would be flipped in one direction
function find_flips_in_dir(board, r, c, dr, dc, player) {
  const opponent = player === "black" ? "white" : "black";
  const flips = [];
  let row = r + dr;
  let col = c + dc;
  while (
    is_in_bound(row, col) &&
    board[get_idx_from_rowcol(row, col)] === opponent
  ) {
    flips.push(get_idx_from_rowcol(row, col));
    row += dr;
    col += dc;
  }
  if (
    is_in_bound(row, col) &&
    board[get_idx_from_rowcol(row, col)] === player &&
    flips.length
  ) {
    return flips;
  }
  return [];
}

/// find all legal moves for player → Map<index, [flipped indices]>
function get_legal_moves(board, player) {
  const moves = new Map();
  for (let idx = 0; idx < SIZE * SIZE; idx++) {
    if (board[idx] !== null) continue;

    const [r, c] = get_rowcol_from_idx(idx);
    let all_flips = [];
    for (const [dr, dc] of DIRS) {
      all_flips = all_flips.concat(
        find_flips_in_dir(board, r, c, dr, dc, player)
      );
    }
    if (all_flips.length) moves.set(idx, all_flips);
  }
  return moves;
}

/// yellow dot indicating a legal move
function HintDot() {
  return <div className="hint-dot" />;
}

function Board({ playerColor = "black", onGameEnd = () => {} }) {
  /// initializes the board
  const initialBoard = (() => {
    const arr = Array(SIZE * SIZE).fill(null);
    arr[get_idx_from_rowcol(3, 3)] = "white";
    arr[get_idx_from_rowcol(3, 4)] = "black";
    arr[get_idx_from_rowcol(4, 3)] = "black";
    arr[get_idx_from_rowcol(4, 4)] = "white";
    return arr;
  })();

  /// React states
  const [board, setBoard] = useState(initialBoard);
  const [currentPlayer, setCurrentPlayer] = useState("black");
  const [flipOrder, setFlipOrder] = useState(new Map());
  const [toastMessage, setToastMessage] = useState("");

  /// memoises legal moves for current player
  const legal_moves = useMemo(
    () => get_legal_moves(board, currentPlayer),
    [board, currentPlayer]
  );

  /// flips pieces and switches turns for a move
  const flip_for_move = (idx, flips, player) => {
    setFlipOrder(() => {
      const m = new Map();
      m.set(idx, 0); // new stone first
      flips.forEach((sq, i) => m.set(sq, i + 1));
      return m;
    });

    setBoard((prev) => {
      const next = [...prev];
      next[idx] = player;
      for (const f of flips) next[f] = player;

      setCurrentPlayer(player === "black" ? "white" : "black");
      return next;
    });
  };

  /// handles player turn
  const handleClick = (idx) => {
    // ignores illegal clicks
    if (currentPlayer !== playerColor || !legal_moves.has(idx)) return;

    // logs the move
    let col = idx % SIZE;
    let row = Math.floor(idx / SIZE);
    console.log(
      `Player of color ${currentPlayer} clicked on row: ${row}, col: ${col}`
    );

    // appies the move and switch turns
    const flips = legal_moves.get(idx);
    flip_for_move(idx, flips, currentPlayer);
  };

  /// passes the turn or ends game when no legal moves
  useEffect(() => {
    console.log(`Checking legal moves for ${currentPlayer} player`);
    if (legal_moves.size !== 0) return;

    // check if opponent has any moves
    const opponent = currentPlayer === "black" ? "white" : "black";
    const opponentMoves = get_legal_moves(board, opponent);
    if (opponentMoves.size === 0) {
      const blackCount = board.filter((c) => c === "black").length;
      const whiteCount = board.filter((c) => c === "white").length;
      console.log(
        `Game over. ${blackCount} black pieces, ${whiteCount} white pieces`
      );
      setTimeout(() => {
        onGameEnd({ black: blackCount, white: whiteCount });
      }, 1500);
      return;
    }

    // passes current player's turn
    setTimeout(() => {
      const msg = `No valid move for ${currentPlayer}. Passing turn.`;
      console.log(msg);
      setToastMessage(msg);
      setCurrentPlayer(opponent);
    }, 500);
  }, [legal_moves, currentPlayer, board]);

  /// handles backend turn
  useEffect(() => {
    // ignore human turns or positions where no moves are available
    if (currentPlayer === playerColor || legal_moves.size === 0) return;

    // requests a move from the backend
    const requestMove = async () => {
      try {
        const res = await fetch(
          `${import.meta.env.VITE_SERVER_URL}/next-move`,
          {
            method: "POST",
            headers: {
              "Content-Type": "application/json",
            },
            body: JSON.stringify({
              board: board.map((cell) => cell ?? ""),
              current_player: currentPlayer,
            }),
          }
        );

        if (!res.ok) {
          throw new Error(`Backend error: ${res.status} ${res.statusText}`);
        }

        const { idx: moveIdx } = await res.json();
        // validate the value we got back
        if (!Number.isInteger(moveIdx)) {
          throw new Error(`Illegal move received from backend: '${moveIdx}'`);
        } else if (moveIdx < 0 || moveIdx >= SIZE * SIZE) {
          console.log(
            `No move available from backend for ${currentPlayer} player`
          );
          return;
        }

        // logs the move
        let col = moveIdx % SIZE;
        let row = Math.floor(moveIdx / SIZE);
        console.log(
          `Backend of color ${currentPlayer} responses with row: ${row}, col: ${col}`
        );

        const flips = legal_moves.get(moveIdx);
        flip_for_move(moveIdx, flips, currentPlayer);
      } catch (err) {
        alert("Error requesting move from backend: " + err.message);
      }
    };

    const timerId = setTimeout(requestMove, 500);

    // cleanup to avoid duplicate timers in React StrictMode
    return () => clearTimeout(timerId);
  }, [currentPlayer, playerColor]);

  /// renders the board
  const squares = [];
  for (let row = 0; row < SIZE; row++) {
    for (let col = 0; col < SIZE; col++) {
      const index = get_idx_from_rowcol(row, col);
      const isDark = (row + col) % 2 === 1;
      const order = flipOrder.get(index) ?? 0;
      const delayMs = order * FLIP_DELAY;

      squares.push(
        <div
          key={`${row}-${col}`}
          className={`square ${isDark ? "dark" : "light"} ${
            board[index] ? "occupied" : ""
          }`}
          onClick={() => handleClick(index)}
        >
          {board[index] && <Piece color={board[index]} delay={delayMs} />}
          {!board[index] &&
            currentPlayer === playerColor &&
            legal_moves.has(index) && <HintDot />}
        </div>
      );
    }
  }

  return (
    <>
      {toastMessage && (
        <PassToast text={toastMessage} onDone={() => setToastMessage("")} />
      )}
      <div className="board">{squares}</div>
    </>
  );
}

export default Board;
