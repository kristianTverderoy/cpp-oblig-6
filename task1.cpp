#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using std::make_unique;

using namespace std;

class ChessBoard {
public:
  enum class Color { WHITE, BLACK };

  class Piece {
  public:
    Piece(Color color, char initialX, char initialY)
        : color(color), xCoordinate(initialX), yCoordinate(initialY) {}
    virtual ~Piece() {}
    Color color;
    char xCoordinate;
    char yCoordinate;

    std::string color_string() const {
      if (color == Color::WHITE)
        return "white";
      else
        return "black";
    }

    /// Return color and type of the chess piece
    virtual std::string type() const = 0;

    /// Returns true if the given chess piece move is valid
    virtual bool valid_move(int from_x, int from_y, int to_x,
                            int to_y) const = 0;
    /// Symbol representing the type of chess piece
    virtual char symbol() const = 0;

    virtual char getXLocation() const = 0;
    virtual char getYLocation() const = 0;
    virtual void setLocation(int newXLocation, int newYlocation) = 0;
  };

  class King : public Piece {
  public:
    King(Color color, char initialX, char initialY)
        : Piece(color, initialX, initialY) {}
    std::string type() const override {
      if (color == Color::WHITE) {
        return "White king(K)";
      } else {
        return "Black king(k)";
      }
    }

    bool valid_move(int from_x, int from_y, int to_x, int to_y) const override {
      if (((abs(to_x - from_x) == 1) && (abs(to_y - from_y) == 1)) ||
          ((abs(to_x - from_x) == 0) && (abs(to_y - from_y) == 1)) ||
          ((abs(to_x - from_x) == 1) && (abs(to_y - from_y) == 0))) {
        return true;
      }
      return false;
    }
    /**
     * White king is capitalized, while black is lowercase.
     */
    char symbol() const override {
      if (color == Color::WHITE) {
        return 'K';
      } else {
        return 'k';
      }
    }

    char getXLocation() const override { return this->xCoordinate; }
    char getYLocation() const override { return this->yCoordinate; }
    void setLocation(int toX, int toY) override {
      if (valid_move(this->xCoordinate, this->yCoordinate, toX, toY)) {
        this->xCoordinate = toX;
        this->yCoordinate = toY;
      }
    }
  };

  class Knight : public Piece {
  public:
    Knight(Color color, char initialX, char initialY)
        : Piece(color, initialX, initialY) {};

    std::string type() const override {
      if (color == Color::WHITE) {
        return "White knight(H)";
      } else {
        return "Black knight(h)";
      }
    }

    bool valid_move(int from_x, int from_y, int to_x, int to_y) const override {
      if ((abs(to_x - from_x) == 2 && abs(to_y - from_y) == 1) ||
          ((abs(to_x - from_x) == 1 && abs(to_y - from_y) == 2))) {
        return true;
      }
      return false;
    }

    char symbol() const override {
      if (color == Color::WHITE) {
        return 'H';
      } else {
        return 'h';
      }
    }
    char getXLocation() const override { return this->xCoordinate; }
    char getYLocation() const override { return this->yCoordinate; }

    void setLocation(int toX, int toY) override {
      if (valid_move(this->xCoordinate, this->yCoordinate, toX, toY)) {
        this->xCoordinate = toX;
        this->yCoordinate = toY;
      }
    }
  };

  ChessBoard() {
    // Initialize the squares stored in 8 columns and 8 rows:
    squares.resize(8);
    for (auto &square_column : squares)
      square_column.resize(8);
  }

  /// 8x8 squares occupied by 1 or 0 chess pieces
  vector<vector<shared_ptr<Piece>>> squares;

  /**
   * Gets the piece on coordinate [x][y] in the vector.
   */
  char getPiece(int x, int y) const {
    if (squares[x][y] != nullptr) {
      return squares[x][y]->symbol();
    }
    return '.';
  }

  /// Move a chess piece if it is a valid move.
  /// Does not test for check or checkmate.
  bool move_piece(const std::string &from, const std::string &to) {
    int from_x = from[0] - 'a';
    int from_y = stoi(string() + from[1]) - 1;
    int to_x = to[0] - 'a';
    int to_y = stoi(string() + to[1]) - 1;

    auto &piece_from = squares[from_x][from_y];
    if (piece_from) {
      if (piece_from->valid_move(from_x, from_y, to_x, to_y)) {
        cout << piece_from->type() << " is moving from " << from << " to " << to
             << endl;
        auto &piece_to = squares[to_x][to_y];
        if (piece_to) {
          if (piece_from->color != piece_to->color) {
            cout << piece_to->type() << " is being removed from " << to << endl;
            if (auto king = dynamic_cast<King *>(piece_to.get()))
              cout << king->color_string() << " lost the game" << endl;
          } else {
            // piece in the from square has the same color as the piece in the
            // to square
            cout << "can not move " << piece_from->type() << " from " << from
                 << " to " << to << endl;
            return false;
          }
        }
        piece_to = std::move(piece_from);
        return true;
      } else {
        cout << "can not move " << piece_from->type() << " from " << from
             << " to " << to << endl;
        return false;
      }
    } else {
      cout << "no piece at " << from << endl;
      return false;
    }
  }
};

class ChessBoardPrint {
private:
  const std::function<char(int, int)> afterPieceMove;
  const ChessBoard board;
  const int boardXSize;
  const int boardYSize;

public:
  ChessBoardPrint(const ChessBoard &board_)
      : afterPieceMove(
            [&board_](int x, int y) { return board_.getPiece(x, y); }),
        boardXSize(board_.squares.size()),
        boardYSize(board_.squares[1].size()) {}

  void print() {
    string columns =
        " abcdefghijklmnopqrstuvwxyz"; // more letters incase the chess board is
                                       // > 8 for some reason.
    vector<string> rows{
        "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "10",
        "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
        "21", "22", "23", "24", "25", "26", "27"}; // more rows incase the chess
                                                   // board has a lot of rows
                                                   // too.
                                                   //
    for (int c = 0; c < boardXSize + 1; c++) {
      cout << columns[c] << " ";
      if (c == boardXSize) {
        cout << "\n";
      }
    }
    for (int i = 0; i < boardYSize; i++) {
      for (int j = 0; j < boardXSize; j++) {
        if (j == 0) {
          // sets the row value before plotting the chess board
          cout << rows[(boardYSize - 1) - i] << " ";
        }
        std::cout << afterPieceMove(j, i) << " ";
        if (j == boardXSize - 1) {
          std::cout << "\n";
        }
      }
    }
    cout << "\n";
  }
};

int main() {
  ChessBoard board;
  ChessBoardPrint printer(board);

  board.squares[4][0] =
      make_unique<ChessBoard::King>(ChessBoard::Color::WHITE, 4, 0);
  board.squares[1][0] =
      make_unique<ChessBoard::Knight>(ChessBoard::Color::WHITE, 1, 0);
  board.squares[6][0] =
      make_unique<ChessBoard::Knight>(ChessBoard::Color::WHITE, 6, 0);
  board.squares[4][7] =
      make_unique<ChessBoard::King>(ChessBoard::Color::BLACK, 4, 7);
  board.squares[1][7] =
      make_unique<ChessBoard::Knight>(ChessBoard::Color::BLACK, 1, 7);
  board.squares[6][7] =
      make_unique<ChessBoard::Knight>(ChessBoard::Color::BLACK, 6, 7);
  cout << "Start position \n";
  printer.print();
  cout << "Invalid moves:" << endl;
  board.move_piece("e3", "e2");
  board.move_piece("e1", "e3");
  board.move_piece("b1", "b2");
  cout << endl;

  cout << "A simulated game:" << endl;
  board.move_piece("e1", "e2");
  printer.print();
  board.move_piece("g8", "h6");
  printer.print();
  board.move_piece("b1", "c3");
  printer.print();
  board.move_piece("h6", "g8");
  printer.print();
  board.move_piece("c3", "d5");
  printer.print();
  board.move_piece("g8", "h6");
  printer.print();
  board.move_piece("d5", "f6");
  printer.print();
  board.move_piece("h6", "g8");
  printer.print();
  board.move_piece("f6", "e8");
  printer.print();
}
