use crate::components::{BLACK, Colors, DIRECTIONS, SIZE, WHITE};
use std::collections::HashMap;

#[derive(Clone)]
pub struct Board {
    // the board
    pub board: [[Colors; SIZE]; SIZE],

    // the backend player color
    pub player: Colors,
}

impl Board {
    pub fn new(board: Vec<String>, target_player: String) -> Self {
        let mut rs_board = [[Colors::Empty; SIZE]; SIZE];
        for i in 0..SIZE {
            for j in 0..SIZE {
                rs_board[i][j] = match board[i * SIZE + j].as_str() {
                    BLACK => Colors::Black,
                    WHITE => Colors::White,
                    _ => Colors::Empty,
                }
            }
        }
        let current_player = match target_player.as_str() {
            BLACK => Colors::Black,
            WHITE => Colors::White,
            _ => Colors::Empty,
        };

        Board {
            board: rs_board,
            player: current_player,
        }
    }

    #[allow(dead_code)]
    pub fn print_board(&self) {
        for i in 0..SIZE {
            for j in 0..SIZE {
                print!("{} ", self.board[i][j]);
            }
            println!();
        }
    }

    pub fn get_opponent(&self, player_color: Colors) -> Colors {
        match player_color {
            Colors::Black => Colors::White,
            Colors::White => Colors::Black,
            _ => {
                panic!("Invalid player");
            }
        }
    }

    pub fn in_bounds(&self, row: i32, col: i32) -> bool {
        row >= 0 && row < SIZE as i32 && col >= 0 && col < SIZE as i32
    }

    /// returns all flips for a move
    pub fn get_flips(&self, row: i32, col: i32, current_player: Colors) -> Vec<(i32, i32)> {
        let opponent = self.get_opponent(current_player);
        let mut flips: Vec<(i32, i32)> = Vec::new();
        for (dx, dy) in DIRECTIONS {
            let (mut cr, mut cc) = (row + dx, col + dy);
            let mut current_flips: Vec<(i32, i32)> = Vec::new();
            loop {
                if !self.in_bounds(cr, cc) {
                    current_flips.clear();
                    break;
                }
                match self.board[cr as usize][cc as usize] {
                    color if color == opponent => {
                        current_flips.push((cr, cc));
                        cr += dx;
                        cc += dy;
                    }
                    color if color == current_player => {
                        break;
                    }
                    _ => {
                        current_flips.clear();
                        break;
                    }
                }
            }
            for (cr, cc) in current_flips {
                flips.push((cr, cc));
            }
        }
        flips
    }

    /// returns all valid moves for a player
    pub fn get_valid_moves(&self, current_player: Colors) -> HashMap<(i32, i32), Vec<(i32, i32)>> {
        let mut valid_moves = HashMap::new();
        for i in 0..SIZE {
            for j in 0..SIZE {
                if self.board[i][j] == Colors::Empty {
                    let flips = self.get_flips(i as i32, j as i32, current_player);
                    if !flips.is_empty() {
                        valid_moves.insert((i as i32, j as i32), flips);
                    }
                }
            }
        }
        valid_moves
    }

    /// makes a move and flips the pieces
    pub fn make_move(
        &mut self,
        row: i32,
        col: i32,
        flips: Vec<(i32, i32)>,
        current_player: Colors,
    ) {
        self.board[row as usize][col as usize] = current_player;
        for (cr, cc) in flips {
            self.board[cr as usize][cc as usize] = current_player;
        }
    }
}
