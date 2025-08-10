use std::fmt;

mod board;
pub mod minimax;

// Board constants
pub const SIZE: usize = 8;
pub const BLACK: &str = "black";
pub const WHITE: &str = "white";
pub const DIRECTIONS: [(i32, i32); 8] = [
    (-1, -1),
    (1, 1),
    (1, -1),
    (-1, 1),
    (-1, 0),
    (1, 0),
    (0, -1),
    (0, 1),
];

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Colors {
    Black,
    White,
    Empty,
}

impl fmt::Display for Colors {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Colors::Black => write!(f, "B"),
            Colors::White => write!(f, "W"),
            Colors::Empty => write!(f, "U"),
        }
    }
}

// Minimax constants
pub const MAX_DEPTH: i32 = 5;
pub const COINPARITY_WEIGHT: i32 = 4;
pub const CORNEROCCUPENCY_WEIGHT: i32 = 11;
pub const MOBILITY_WEIGHT: i32 = 6;
pub const EDGEOCCUPENCY_WEIGHT: i32 = 5;
