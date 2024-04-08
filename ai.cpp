// AI
// This is where you build your AI
#include <iostream>
#include "ai.hpp"
#include <string>
#include <cstring>
#include <ctime>
#include <array>
#include <vector>
#include <random>
#include <algorithm>
#include <sstream>
#include <chrono> // for timing
#include <climits> 
#include <thread> // for std::this_thread
#include <stdexcept> // for std::runtime_error

using namespace std;
using namespace chrono;
// <<-- Creer-Merge: includes -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
// You can add #includes here for your AI.
//#include "chessBoard.hpp"

// <<-- /Creer-Merge: includes -->>
const int BOARD_SIZE = 8;

namespace cpp_client
{

namespace chess
{

class chessBoard
{
    public:
        // Function to check if the given coordinates are valid on the chessboard
        bool isValid(int x, int y)
        {
            return (x >= 0 && x < 8 && y >= 0 && y < 8);
        }
        // ======================================================= Make chess board =====================================================//
        // make function that creates chess board
        void initializeBoard(const std::string& fen, char chessboard[8][8]) 
        {
            char fen_chars[64];
            int fen_length = fen.length();

            for(int i = 0; i < fen_length; ++i) 
            {
                fen_chars[i] = fen[i];
            }
            int rank = 0;  // Rank index
            int file = 0;  // File index

            for (int i = 0; i < fen_length; ++i) 
            {
                if (fen_chars[i] == '/') 
                {
                    rank++;  // Move to the next rank
                    file = 0;  // Reset the file index
                } 
                else if (isdigit(fen_chars[i])) 
                {
                    int offset = fen_chars[i] - '0';
                    for (int j = 0; j < offset; ++j) 
                    {
                        chessboard[rank][file++] = '-';
                    }
                } 
                else 
                {
                    chessboard[rank][file++] = fen_chars[i];
                }
            }
        }

        // ======================================================= Print chess board ========================================================//
        void printBoard(const char chessboard[8][8]) {
            for (int rank = 0; rank < 8; ++rank) 
            {
                for (int file = 0; file < 8; ++file) 
                {
                    std::cout << chessboard[rank][file] << ' ';
                }
                std::cout << std::endl;
            }
            cout << "==============================" << endl;
        }

        // Function for pawn moves
        // Function to make a random move of a pawn on your side of the board 
        // ======================================================= Make Pawn move  ============================================================//
        // Function to get all possible pawn moves in UCI format
        vector<string> getPawnMoves(char chessboard[8][8], char color)
        {
            vector<string> possibleMoves;
            possibleMoves.reserve(4);

            // Set the direction of movement based on the color
            int direction = (color == 'w') ? -1 : 1;

            // Traverse each cell of the chessboard
            for (int row = 0; row < 8; row++) {
                for (int col = 0; col < 8; col++) {
                    if (chessboard[row][col] == (color == 'w' ? 'P' : 'p')) {
                        // Check for valid pawn moves
                        // Move one square forward
                        int newRow = row + direction;
                        int newCol = col;
                        if (newRow >= 0 && newRow < 8 && chessboard[newRow][newCol] == '-') {
                            string move = "";
                            move += col + 'a';
                            move += 8 - row + '0';
                            move += col + 'a';
                            move += 8 - newRow + '0';
                            possibleMoves.push_back(move);
                        }

                        // Move two squares forward from the starting position
                        if (((color == 'w' && row == 6) || (color == 'b' && row == 1)) &&
                            chessboard[row + direction][col] == ' ' && chessboard[row + 2 * direction][col] == '-') {
                            string move = "";
                            move += col + 'a';
                            move += 8 - row + '0';
                            move += col + 'a';
                            move += 8 - (row + 2 * direction) + '0';
                            possibleMoves.push_back(move);
                        }

                        // Capture diagonally to the left if opponent's piece is present
                        int newRowCapture = row + direction;
                        int newColCapture = col - 1;
                        if (newRowCapture >= 0 && newRowCapture < 8 && newColCapture >= 0 && newColCapture < 8 &&
                            chessboard[newRowCapture][newColCapture] != '-' &&
                            ((color == 'w' && islower(chessboard[newRowCapture][newColCapture])) ||
                            (color == 'b' && isupper(chessboard[newRowCapture][newColCapture])))) {
                            std::string move = "";
                            move += col + 'a';
                            move += 8 - row + '0';
                            move += newColCapture + 'a';
                            move += 8 - newRowCapture + '0';
                            possibleMoves.push_back(move);
                        }

                        // Capture diagonally to the right
                        int newRowCapture2 = row + direction;
                        int newColCapture2 = col + 1;
                        if (newRowCapture2 >= 0 && newRowCapture2 < 8 && newColCapture2 >= 0 && newColCapture2 < 8 &&
                            chessboard[newRowCapture2][newColCapture2] != '-' &&
                            ((color == 'w' && islower(chessboard[newRowCapture2][newColCapture2])) ||
                            (color == 'b' && isupper(chessboard[newRowCapture2][newColCapture2])))) {
                            //chessboard[newRowCapture2][newColCapture2] != chessboard[row][col]) {
                            string move = "";
                            move += col + 'a';
                            move += 8 - row + '0';
                            move += newColCapture2 + 'a';
                            move += 8 - newRowCapture2 + '0';
                            possibleMoves.push_back(move);
                        }
                    }
                }
            }

            return possibleMoves;
        }
        // ======================================================= Make Knight move  ============================================================//
        // Function to generate all possible knight moves
        vector<string> getPossibleKnightMoves(char chessboard[8][8], char color)
        {
            vector<string> possibleMoves;
            int dx[] = {2, 2, 1, 1, -2, -2, -1, -1};
            int dy[] = {1, -1, 2, -2, 1, -1, 2, -2};

            // Iterate over the chessboard
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    // Check if the current cell contains a knight of the player's color
                    if (chessboard[i][j] == 'N' && ((color == 'w' && isupper(chessboard[i][j])) || (color == 'b' && islower(chessboard[i][j]))))
                    {
                        // Generate all possible knight moves
                        for (int k = 0; k < 8; k++)
                        {
                            int nx = i + dx[k];
                            int ny = j + dy[k];
                            if (isValid(nx, ny) && (chessboard[nx][ny] == '-' || (islower(chessboard[nx][ny]) && color == 'w') || (isupper(chessboard[nx][ny]) && color == 'b')))
                            {
                                string move = "";
                                move += static_cast<char>('a' + j);
                                move += static_cast<char>('8' - i);
                                move += static_cast<char>('a' + ny);
                                move += static_cast<char>('8' - nx);
                                possibleMoves.push_back(move);
                            }
                        }
                    }

                    // Check if the current cell contains a knight of the player's color
                    if (chessboard[i][j] == 'n' && ((color == 'w' && isupper(chessboard[i][j])) || (color == 'b' && islower(chessboard[i][j]))))
                    {
                        // Generate all possible knight moves
                        for (int k = 0; k < 8; k++)
                        {
                            int nx = i + dx[k];
                            int ny = j + dy[k];
                            if (isValid(nx, ny) && (chessboard[nx][ny] == '-' || (islower(chessboard[nx][ny]) && color == 'w') || (isupper(chessboard[nx][ny]) && color == 'b')))
                            {
                                string move = "";
                                move += static_cast<char>('a' + j);
                                move += static_cast<char>('8' - i);
                                move += static_cast<char>('a' + ny);
                                move += static_cast<char>('8' - nx);
                                possibleMoves.push_back(move);
                            }
                        }
                    }
                }
            }

            return possibleMoves;
        }

        // ======================================================= Make Bishop move  ============================================================//
        std::vector<std::string> getPossibleBishopMoves(char chessboard[8][8], char color)
        {
            std::vector<std::string> possibleMoves;
            int dx[] = {-1, -1, 1, 1};
            int dy[] = {-1, 1, -1, 1};

            // Iterate over the chessboard
            for (int row = 0; row < 8; row++) {
                for (int col = 0; col < 8; col++) {
                    // Check if the current cell contains a bishop of the player's color
                    if (chessboard[row][col] == (color == 'w' ? 'B' : 'b')) {
                        // Generate all possible bishop moves
                        for (int k = 0; k < 4; k++) {
                            for (int dist = 1; dist < 8; dist++) {
                                int newRow = row + dist * dx[k];
                                int newCol = col + dist * dy[k];
                                if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                                    if (chessboard[newRow][newCol] == '-') {
                                        string move = "";
                                        move += col + 'a';
                                        move += 8 - row + '0';
                                        move += newCol + 'a';
                                        move += 8 - newRow + '0';
                                        possibleMoves.push_back(move);
                                    } else if (islower(chessboard[newRow][newCol]) != islower(chessboard[row][col])) {
                                        // Capture the opponent's piece
                                        string move = "";
                                        move += col + 'a';
                                        move += 8 - row + '0';
                                        move += newCol + 'a';
                                        move += 8 - newRow + '0';
                                        possibleMoves.push_back(move);
                                        break;
                                    } else {
                                        // Break the loop if the bishop encounters a piece of the same color
                                        break;
                                    }
                                } else {
                                    // Break the loop if the bishop goes out of the chessboard
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            return possibleMoves;
        }


        // ======================================================= Make Rook move  ============================================================//
        std::vector<std::string> getPossibleRookMoves(char chessboard[8][8], char color)
        {
            std::vector<std::string> possibleMoves;
            int dx[] = {-1, 0, 1, 0};
            int dy[] = {0, 1, 0, -1};

            // Iterate over the chessboard
            for (int row = 0; row < 8; row++) {
                for (int col = 0; col < 8; col++) {
                    // Check if the current cell contains a rook of the player's color
                    if (chessboard[row][col] == (color == 'w' ? 'R' : 'r')) {
                        // Generate all possible rook moves
                        for (int k = 0; k < 4; k++) {
                            for (int dist = 1; dist < 8; dist++) {
                                int newRow = row + dist * dx[k];
                                int newCol = col + dist * dy[k];
                                if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                                    if (chessboard[newRow][newCol] == '-') {
                                        std::string move = "";
                                        move += col + 'a';
                                        move += 8 - row + '0';
                                        move += newCol + 'a';
                                        move += 8 - newRow + '0';
                                        possibleMoves.push_back(move);
                                    } else if ((color == 'w' && islower(chessboard[newRow][newCol])) ||
                                            (color == 'b' && isupper(chessboard[newRow][newCol]))) {
                                        // Break the loop if the rook encounters an opponent's piece
                                        std::string move = "";
                                        move += col + 'a';
                                        move += 8 - row + '0';
                                        move += newCol + 'a';
                                        move += 8 - newRow + '0';
                                        possibleMoves.push_back(move);
                                        break;
                                    } else {
                                        // Break the loop if the rook encounters its own side's piece
                                        break;
                                    }
                                } else {
                                    // Break the loop if the rook goes out of the chessboard
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            return possibleMoves;
        }

        // ======================================================= Make Queen move  ============================================================//
        std::vector<std::string> getPossibleQueenMoves(char chessboard[8][8], char color)
        {
            std::vector<std::string> possibleMoves;
            int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
            int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

            // Iterate over the chessboard
            for (int row = 0; row < 8; row++) {
                for (int col = 0; col < 8; col++) {
                    // Check if the current cell contains a queen of the player's color
                    if (chessboard[row][col] == (color == 'w' ? 'Q' : 'q')) {
                        // Generate all possible queen moves
                        for (int k = 0; k < 8; k++) {
                            for (int dist = 1; dist < 8; dist++) {
                                int newRow = row + dist * dx[k];
                                int newCol = col + dist * dy[k];
                                if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                                    if (chessboard[newRow][newCol] == '-') {
                                        std::string move = "";
                                        move += col + 'a';
                                        move += 8 - row + '0';
                                        move += newCol + 'a';
                                        move += 8 - newRow + '0';
                                        possibleMoves.push_back(move);
                                    } else if ((color == 'w' && islower(chessboard[newRow][newCol])) ||
                                            (color == 'b' && isupper(chessboard[newRow][newCol]))) {
                                        // Break the loop if the queen encounters an opponent's piece
                                        std::string move = "";
                                        move += col + 'a';
                                        move += 8 - row + '0';
                                        move += newCol + 'a';
                                        move += 8 - newRow + '0';
                                        possibleMoves.push_back(move);
                                        break;
                                    } else {
                                        // Break the loop if the queen encounters its own side's piece
                                        break;
                                    }
                                } else {
                                    // Break the loop if the queen goes out of the chessboard
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            return possibleMoves;
        }

        // ======================================================= Make King move  ============================================================//
        std::vector<std::string> getPossibleKingMoves(char chessboard[8][8], char color)
        {
            std::vector<std::string> possibleMoves;
            int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
            int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

            // Iterate over the chessboard
            for (int row = 0; row < 8; row++) {
                for (int col = 0; col < 8; col++) {
                    // Check if the current cell contains a king of the player's color
                    if (chessboard[row][col] == (color == 'w' ? 'K' : 'k')) {
                        // Generate all possible king moves
                        for (int k = 0; k < 8; k++) {
                            int newRow = row + dx[k];
                            int newCol = col + dy[k];
                            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                                if (chessboard[newRow][newCol] == '-') {
                                    std::string move = "";
                                    move += col + 'a';
                                    move += 8 - row + '0';
                                    move += newCol + 'a';
                                    move += 8 - newRow + '0';
                                    possibleMoves.push_back(move);
                                } else if ((color == 'w' && islower(chessboard[newRow][newCol])) ||
                                        (color == 'b' && isupper(chessboard[newRow][newCol]))) {
                                    // Break the loop if the king encounters an opponent's piece
                                    std::string move = "";
                                    move += col + 'a';
                                    move += 8 - row + '0';
                                    move += newCol + 'a';
                                    move += 8 - newRow + '0';
                                    possibleMoves.push_back(move);
                                }
                            }
                        }
                    }
                }
            }

            return possibleMoves;
        }
        

        // ======================================================= Get random string  ============================================================//
        string getRandomString(const vector<string>& strings)
        {
            random_device rd;  // Seed the random number generator
            mt19937 rng(rd()); // Initialize the random number generator

            // Generate a random index within the range of the vector size
            uniform_int_distribution<int> uni(0, strings.size() - 1);
            int randomIndex = uni(rng);

            // Return the randomly selected string
            return strings[randomIndex];
        }

        // ======================================================= Check if in check  ============================================================//
        // Function to check if any of the moves hit the king
        bool check(char board[][8], char currentPlayer) {
            int kingRow = -1;
            int kingCol = -1;
            char kingPiece;
            char enemy;

            if (currentPlayer == 'w') {
                kingPiece = 'K';
                enemy = 'b';
            } else {
                kingPiece = 'k';
                enemy = 'w';  
            }

            // Find the position of the current player's king
            for (int row = 0; row < 8; ++row) {
                for (int col = 0; col < 8; ++col) {
                    if (board[row][col] == kingPiece) {
                        kingRow = row;
                        kingCol = col;
                        break;
                    }
                }
            }

            string uciPosition;
            char uciFile = '\0';
            int uciRank = 0;
            char destFile = '\0';
            int destRank = 0;

            // Convert king position to UCI notation
            if (kingRow != -1 && kingCol != -1) {
                uciFile = 'a' + kingCol;
                uciRank = 8 - kingRow;
                uciPosition = string() + uciFile + to_string(uciRank);
            }

            vector<string> opponent = getMoves(board, enemy); // get all moves that opponent can make based on board

            // Check if any of the opponent's moves hit the king
            for (const string& opponentMove : opponent) {
                destFile = opponentMove[2];
                destRank = opponentMove[3] - '0';

                //cout << "=================================================" << endl;
                //cout << "opponent move: " << opponentMove << endl;
                //cout << "King: " << uciFile << uciRank << endl;
                //cout << "target: " << destFile << destRank << endl;
                
                if (destFile == uciFile && destRank == uciRank) {
                    //cout << "Move that puts me in check!: " << opponentMove << endl;
                    return true;
                }
                
            }

            return false;
        }


        // ===================================== Get all possible move (not including castling or en passant)  ================================//
        vector<string> getPossibleMoves(char chessboard[8][8], char color) {
            char enemy;
            if (color == 'w') {
                enemy = 'b';
            } else {
                enemy = 'w';
            }
            
            vector<string> movesOutOfCheck; // valid moves that don't put me in check
            vector<string> my_moves = getMoves(chessboard, color); // get all moves that i can make based on current board
            vector<string> opponent_moves = getMoves(chessboard, enemy); // get all moves that opponent can make based on current board
            
            // Iterate over each possible move that you have available (doesn't mean its valid)
            for (const string& move : my_moves) {

                // Apply the move to a copy of the chessboard
                char tempChessboard[8][8];
                memcpy(tempChessboard, chessboard, sizeof(char) * 8 * 8); // copies content of original to temp

                int startRow = 8 - (move[1] - '0');
                int startCol = move[0] - 'a';
                int endRow = 8 - (move[3] - '0');
                int endCol = move[2] - 'a';

                tempChessboard[endRow][endCol] = tempChessboard[startRow][startCol];
                tempChessboard[startRow][startCol] = '-';

                // Check if the move successfully gets the king out of check
                if (!check(tempChessboard, color)) 
                {
                    movesOutOfCheck.push_back(move); // if it does then add the move to the vector string
                }
            }

            return movesOutOfCheck;
        }

        // ======================================================= enPassant  ============================================================//
        string canEnPassant(const string& fen, char board[][8], char currentPlayer) {
            
             std::string enPassant = "-";
    
            // Splitting the FEN string into individual sections
            size_t spacePos = fen.find(' ');
            string boardSection = fen.substr(0, spacePos);
            
            // Finding the en passant section in the FEN string
            size_t epPos = fen.find(" ep ");
            if (epPos != string::npos) {
                size_t startPos = epPos + 4;
                size_t endPos = fen.find(' ', startPos);
                enPassant = fen.substr(startPos, endPos - startPos);
            }
            //cout << "enPassant target: " << enPassant << endl; 

            // Find the rank and file of the en passant target square
            int targetRank = enPassant[1];
            int targetFile = enPassant[0];

            //cout << "enPassant file and rank: " << targetFile << targetRank << endl;

            // Check if the target square is empty or contains the opponent's pawn
            if (board[targetRank][targetFile] != '-' && ((board[targetRank][targetFile] == 'P' && currentPlayer == 'w') || (board[targetRank][targetFile] == 'p' && currentPlayer == 'b'))) {
                // Check if the adjacent squares have a pawn of the current player
                int adjacentRank = (currentPlayer == 'w') ? targetRank - 1 : targetRank + 1;

                // Check left adjacent square
                if (targetFile > 0 && board[adjacentRank][targetFile - 1] == 'P' && currentPlayer == 'w') {
                    return string(1, 'a' + targetFile - 1) + to_string(8 - adjacentRank) + string(1, 'a' + targetFile) + to_string(8 - targetRank);
                }
                if (targetFile > 0 && board[adjacentRank][targetFile - 1] == 'p' && currentPlayer == 'b') {
                    return string(1, 'a' + targetFile - 1) + to_string(8 - targetRank) + string(1, 'a' + targetFile) + to_string(8 - adjacentRank);
                }

                // Check right adjacent square
                if (targetFile < 7 && board[adjacentRank][targetFile + 1] == 'P' && currentPlayer == 'w') { 
                    return string(1, 'a' + targetFile + 1) + to_string(8 - adjacentRank) + string(1, 'a' + targetFile) + to_string(8 - targetRank);
                }
                if (targetFile < 7 && board[adjacentRank][targetFile + 1] == 'p' && currentPlayer == 'b') {
                    return string(1, 'a' + targetFile + 1) + to_string(8 - targetRank) + string(1, 'a' + targetFile) + to_string(8 - adjacentRank);
                }
            }
            
            return ""; // Return an empty string if en passant is not possible
        }

        // ======================================================= Castling  ============================================================//
        string canCastle(const string& fen, char board[][8], char currentPlayer) {
            // Extract the castling availability from the FEN string
            //cout << "fen string: " << fen << endl; // check if fen string valid

            size_t fenPos = fen.find(' ');
            size_t CastlePos = fen.find(' ', fenPos + 1);
            size_t CastleAvailabilityPos = fen.find(' ', CastlePos + 1);
            std::string CastleAvailability = fen.substr(CastlePos + 1, CastleAvailabilityPos- (CastlePos + 1));


            //cout << "castling availability: " << CastleAvailability << endl;

            // Determine the king's initial position based on the current player
            int kingRow = (currentPlayer == 'w') ? 7 : 0;
            int kingCol = 4;

            // Check if the king is in its initial position and castling is available for the player
            if (board[kingRow][kingCol] == 'K' && currentPlayer == 'w') {
                if (CastleAvailability.find('K') != string::npos) {
                    // Check if the squares between the king and rook are empty
                    if (board[kingRow][kingCol + 1] == '-' && board[kingRow][kingCol + 2] == '-') {
                        return "e1g1"; // Return the castling move in UCI format
                    }
                }
                if (CastleAvailability.find('Q') != string::npos) {
                    // Check if the squares between the king and rook are empty
                    if (board[kingRow][kingCol - 1] == '-' && board[kingRow][kingCol - 2] == '-' && board[kingRow][kingCol - 3] == '-') {
                        return "e1c1"; // Return the castling move in UCI format
                    }
                }
            } else if (board[kingRow][kingCol] == 'k' && currentPlayer == 'b') {
                if (CastleAvailability.find('k') != string::npos) {
                    // Check if the squares between the king and rook are empty
                    if (board[kingRow][kingCol + 1] == '-' && board[kingRow][kingCol + 2] == '-') {
                        return "e8g8"; // Return the castling move in UCI format
                    }
                }
                if (CastleAvailability.find('q') != string::npos) {
                    // Check if the squares between the king and rook are empty
                    if (board[kingRow][kingCol - 1] == '-' && board[kingRow][kingCol - 2] == '-' && board[kingRow][kingCol - 3] == '-') {
                        return "e8c8"; // Return the castling move in UCI format
                    }
                }
            }

            return ""; // Return an empty string if castling is not possible
        }

        //==============================================================================================================================================
        vector<string> getMoves(char board[][8], char color)
        {
            vector<string> PawnMoves = getPawnMoves(board, color); // get pawn moves
            vector<string> KnightMoves = getPossibleKnightMoves(board, color);
            vector<string> BishopMoves = getPossibleBishopMoves(board, color);
            vector<string> RookMoves = getPossibleRookMoves(board, color);
            vector<string> QueenMoves = getPossibleQueenMoves(board, color);
            vector<string> KingMoves = getPossibleKingMoves(board, color);
            vector<string> AllMoves; // all moves i can make

            AllMoves.insert(AllMoves.end(), PawnMoves.begin(), PawnMoves.end());
            AllMoves.insert(AllMoves.end(), KnightMoves.begin(), KnightMoves.end());
            AllMoves.insert(AllMoves.end(), BishopMoves.begin(), BishopMoves.end());
            AllMoves.insert(AllMoves.end(), RookMoves.begin(), RookMoves.end());
            AllMoves.insert(AllMoves.end(), QueenMoves.begin(), QueenMoves.end());
            AllMoves.insert(AllMoves.end(), KingMoves.begin(), KingMoves.end());
            return AllMoves;
        }
        // ============================= Sort chess moves =====================================================//
        void sortChessMoves(vector<string>& moves) {
            sort(moves.begin(), moves.end());
        }
        
        // ==================================================== heuristic ============================================================ //
        int heur(char board[][8]) {
            int value = 0;
            
            // Assign points to each piece based on its type and position
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    char piece = board[i][j];
                    
                    if (piece == 'p') {
                        value -= 100;
                        if (i > 1 && i < 6) { // Encourage pawns to advance to the center
                            value += 10;
                        }
                        if (i == 6) { // Give extra points for pawns that reach the opponent's side
                            value += 150;
                        }
                    } 
                    else if (piece == 'P') {
                        value += 100;
                        if (i > 1 && i < 6) { // Encourage pawns to advance to the center
                            value -= 10;
                        }
                        if (i == 1) { // Give extra points for pawns that reach the opponent's side
                            value -= 150;
                        }
                    } 
                    else if (piece == 'n' || piece == 'b') {
                        value -= 320;
                        if (i > 1 && i < 6) { // Encourage knights and bishops to advance to the center
                            value += 20;
                        }
                    } 
                    else if (piece == 'N' || piece == 'B') {
                        value += 320;
                        if (i > 1 && i < 6) { // Encourage knights and bishops to advance to the center
                            value -= 20;
                        }
                    } 
                    else if (piece == 'r') {
                        value -= 500;
                        if (i > 1 && i < 6) { // Encourage rooks to control the center
                            value += 50;
                        }
                    } 
                    else if (piece == 'R') {
                        value += 500;
                        if (i > 1 && i < 6) { // Encourage rooks to control the center
                            value -= 50;
                        }
                    } 
                    else if (piece == 'q') {
                        value -= 900;
                        if (i > 1 && i < 6) { // Encourage queens to control the center
                            value += 80;
                        }
                    } 
                    else if (piece == 'Q') {
                        value += 900;
                        if (i > 1 && i < 6) { // Encourage queens to control the center
                            value -= 80;
                        }
                    }
                }
            }
            
            return value;
        }



        // =============================================== Applies move onto board ===================================================== //
        void applyMove(char board[][8], string move) {
            // Extract source and destination squares from UCI move
            int srcFile = move[0] - 'a';
            int srcRank = move[1] - '1';
            int destFile = move[2] - 'a';
            int destRank = move[3] - '1';

            // Move piece from source to destination
            board[destRank][destFile] = board[srcRank][srcFile];
            board[srcRank][srcFile] = ' ';
        }

        // ============================================= Event Horizon =============================================================//
        int quiescentSearch(char board[][8], char player) {
            //int quietVal = heur(board);
            vector<string> possibleMoves = getMoves(board, player); // gets all possible moves
            if (player != 'w') { // if player is black
                int maxVal = -1000;
                string move = "";
                // Check all moves
                for (auto i : possibleMoves) {
                    char newBoard[8][8];
                    memcpy(newBoard, board, 64);
                    applyMove(newBoard, i);
                    // Get the value of the move
                    int value = heur(newBoard);
                    // If the value is equal to maxval, choose a random one
                    if (value == maxVal) {
                        bool boolrand = rand() % 2;
                        if (boolrand == 1) {
                            move = i;
                        }
                    }
                    // If the value is greater than maxval, update maxval and move
                    else if (value > maxVal) {
                        maxVal = value;
                        move = i;
                    }
                }
                // Return the value and the move
                return maxVal;
            } // color is white
            else 
            {
                int minVal = 1000;
                string move = "";
                // Goes through all possible moves
                for (auto i : possibleMoves) {
                    char newBoard[8][8];
                    memcpy(newBoard, board, 64);
                    applyMove(newBoard, i);
                    // Gets the value of the move
                    int value = heur(newBoard);
                    // If the value is equal to minval, choose a random one
                    if (value == minVal) {
                        bool boolrand = rand() % 2;
                        if (boolrand == 1) {
                            move = i;
                        }
                    }
                    // If the value is less than minval, update minval and move
                    else if (value < minVal) {
                        minVal = value;
                        move = i;
                    }
                }
                // Return the value and the move
                return minVal;
            }
        }

        // =========================================== MiniMax =====================================================//
        int minimax(char board[][8], int depth, bool isMaximizingPlayer, char player) 
        {
            vector<string> possibleMoves = getMoves(board, player); // get all possible moves
            possibleMoves = getPossibleMoves(board, player); // get all valid moves
            sortChessMoves(possibleMoves);
            // check if the depth limit has been reached or the game is over
            if (depth == 0 || (possibleMoves.size()==0)) {
                return heur(board);
            }

            // get all legal moves for the current player
            vector<string> moves = getMoves(board, player);

            // evaluate all possible moves
            if (isMaximizingPlayer) {
                int maxEval = INT_MIN;
                for (string move : moves) {
                    // make a copy of the board and apply the move
                    char copyBoard[8][8];
                    memcpy(copyBoard, board, 64);
                    applyMove(copyBoard, move);

                    // recursively evaluate the position
                    int eval = minimax(copyBoard, depth - 1, false, player);

                    // update maxEval
                    if (eval > maxEval) {
                        maxEval = eval;
                    }
                }
                return maxEval;
            } 
            else {
                int minEval = INT_MAX;
                for (string move : moves) {
                    // make a copy of the board and apply the move
                    char copyBoard[8][8];
                    memcpy(copyBoard, board, 64);
                    applyMove(copyBoard, move);

                    // recursively evaluate the position
                    int eval = minimax(copyBoard, depth - 1, true, player);

                    // update minEval
                    if (eval < minEval) {
                        minEval = eval;
                    }
                }
                return minEval;
            }
        }

        // =========================================== get best move =============================================================//
        // function to get the best move using minimax
        string getBestMove(char board[][8], int depth, char player) 
        {
            const int TIME_LIMIT = 10; // in seconds
            auto start_time = std::chrono::steady_clock::now();
            int bestScore = INT_MIN;
            string bestMove = "";
            vector<string> moves = getMoves(board, player);
            moves = getPossibleMoves(board, player);

            for (string move : moves) {
                // Check elapsed time
                auto end_time = std::chrono::steady_clock::now();
                auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
                if (elapsed_time >= TIME_LIMIT) {
                    break;
                }
                
                // make a copy of the board and apply the move
                char copyBoard[8][8];
                memcpy(copyBoard, board, 64);
                applyMove(copyBoard, move);

                // call the minimax function to evaluate the position
                int score = minimax(copyBoard, depth - 1, false, player);

                // update the best move
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = move;
                }
            }
            return bestMove;
        }




};

// ==================================================================================================================================================================//
/// <summary>
/// This returns your AI's name to the game server.
/// Replace the string name.
/// </summary>
/// <returns>The name of your AI.</returns>
std::string AI::get_name() const
{
    // <<-- Creer-Merge: get-name -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // REPLACE WITH YOUR TEAM NAME!
    return "Joe_kang";
    // <<-- /Creer-Merge: get-name -->>
}

/// <summary>
/// This is automatically called when the game first starts, once the game objects are created
/// </summary>
void AI::start()
{
    // <<-- Creer-Merge: start -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // This is a good place to initialize any variables
    
    // <<-- /Creer-Merge: start -->>
}

/// <summary>
/// This is automatically called the game (or anything in it) updates
/// </summary>
void AI::game_updated()
{
    // <<-- Creer-Merge: game-updated -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // If a function you call triggers an update this will be called before it returns.
    // <<-- /Creer-Merge: game-updated -->>
}

/// <summary>
/// This is automatically called when the game ends.
/// </summary>
/// <param name="won">true if you won, false otherwise</param>
/// <param name="reason">An explanation for why you either won or lost</param>
void AI::ended(bool won, const std::string& reason)
{
    //<<-- Creer-Merge: ended -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // You can do any cleanup of your AI here.  The program ends when this function returns.
    //<<-- /Creer-Merge: ended -->>
}

/// <summary>
/// This is called every time it is this AI.player's turn to make a move.
/// </summary>
/// <returns>A string in Universal Chess Interface (UCI) or Standard Algebraic Notation (SAN) formatting for the move you want to make. If the move is invalid or not properly formatted you will lose the game.</returns>
std::string AI::make_move()
{
    // <<-- Creer-Merge: makeMove -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // Put your game logic here for make_move here
    
    // initialize variables
    chessBoard taco;
    string randomMove;
    vector<string> AllMoves;
    vector<string> opponent_moves;
    string fen = game->fen; // get  fen string
    string randomPossibleMove;

    char My_color = player->color[0]; // get the first char to indicate color
    char enemy_color;

    if(My_color == 'w'){
        enemy_color = 'b';
    }
    else{
        enemy_color = 'w';
    }
    
    char board[8][8]; // create board

    taco.initializeBoard(fen, board); // initialize board

    AllMoves = taco.getMoves(board, My_color); // gets all possible moves based on board state
    opponent_moves = taco.getMoves(board, enemy_color); // gets all possible moves based on board state for opponent

    // make move so that it doesn't put me in check or takes me out of check
    taco.check(board,My_color); // double checking if check works
    AllMoves = taco.getPossibleMoves(board, My_color); // get all possible moves including enpassant and castling
    string enPassant = taco.canEnPassant(fen, board, My_color);
    if(enPassant != ""){
        AllMoves.push_back(enPassant); // if there is an enPassant move that isn't just an empty string then push it into the Allmoves vector
    }
    string castle = taco.canCastle(fen, board, My_color);
    if(castle != ""){
        AllMoves.push_back(castle); // if there is a castling move that isn't just an empty string then push it into the Allmoves vector
    }
    
    taco.sortChessMoves(AllMoves); // sort chess moves in alphabetical order

    // if any of the moves are out of range then remove it
    vector<string> valid; 
    for (const auto& str : AllMoves) {
        if (str[0] >= 'a' && str[0] <= 'h' && str[1] >= '1' && str[1] <= '8' && str[2] >= 'a' && str[2] <= 'h' && str[3] >= '1' && str[3] <= '8') {
            valid.push_back(str);
        }
    }
    AllMoves = valid;
    randomPossibleMove = taco.getRandomString(AllMoves); // gets random move out of all possible moves
    
    //===================================== timing stuff and doing minimax ==========================================//
    // timing stuff
    string bestmove = "";
    int maxDepth = 3; // example max depth value
    bestmove = taco.getBestMove(board, maxDepth, My_color); // gets the best move using minimax
    
    //===============================================================================================================//
    int num_moves = AllMoves.size();
    cout << num_moves << endl;
    // print all possible moves that i can make
    for (const auto& str : AllMoves) {
        cout << str << " ";
    }
    cout << endl;
    cout << "My move: " << bestmove << endl;
    cout << endl;

    // <<-- /Creer-Merge: makeMove -->>
    return bestmove;
    //return randomPossibleMove;
    //return "a";
}

//<<-- Creer-Merge: methods -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
// You can add additional methods here for your AI to call
// Constants representing the chess pieces




//<<-- /Creer-Merge: methods -->>

} // chess

} // cpp_client
