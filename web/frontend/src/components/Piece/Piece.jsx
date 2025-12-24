import "./Piece.css";

function Piece({ color = "black", delay = 0 }) {
  return <div className={`piece ${color}`} style={{ "--d": `${delay}ms` }} />;
}

export default Piece;
