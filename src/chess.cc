#include <Arduboy.h>
#include "lib/ArduboyExtra/simple_buttons.h"
#include "chess.hh"
#include "util.hh"

Arduboy arduboy;
SimpleButtons buttons (arduboy);

bool show_logo = true;

typedef enum {
    NONE    = 0,
    PAWN    = 1,
    ROOK    = 2,
    KNIGHT  = 3,
    BISHOP  = 4,
    QUEEN   = 5,
    KING    = 6,
} rank_t;

static const uint8_t PROGMEM RANK_SPRITES[7][8] = {
    /* none */      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    /* pawn */      { 0x00, 0x40, 0x60, 0x7e, 0x7e, 0x60, 0x40, 0x00 },
    /* rook */      { 0x00, 0x66, 0x7c, 0x7e, 0x7e, 0x7c, 0x66, 0x00 },
    /* knight */    { 0x00, 0x1a, 0x1c, 0x4e, 0x7c, 0x7c, 0x78, 0x00 },
    /* bishop */    { 0x00, 0x40, 0x4c, 0x7e, 0x7e, 0x4c, 0x40, 0x00 },
    /* queen */     { 0x00, 0x5e, 0x70, 0x7e, 0x7e, 0x70, 0x5e, 0x00 },
    /* king */      { 0x00, 0x18, 0x18, 0x7e, 0x7e, 0x18, 0x18, 0x00 },
};

static const uint8_t PROGMEM LOGO[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0x3f, 0x1f, 0x1f, 0x9f, 0x8f, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0x8f, 0x0f, 0x0f, 0x1f, 0x1f, 0x9f, 0xe3, 0xe3, 0xe3, 0x03, 0x01, 0x01, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x0f, 0x03, 0x01, 0x00, 0xc0, 0xf8, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xf0, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x7f, 0x3f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x3f, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0x3f, 0x3f, 0x1f, 0x9f, 0x9f, 0x9f, 0x1f, 0x1f, 0x1f, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0x3f, 0x1f, 0x9f, 0x9f, 0x9f, 0x9f, 0x1f, 0x1f, 0x1f, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0x3f, 0x1f, 0x9f, 0x9f, 0x9f, 0x9f, 0x1f, 0x1f, 0x1f, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xfc, 0xfc, 0xfe, 0xfe, 0xfe, 0xfe, 0xfc, 0xf8, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0x01, 0x00, 0x00, 0x00, 0x9c, 0x9f, 0x9f, 0x9f, 0x9f, 0x9f, 0x9f, 0x9c, 0x80, 0x80, 0x80, 0x80, 0x81, 0xff, 0xff, 0xff, 0xc0, 0x80, 0x00, 0x00, 0x07, 0x0f, 0x0f, 0x0f, 0x1f, 0x1f, 0x1e, 0x18, 0x38, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xc0, 0x80, 0x00, 0x00, 0x07, 0x0f, 0x0f, 0x0f, 0x1f, 0x1f, 0x1e, 0x18, 0x38, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xf8, 0xe0, 0xe0, 0xc0, 0x80, 0x81, 0x03, 0x07, 0x0f, 0x1f, 0x1f, 0x1f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x1f, 0x1f, 0x0f, 0x8f, 0xcf, 0x7f, 0x7f, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x7f, 0x7f, 0xff, 0xff, 0xf8, 0xe0, 0xc0, 0x80, 0x00, 0x03, 0x0f, 0x1f, 0x3f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x3f, 0x1f, 0x1f, 0x3f, 0xff, 0xff, 0xff, 0x03, 0x03, 0x1f, 0x3f, 0x7f, 0x7e, 0x7e, 0x7e, 0x7e, 0x3c, 0x18, 0x00, 0x80, 0xc0, 0xe0, 0xff, 0xff, 0xff, 0x03, 0x03, 0x1f, 0x3f, 0x7f, 0x7e, 0x7e, 0x7e, 0x7e, 0x3c, 0x18, 0x00, 0x80, 0xc0, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, };

static void __attribute__((noreturn)) panic(const char* msg){
    arduboy.clear();
    arduboy.print("PANIC\n");
    arduboy.print(msg);
    arduboy.display();

    for (;;) ;
}

// arduboy headers define WHITE and BLACK, so just use their definition to prevent cooked bugs
typedef uint8_t color_t;

class Coords {
    uint8_t packed;

public:
    Coords() : packed(0xffu) {}
    Coords(int x, int y) {
        if (x < 0 || x > 7 || y < 0 || y > 7) {
            panic("coords out of bounds");
        }

        packed = ((uint8_t)x & 7) | (((uint8_t)y & 7) << 3);
    }

    operator bool() const {
        return packed != 0xffu;
    }

    bool operator ==(const Coords& other) {
        return packed == other.packed;
    }

    bool operator !=(const Coords& other) {
        return packed != other.packed;
    }

    int x() const {
        return (int)(packed & 7);
    }

    int y() const {
        return (int)((packed >> 3) & 7);
    }

    Coords translate(int x_delta, int y_delta) const {
        int x_ = x() + x_delta;
        int y_ = y() + y_delta;

        if (x_ < 0 || x_ > 7 || y_ < 0 || y_ > 7) {
            // out of bounds, return null coord
            return Coords();
        } else {
            return Coords(x_, y_);
        }
    }
};

class Piece {
    uint8_t packed;
public:
    Piece() : packed(0) {}

    Piece(color_t color, rank_t rank)
        : Piece(color, rank, false)
    {}

    Piece(color_t color, rank_t rank, bool moved)
        : packed(((color & 1) << 7) | (moved << 6)| (rank & 7))
    {}

    operator bool() const {
        return packed != 0;
    }

    color_t color() const {
        return (color_t)(packed >> 7);
    }

    rank_t rank() const {
        return (rank_t)(packed & 7);
    }

    bool has_moved() const {
        return (bool)((packed >> 6) & 1);
    }

    Piece move() const {
        return Piece(color(), rank(), true);
    }
};

class Board {
    Piece squares[8][8];

public:
    Board()
        : squares({
            { Piece(BLACK, ROOK), Piece(BLACK, KNIGHT), Piece(BLACK, BISHOP), Piece(BLACK, QUEEN), Piece(BLACK, KING), Piece(BLACK, BISHOP), Piece(BLACK, KNIGHT), Piece(BLACK, ROOK) },
            { Piece(BLACK, PAWN), Piece(BLACK, PAWN), Piece(BLACK, PAWN), Piece(BLACK, PAWN), Piece(BLACK, PAWN), Piece(BLACK, PAWN), Piece(BLACK, PAWN), Piece(BLACK, PAWN) },
            { Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece() },
            { Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece() },
            { Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece() },
            { Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece() },
            { Piece(WHITE, PAWN), Piece(WHITE, PAWN), Piece(WHITE, PAWN), Piece(WHITE, PAWN), Piece(WHITE, PAWN), Piece(WHITE, PAWN), Piece(WHITE, PAWN), Piece(WHITE, PAWN) },
            { Piece(WHITE, ROOK), Piece(WHITE, KNIGHT), Piece(WHITE, BISHOP), Piece(WHITE, QUEEN), Piece(WHITE, KING), Piece(WHITE, BISHOP), Piece(WHITE, KNIGHT), Piece(WHITE, ROOK) },
        })
    {}

    Piece square(Coords coords) const {
        return squares[coords.y()][coords.x()];
    }

    Board move(Coords from, Coords to) const {
        Piece moving = square(from);

        // special castling logic
        if (moving.rank() == KING && !moving.has_moved() && abs(to.x() - from.x()) == 2) {
            // all validations are assumed handled, so we can get straight into the guts of the move:

            int move_direction = signum(to.x() - from.x());

            if (move_direction > 0) {
                // kingside
                Board new_ = *this;
                new_.squares[from.y()][from.x() + 1] = new_.squares[from.y()][from.x() + 3].move();
                new_.squares[from.y()][from.x() + 2] = new_.squares[from.y()][from.x()].move();
                new_.squares[from.y()][from.x() + 3] = Piece();
                new_.squares[from.y()][from.x()] = Piece();
                return new_;
            } else {
                // queenside
                Board new_ = *this;
                new_.squares[from.y()][from.x() - 1] = new_.squares[from.y()][from.x() - 4].move();
                new_.squares[from.y()][from.x() - 2] = new_.squares[from.y()][from.x()].move();
                new_.squares[from.y()][from.x() - 4] = Piece();
                new_.squares[from.y()][from.x()] = Piece();
                return new_;
            }
        }

        Board new_ = *this;
        new_.squares[to.y()][to.x()] = moving.move();
        new_.squares[from.y()][from.x()] = Piece();
        return new_;
    }

    bool is_valid_turn(Coords moving_coords, Coords target_coords, color_t player) const {
        if (!is_valid_move(moving_coords, target_coords)) {
            return false;
        }

        if (move(moving_coords, target_coords).is_check(player)) {
            return false;
        }

        return true;
    }

    bool is_under_attack(color_t player_under_attack, Coords target) const {
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                Coords coords = Coords(x, y);
                Piece piece = square(coords);

                if (piece && piece.color() != player_under_attack) {
                    if (is_valid_move(coords, target)) {
                        // valid move to this square means it's under attack
                        return true;
                    }
                }
            }
        }

        return false;
    }

    bool is_check(color_t player_in_check) const {
        Coords king = find_king(player_in_check);
        return is_under_attack(player_in_check, king);
    }

    // assumes that we're already in check:
    bool is_checkmate(color_t player_in_check) const {
        for (int py = 0; py < 8; py++) {
            for (int px = 0; px < 8; px++) {
                Coords piece_coords = Coords(px, py);
                Piece piece = square(piece_coords);

                if (!piece || piece.color() != player_in_check) {
                    // not our piece, not eligible for moving
                    continue;
                }

                for (int ty = 0; ty < 8; ty++) {
                    for (int tx = 0; tx < 8; tx++) {
                        Coords target = Coords(tx, ty);

                        if (is_valid_turn(piece_coords, target, player_in_check)) {
                            // if there is at least one way out of our pickle, it's not checkmate
                            return false;
                        }
                    }
                }
            }
        }

        return true;
    }

private:
    bool is_valid_move(Coords moving_coords, Coords target_coords) const {
        if (moving_coords == target_coords) {
            return false;
        }

        Piece moving = square(moving_coords);
        Piece target = square(target_coords);

        if (!moving) {
            return false;
        }

        if (target && moving.color() == target.color()) {
            return false;
        }

        switch (moving.rank()) {
            case NONE: {
                // shouldn't ever happen
                return false;
            }
            case PAWN: {
                // pawn can move two steps on first move:
                if (moving.color() == WHITE) {
                    if (moving_coords.y() == 6 && target_coords.y() == 4 && moving_coords.x() == target_coords.x()) {
                        // only if both spots are not occupied

                        if (square(Coords(target_coords.x(), 5))) {
                            return false;
                        }

                        if (square(Coords(target_coords.x(), 4))) {
                            return false;
                        }

                        return true;
                    }
                } else {
                    if (moving_coords.y() == 1 && target_coords.y() == 3 && moving_coords.x() == target_coords.x()) {
                        // ditto

                        if (square(Coords(target_coords.x(), 2))) {
                            return false;
                        }

                        if (square(Coords(target_coords.x(), 3))) {
                            return false;
                        }

                        return true;
                    }
                }

                // pawn may only ever move forward otherwise:
                if (moving.color() == WHITE) {
                    if (moving_coords.y() - 1 != target_coords.y()) {
                        return false;
                    }
                } else {
                    if (moving_coords.y() + 1 != target_coords.y()) {
                        return false;
                    }
                }

                // pawn can move directly forward only if not attacking:
                if (moving_coords.x() == target_coords.x() && !target) {
                    return true;
                }

                // pawn can attack diagonally
                if ((moving_coords.x() - 1 == target_coords.x() || moving_coords.x() + 1 == target_coords.x()) && target) {
                    return true;
                }

                // no other moves are permitted:
                return false;

                // TODO implement promotion
            }
            case ROOK: {
                return is_valid_cardinal_move(moving_coords, target_coords);
            }
            case KNIGHT: {
                if (moving_coords.translate(-2, -1) == target_coords) { return true; }
                if (moving_coords.translate(+2, -1) == target_coords) { return true; }
                if (moving_coords.translate(-2, +1) == target_coords) { return true; }
                if (moving_coords.translate(+2, +1) == target_coords) { return true; }

                if (moving_coords.translate(-1, -2) == target_coords) { return true; }
                if (moving_coords.translate(+1, -2) == target_coords) { return true; }
                if (moving_coords.translate(-1, +2) == target_coords) { return true; }
                if (moving_coords.translate(+1, +2) == target_coords) { return true; }

                return false;
            }
            case BISHOP: {
                return is_valid_diagonal_move(moving_coords, target_coords);
            }
            case QUEEN: {
                return is_valid_cardinal_move(moving_coords, target_coords) || is_valid_diagonal_move(moving_coords, target_coords);
            }
            case KING: {
                int dist_x = abs(target_coords.x() - moving_coords.x());
                int dist_y = abs(target_coords.y() - moving_coords.y());

                if (dist_x == 2 && dist_y == 0) {
                    // castling

                    if (moving.has_moved()) {
                        // can only castle unmoved king
                        return false;
                    }

                    if (target_coords.x() > moving_coords.x()) {
                        // castling kingside

                        Coords thru = moving_coords.translate(1, 0);
                        Coords into = moving_coords.translate(2, 0);
                        Coords rook = moving_coords.translate(3, 0);

                        if (square(thru) || square(into)) {
                            // thru and into squares must be empty
                            return false;
                        }

                        Piece rook_piece = square(rook);
                        if (!rook_piece || rook_piece.rank() != ROOK || rook_piece.has_moved()) {
                            // rook can't have moved
                            return false;
                        }

                        if (is_under_attack(moving.color(), thru) || is_under_attack(moving.color(), into)) {
                            // cannot move king through or into check
                            return false;
                        }

                        return true;
                    } else {
                        // castling queenside

                        Coords thru = moving_coords.translate(-1, 0);
                        Coords into = moving_coords.translate(-2, 0);
                        Coords xtra = moving_coords.translate(-3, 0);
                        Coords rook = moving_coords.translate(-4, 0);

                        if (square(thru) || square(into) || square(xtra)) {
                            // all intermediate squares must be empty
                            return false;
                        }

                        Piece rook_piece = square(rook);
                        if (!rook_piece || rook_piece.rank() != ROOK || rook_piece.has_moved()) {
                            // rook can't have moved
                            return false;
                        }

                        if (is_under_attack(moving.color(), thru) || is_under_attack(moving.color(), into)) {
                            // cannot move king through or into check
                            return false;
                        }

                        return true;
                    }
                } else if (dist_x <= 1 && dist_y <= 1) {
                    // normal move
                    return true;
                } else {
                    return false;
                }
            }
        }

        return true;
    }

    Coords find_king(color_t player) const {
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                Coords coords = Coords(x, y);
                Piece piece = square(coords);

                if (piece.color() == player && piece.rank() == KING) {
                    return coords;
                }
            }
        }

        panic("find_king did not find king");
    }

    bool is_valid_cardinal_move(Coords moving_coords, Coords target_coords) const {
        int dist_x = abs(target_coords.x() - moving_coords.x());
        int dist_y = abs(target_coords.y() - moving_coords.y());

        // make sure movement only occurs along one axis
        if (dist_x != 0 && dist_y != 0) {
            return false;
        }

        int scale_x = signum(target_coords.x() - moving_coords.x());
        int scale_y = signum(target_coords.y() - moving_coords.y());

        int dist = max(dist_x, dist_y);

        for (int i = 1; i < dist; i++) {
            Coords intermediate = moving_coords.translate(i * scale_x, i * scale_y);
            if (square(intermediate)) {
                return false;
            }
        }

        return true;
    }

    bool is_valid_diagonal_move(Coords moving_coords, Coords target_coords) const {
        int dist_x = abs(target_coords.x() - moving_coords.x());
        int dist_y = abs(target_coords.y() - moving_coords.y());

        // make sure movement is diagonal
        if (dist_x != dist_y) {
            return false;
        }

        int scale_x = signum(target_coords.x() - moving_coords.x());
        int scale_y = signum(target_coords.y() - moving_coords.y());

        for (int i = 1; i < dist_x; i++) {
            Coords intermediate = moving_coords.translate(i * scale_x, i * scale_y);
            if (square(intermediate)) {
                return false;
            }
        }

        return true;
    }
};

typedef enum {
    SELECT,
    MOVE,
} state_discriminant_t;

typedef struct {
    state_discriminant_t which;
    union {
        struct {
            Coords cursor;
        } select;
        struct {
            Coords selected;
            Coords cursor;
        } move;
    } as;
} game_state_t;

static Coords move_cursor(Coords cursor) {
    if (buttons.justPressed(LEFT_BUTTON)) {
        if (cursor.x() > 0) {
            cursor = cursor.translate(-1, 0);
        }
    }

    if (buttons.justPressed(RIGHT_BUTTON)) {
        if (cursor.x() < 7) {
            cursor = cursor.translate(1, 0);
        }
    }

    if (buttons.justPressed(UP_BUTTON)) {
        if (cursor.y() > 0) {
            cursor = cursor.translate(0, -1);
        }
    }

    if (buttons.justPressed(DOWN_BUTTON)) {
        if (cursor.y() < 7) {
            cursor = cursor.translate(0, 1);
        }
    }

    return cursor;
}

class Chess {
    Board board;
    game_state_t state;
    bool frame_count;
    color_t current_player;

public:
    Chess()
        : state({
            .which = SELECT,
            .as = {
                .select = { .cursor = Coords(0, 6) },
            },
        })
        , frame_count(false)
        , current_player(WHITE)
    {}

    void loop() {
        frame_count = !frame_count;
        receive_input();
        render();
    }

private:
    void receive_input() {
        switch (state.which) {
            case SELECT: {
                state.as.select.cursor = move_cursor(state.as.select.cursor);

                if (buttons.justPressed(A_BUTTON)) {
                    Coords cursor = state.as.select.cursor;

                    if (board.square(cursor).color() == current_player) {
                        state.which = MOVE;
                        state.as.move.selected = cursor;
                        state.as.move.cursor = cursor;
                    }
                }

                break;
            }
            case MOVE: {
                state.as.move.cursor = move_cursor(state.as.move.cursor);

                if (buttons.justPressed(A_BUTTON)) {
                    Coords selected = state.as.move.selected;
                    Coords cursor = state.as.move.cursor;

                    if (board.is_valid_turn(selected, cursor, current_player)) {
                        Board new_board = board.move(selected, cursor);
                        board = new_board;
                        next_player();
                    }
                } else if (buttons.justPressed(B_BUTTON)) {
                    Coords selected = state.as.move.selected;
                    state.which = SELECT;
                    state.as.select.cursor = selected;
                }

                break;
            }
        }
    }

    void next_player() {
        int y_begin, y_scale;

        if (current_player == WHITE) {
            current_player = BLACK;
            y_begin = 0;
            y_scale = 1;
        } else {
            current_player = WHITE;
            y_begin = 7;
            y_scale = -1;
        }

        for (int i = 0; i < 8; i++) {
            int y = y_begin + i * y_scale;
            for (int x = 0; x < 8; x++) {
                const Piece piece = board.square(Coords(x, y));
                if (piece && piece.color() == current_player) {
                    state.which = SELECT;
                    state.as.select.cursor = Coords(x, y);
                    return;
                }
            }
        }

        // should never happen, TODO panic
    }

    void render() {
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                draw_square(Coords(x, y));
            }
        }

        bool in_check = board.is_check(current_player);

        if (in_check) {
            if (board.is_checkmate(current_player)) {
                int w = 72;
                int h = 28;
                int x = (128 - w) / 2;
                int y = (64 - h) / 2;

                // show game over modal
                arduboy.drawRect(x - 1, y - 1, w + 2, h + 2, WHITE);
                arduboy.fillRect(x, y, w, h, BLACK);
                arduboy.setCursor(x + 7, y + 4);
                arduboy.print("Checkmate!");
                arduboy.setCursor(x + 7, y + 16);
                if (current_player == WHITE) {
                    arduboy.print("Black wins");
                } else {
                    arduboy.print("White wins");
                }
                return;
            }
        }

        // draw selection
        switch (state.which) {
            case SELECT: {
                int x = state.as.select.cursor.x() * 8 + 32;
                int y = state.as.select.cursor.y() * 8;
                arduboy.drawRect(x - 1, y - 1, 10, 10, frame_count);
                break;
            }
            case MOVE: {
                int x = state.as.move.selected.x() * 8 + 32;
                int y = state.as.move.selected.y() * 8;
                arduboy.drawRect(x - 1, y - 1, 10, 10, WHITE);

                if (state.as.move.selected != state.as.move.cursor) {
                    int x = state.as.move.cursor.x() * 8 + 32;
                    int y = state.as.move.cursor.y() * 8;
                    arduboy.drawRect(x - 1, y - 1, 10, 10, frame_count);
                }
                break;
            }
        }

        if (current_player == WHITE) {
            arduboy.setCursor(0, 64 - 8);
            arduboy.print("White");

            if (in_check) {
                arduboy.setCursor(0, 64 - 16);
                arduboy.print("CHECK");
            }
        } else {
            arduboy.setCursor(0, 0);
            arduboy.print("Black");

            if (in_check) {
                arduboy.setCursor(0, 8);
                arduboy.print("CHECK");
            }
        }
    }

    void draw_square(Coords square) {
        Piece piece = board.square(square);

        int x = square.x() * 8 + 32;
        int y = square.y() * 8;

        static const uint8_t PROGMEM SQUARE_SPRITES[3][8] = {
            // black:
            { 0x00, 0xaa, 0x00, 0xaa, 0x00, 0xaa, 0x00, 0xaa },
            // white:
            { 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa },
        };

        // draw square, but only if a valid move target in move mode
        if (state.which != MOVE || board.is_valid_turn(state.as.move.selected, square, current_player)) {
            int square_sprite_idx = ~(square.x() ^ square.y()) & 1;
            arduboy.drawBitmap(x, y, SQUARE_SPRITES[square_sprite_idx], 8, 8, 1);
        }

        // draw outline in reverse colour
        const uint8_t* sprite = RANK_SPRITES[piece.rank()];
        for (int y_off = -1; y_off <= 1; y_off++) {
            for (int x_off = -1; x_off <= 1; x_off++) {
                arduboy.drawBitmap(x + x_off, y + y_off, sprite, 8, 8, !piece.color());
            }
        }

        // draw sprite in piece colour
        arduboy.drawBitmap(x, y, RANK_SPRITES[piece.rank()], 8, 8, piece.color());
    }
};

Chess chess;

void chess_init() {
    arduboy.beginNoLogo();
}

void chess_loop() {
    if (arduboy.nextFrame()) {
        buttons.poll();
        arduboy.clear();

        if (show_logo) {
            if (buttons.justPressed(A_BUTTON) || buttons.justPressed(B_BUTTON)) {
                show_logo = false;
            } else {
                arduboy.drawBitmap(0, 0, LOGO, 128, 64, WHITE);
            }
        } else {
            chess.loop();
        }

        arduboy.display();
    }
}
