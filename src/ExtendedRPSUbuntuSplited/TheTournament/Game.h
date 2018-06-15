#ifndef ADTO_TARGIL1_GAME_H
#define ADTO_TARGIL1_GAME_H

//--------------------------
// Game Class
//--------------------------

// Functions as game manager.Manages the progress of the game, 
// and creates the differents algorithms for the different players of the game.

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "BoardImpl.h"
// TODO delete!!!
#include <fstream>
#include "Player.h"
#include "Joker.h"

#define OUTPUT_FILE_NAME "rps.output"
#define NUM_OF_PLAYERS 2
#define FLAGS_CAPTURED "All flags of the opponent are captured"
#define PIECES_EATEN_PLAYER "All moving PIECEs of the opponent are eaten"
#define PIECES_EATEN_BOTH_PLAYERS "All moving PIECEs of both players are eaten"
// deleted #define TIE_NO_WINNER "A tie - both Moves input files done without a winner"
#define TIE_FLAGS_EATEN "A tie - all flags are eaten by both players in the position files"
//ask #define TIE_NO_MOVING_PIECES "A tie - all moving are eaten by both players in the position files"
#define BAD_POS_PLAYER "Bad Positioning input for player %d"
#define BAD_POS_BOTH_PLAYERS "Bad Positioning input for both players"
#define BAD_MOVE_PLAYER "Bad Moves input for player %d"
#define TIE_NO_FIGHTS "A tie – no fight for 100 moves" // No fight for 100 consequative moves

#define MESSAGE_MAX_LEN 150
#define INPUT_FILE_NAME_MAX_LEN 100

//class HilaAndJaelExtendedRPS::Joker;
//class PlayerAlgorithm;
//class HilaAndJaelExtendedRPS::Player;

using namespace HilaAndJaelExtendedRPS;

class Game
{
// This declaration is needed for the functions to
// recognize the enum.
// It is defined afterwards.
public:
    enum class Winner;

private:

	BoardImpl<Piece> mGameBoard;
	Winner mWinner = Winner::None;

	// shared ptr because it is shared among it's pieces (as their owner) and the game.
	std::vector<shared_ptr<Player>> mPlayersVec;
	std::vector<unique_ptr<PlayerAlgorithm>> mAlgorithmsVec;

	// TODO: delete!!!
	//std::ofstream logFile;

	// Put a non joker piece in given position.  
	// Get detailed about positioning from the given tokens, and checks for tokens validity.
	bool PutNonJokerOnBoard(int playerNum, const std::unique_ptr<PiecePosition>& piecePos, BoardImpl<Piece>& board) const;

	// Changes the joker actual type to the one represented by the given character.
	bool ChangeJokerActualType(Joker* joker, char cJokerRepresantation) const;

	// Initializes the joker with it's owner and actual type represented by the given character.
	bool InitJokerOwnerAndActualType(Joker* joker, char cJokerRepresantation, int playerNum) const;

	// Put a joker piece in given position. 
	// Get detailed about positioning from the given tokens, and checks for tokens validity.
	bool PutJokerOnBoard(int playerNum, const std::unique_ptr<PiecePosition>& piecePos, BoardImpl<Piece>& board) const;

	// Put the given player's initial posiotions on given board
	bool PutPlayerPiecesOnBoard(int playerNum, const std::vector<unique_ptr<PiecePosition>>& playerPiecePositions, BoardImpl<Piece>& board) const;

	// Put the both players initial posiotions on given boards.
	// Report errors if any (bad positioning or no flags).
	bool PutPiecePositionsOnBoard(const std::vector<unique_ptr<PiecePosition>>& player1PiecePositions,
		const std::vector<unique_ptr<PiecePosition>>& player2PiecePositions,
		BoardImpl<Piece>& tempPlayer1Board, BoardImpl<Piece>& tempPlayer2Board);

	// When positioning or moves from the algorithm has bad format,
	// Report game over message and winner.
	void SetBadInputMessageWithWinner(int loserNum, Winner winner, const char* templateBadFormatMessage);

	// TODO: ask if needed
	// When game is over, prints the relevant output file.
	void ReportGameOverToFile(Winner winner, const std::string& gameOverMessage, bool ifToPrintBoard = true) const;

	// Reports the winner.
	void ReportGameOver(Winner winner){
		mWinner = winner;
	}

	// TODO: maybe pass a boolean, if to check tie.
	// Returns true iff all the pieces of one of the players or for both are eaten.
	bool ReportAllMovingPiecesEaten();

	// Returns true iff the flags are captured (both flags or one)
	// or all the pieces for both players are eaten.
	bool ReportGameOverAfterInitBoard();

	// Note that if this player index is 1 than the other player index is 0 and vice versa.
	// Index is 0 based
	int GetOpponentIndex(int playerIndex) const { return 1 - playerIndex; }

	// Position on the board the initial pieces of the players, according to their algorithm.
	bool HandlePositioning();

	// Tries to get a move from algorithm and perform it. If game over, returns nullptr.
	// Else, returns the current move of the given player.
	unique_ptr<Move> CheckGetMove(int playerIndex, FightInfoImpl& fightToFill);

	// Notify the given other player about the current move and fight that occured.
	void NotifyOtherPlayer(int otherPlayerIndex, const FightInfoImpl& fightToFill, const Move& move) const;

	// Play the game by making the players' moves.
	void HandleMoves();

	// Get the winner object according to loser player's number.
	Winner GetWinnerOutOfLooser(int loserNum) const;

	// Change a joker of the given player according to the information in jokerChange.
	bool ChangeJokerRepresentation(const JokerChange& jokerChange, int playerNum);

	// Log the current board after move to file.
	//void LogAfterMove(int countMoves);

public:
    enum class Winner { Tie = 0, Player1 = 1, Player2 = 2, None = 3 };

	Game(unique_ptr<PlayerAlgorithm> player1Algorithm, unique_ptr<PlayerAlgorithm> player2Algorithm);

	void RunGame();

	Winner GetWinner() const{
		return mWinner;
	}
};
#endif //ADTO_TARGIL1_GAME_H
