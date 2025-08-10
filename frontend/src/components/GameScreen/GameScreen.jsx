import "./GameScreen.css";
import Board from "../Board/Board";
import { useState } from "react";
import EndScreen from "../EndScreen/EndScreen";

function GameScreen({ playerColor, onAbort }) {
  const [endResult, setEndResult] = useState(null);

  const handleRestart = () => {
    setEndResult(null);
    onAbort();
  };
  return (
    <div className="game-screen">
      <header className="game-header">
        <button className="back-btn" onClick={onAbort}>
          ← Back to Start
        </button>
        <h1 className="title">Reversi</h1>
        <p className="caption">You are playing as {playerColor}</p>
      </header>

      <div className="card">
        <Board
          playerColor={playerColor}
          onGameEnd={(res) => setEndResult(res)}
        />
      </div>

      {endResult && <EndScreen result={endResult} onRestart={handleRestart} />}
    </div>
  );
}

export default GameScreen;
