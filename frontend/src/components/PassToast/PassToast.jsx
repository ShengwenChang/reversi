import { useEffect } from "react";
import "./PassToast.css";

/**
 * Lightweight toast that vanishes automatically after 2 seconds.
 * @param {{text:string,onDone:()=>void}} props
 */
function PassToast({ text, onDone }) {
  useEffect(() => {
    const t = setTimeout(onDone, 1500);
    return () => clearTimeout(t);
  }, [onDone]);

  return <div className="pass-toast">{text}</div>;
}

export default PassToast;
