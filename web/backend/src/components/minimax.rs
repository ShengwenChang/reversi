use std::sync::{Arc, RwLock};

use crate::components::board::*;
use crate::components::{
    COINPARITY_WEIGHT, CORNEROCCUPENCY_WEIGHT, Colors, EDGEOCCUPENCY_WEIGHT, MAX_DEPTH,
    MOBILITY_WEIGHT, SIZE,
};

pub fn minimax_next_move(board: Vec<String>, color: String) -> (i32, i32) {
    let board = Board::new(board, color);
    let best_move = Arc::new(RwLock::new((-1, -1)));
    let _ = alpha_beta_pruning(
        &board,
        f32::NEG_INFINITY,
        f32::INFINITY,
        board.player,
        MAX_DEPTH,
        best_move.clone(),
    );
    let best_move = best_move.read().unwrap();
    (best_move.0, best_move.1)
}

fn alpha_beta_pruning(
    board: &Board,
    mut alpha: f32,
    mut beta: f32,
    current_player: Colors,
    depth: i32,
    best_move: Arc<RwLock<(i32, i32)>>,
) -> f32 {
    let moves = board.get_valid_moves(current_player);
    if depth == 0 || moves.is_empty() {
        return board.evaluate(board.player) as f32;
    }

    if current_player == board.player {
        let mut value = f32::NEG_INFINITY;
        for ((r, c), flips) in moves {
            let mut child_board = board.clone();
            child_board.make_move(r, c, flips, current_player);
            let score = alpha_beta_pruning(
                &child_board,
                alpha,
                beta,
                board.get_opponent(current_player),
                depth - 1,
                best_move.clone(),
            );

            if score > value {
                value = score;
                if depth == MAX_DEPTH {
                    best_move.write().unwrap().0 = r;
                    best_move.write().unwrap().1 = c;
                }
            }

            alpha = alpha.max(value);

            if alpha >= beta {
                return value;
            }
        }
        value
    } else {
        let mut value = f32::INFINITY;
        for ((r, c), flips) in moves {
            let mut child_board = board.clone();
            child_board.make_move(r, c, flips, current_player);
            let score = alpha_beta_pruning(
                &child_board,
                alpha,
                beta,
                board.get_opponent(current_player),
                depth - 1,
                best_move.clone(),
            );

            if score < value {
                value = score;
                if depth == MAX_DEPTH {
                    best_move.write().unwrap().0 = r;
                    best_move.write().unwrap().1 = c;
                }
            }

            beta = beta.min(value);

            if alpha >= beta {
                return value;
            }
        }
        value
    }
}

impl Board {
    pub fn evaluate(&self, current_player: Colors) -> f32 {
        COINPARITY_WEIGHT as f32 * self.get_coin_parity(current_player) as f32
            + CORNEROCCUPENCY_WEIGHT as f32 * self.get_corner_occupancy(current_player) as f32
            + MOBILITY_WEIGHT as f32 * self.get_mobility(current_player) as f32
            + EDGEOCCUPENCY_WEIGHT as f32 * self.get_edge_occupancy(current_player) as f32
    }

    /// returns the net number of pieces of the current player on the board
    fn get_coin_parity(&self, current_player: Colors) -> i32 {
        let mut q = 0;
        for i in 0..SIZE {
            for j in 0..SIZE {
                if self.board[i][j] == current_player {
                    q += 1;
                } else if self.board[i][j] == self.get_opponent(current_player) {
                    q -= 1;
                }
            }
        }
        q
    }

    /// returns the 5 * net number of corner occupied by the current player
    fn get_corner_occupancy(&self, current_player: Colors) -> i32 {
        let mut q = 0;
        let corners = [(0, 0), (0, SIZE - 1), (SIZE - 1, 0), (SIZE - 1, SIZE - 1)];
        let opponent = self.get_opponent(current_player);
        for (r, c) in corners.iter() {
            match self.board[*r][*c] {
                color if color == current_player => q += 5,
                color if color == opponent => q -= 5,
                _ => {}
            }
        }
        q
    }

    /// returns the net number of valid moves for the current player
    fn get_mobility(&self, current_player: Colors) -> i32 {
        let agent_moves = self.get_valid_moves(current_player).len() as i32;
        let opponent_moves = self
            .get_valid_moves(self.get_opponent(current_player))
            .len() as i32;
        agent_moves - opponent_moves
    }

    /// returns the net number of edge pieces of the current player
    fn get_edge_occupancy(&self, current_player: Colors) -> i32 {
        let mut q = 0;
        let opponent = self.get_opponent(current_player);

        for j in 2..(SIZE - 2) {
            match self.board[0][j] {
                color if color == current_player => q += 1,
                color if color == opponent => q -= 1,
                _ => {}
            }
            match self.board[SIZE - 1][j] {
                color if color == current_player => q += 1,
                color if color == opponent => q -= 1,
                _ => {}
            }
        }

        for i in 2..(SIZE - 2) {
            match self.board[i][0] {
                color if color == current_player => q += 1,
                color if color == opponent => q -= 1,
                _ => {}
            }
            match self.board[i][SIZE - 1] {
                color if color == current_player => q += 1,
                color if color == opponent => q -= 1,
                _ => {}
            }
        }

        q
    }
}
