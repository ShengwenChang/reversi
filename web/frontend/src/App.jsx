import { useState } from "react";
import StartScreen from "./components/StartScreen/StartScreen";
import GameScreen from "./components/GameScreen/GameScreen";

function App() {
  const [playerColor, setPlayerColor] = useState(null); // null | "black" | "white"

  return (
    <div id="app">
      {playerColor === null ? (
        <StartScreen onSelect={setPlayerColor} />
      ) : (
        <GameScreen
          playerColor={playerColor}
          onAbort={() => setPlayerColor(null)}
        />
      )}
    </div>
  );
}

export default App;
