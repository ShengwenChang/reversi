import "./StartScreen.css";
import Piece from "../Piece/Piece";

function StartScreen({ onSelect }) {
  return (
    <div className="start-screen">
      <h1 className="title">Reversi</h1>
      <p className="subtitle">Made with ❤️ by Wesley</p>

      <button className="color-selector" onClick={() => onSelect("black")}>
        <Piece color="black" />
        <span>Play as Black</span>
      </button>

      <button className="color-selector" onClick={() => onSelect("white")}>
        <Piece color="white" />
        <span>Play as White</span>
      </button>
    </div>
  );
}

export default StartScreen;
