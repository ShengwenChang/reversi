import "./EndScreen.css";
import Piece from "../Piece/Piece";

/**
 * Modal shown when the game finishes.
 * @param {{result:{black:number,white:number}, onRestart:()=>void}} props
 */
function EndScreen({ result, onRestart }) {
  const { black, white } = result;
  const winner = black > white ? "black" : white > black ? "white" : "tie";

  const headline =
    winner === "tie"
      ? "It's a Tie"
      : `${winner.charAt(0).toUpperCase() + winner.slice(1)} Wins`;

  return (
    <div className="end-overlay">
      <div className="end-modal">
        <div className="piece-container">
          {winner === "tie" ? (
            <>
              <div className="piece-wrapper">
                <Piece color="black" />
              </div>
              <div className="piece-wrapper">
                <Piece color="white" />
              </div>
            </>
          ) : (
            <div className="piece-wrapper">
              <Piece color={winner} />
            </div>
          )}
        </div>

        <h2 className="headline">{headline}</h2>
        <p className="sub">Game Over</p>

        <button className="btn-restart" onClick={onRestart}>
          <span className="arrow">←</span>
          <span className="label">Return to Start</span>
        </button>
      </div>
    </div>
  );
}

export default EndScreen;
