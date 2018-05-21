#ifndef ADTO_TARGIL1_GAME_H
#define ADTO_TARGIL1_GAME_H

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "Player.h"
#include "BoardImpl.h"
#include "PlayerAlgorithm.h"

class Joker;

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

class Game
{
private:
	//PlayerAlgorithm& mPlayer1Algorithm;
	//PlayerAlgorithm& mPlayer2Algorithm;

	enum class Winner { Tie = 0, Player1 = 1, Player2 = 2, None = 3 };

	BoardImpl mGameBoard;
	Player* mPlayers[NUM_OF_PLAYERS];

	// TODO: check if needed
	Winner mWinner = Winner::None;

	//PlayerAlgorithm* mAlgorithms[NUM_OF_PLAYERS];

	//std::string mGameOverMessage = "";
	//int mProblematicLineOfPlayer[NUM_OF_PLAYERS] = { -1, -1 };
	//bool isInputFileCannotBeOpened = false;

	// Handles the parsing and execution of the positioning files,
	// while setting the winner (and relevant message) if any.
	// Returns false in case input file is missing or cannot be opened 
	bool PutPiecePositionsOnBoard(std::vector<unique_ptr<PiecePosition>>& player1PiecePositions,
		std::vector<unique_ptr<PiecePosition>>& player2PiecePositions, BoardImpl& tempPlayer1Board, BoardImpl& tempPlayer2Board);
	
	// When one of the input files (position file or move file) has bad format,
	// update message and winner.
	void SetBadInputFileMessageWithWinner(int loserNum, Winner winner, const char* templateBadFormatMessage);

	//// When both of the position files has bad format,
	//// update message and winner.
	//bool HandleBadPositionFilesMessageWithWinner(int lineNum1, int lineNum2);

	// When game is over, prints the relevant output file.
	void MakeOutputFile(const std::string& gameOverMessage, bool ifToPrintBoard = true);

	void ReportGameOver(Winner winner, const std::string & gameOverMessage, bool ifToPrintBoard = true);

	// TODO: maybe pass a boolean, if to check tie.
	// Returns true iff all the pieces of one of the players or for both are eaten.
	bool ReportAllMovingPiecesEaten();

	bool ReportGameOverAfterInitBoard();

	// Checks whether one of the players wins the game, i.e
	// captured all the flags of the opponent (one flag in Ex1)
	//bool ReportGameOverAfterMove();

	bool IsGameOver() { return mWinner != Game::Winner::None; }

	bool PutPlayerPiecesOnBoard(Player& player, std::vector<unique_ptr<PiecePosition>>& playerPiecePositions, BoardImpl& board);

	// Note that if this player index is 1 than the other player index is 0 and vice versa.
	// Index is 0 based
	int GetOpponentIndex(int playerIndex) { return 1 - playerIndex; }

	// Put a non joker piece in given position.  
	// Get detailed about positioning from the given tokens, and checks for tokens validity.
	bool PutNonJokerOnBoard(Player& player, std::unique_ptr<PiecePosition>& piecePos, BoardImpl& board);

	// Changes the joker actual type to the one represented by the given character.
	bool ChangeJokerActualType(Joker* joker, char cJokerRepresantation);

	// Initializes the joker with it's owner and actual type represented by the given character.
	bool InitJokerOwnerAndActualType(Joker* joker, char cJokerRepresantation, Player* owner);

	// Put a joker piece in given position. 
	// Get detailed about positioning from the given tokens, and checks for tokens validity.
	bool PutJokerOnBoard(Player& player, std::unique_ptr<PiecePosition>& piecePos, BoardImpl& board);

	// Position on the board the initial pieces of the players, according to their algorithm.
	bool HandlePositioning();

	// If game over, returns nullptr.
	// Else, returns the current move of the given player.
	unique_ptr<Move> CheckGetMove(int playerIndex, FightInfoImpl& fightToFill);

	// Notify the given other player about the current move and fight that occured.
	void NotifyOtherPlayer(int otherPlayerIndex, FightInfoImpl& fightToFill, Move& move);

	// Play the game by making the players' moves.
	void HandleMoves();

	// Get the winner object according to loser player's number.
	Winner GetWinner(int loserNum) const;

	// Change a joker of the given player according to the information in jokerChange.
	bool ChangeJokerRepresentation(const JokerChange& jokerChange, int playerNum);

public:
	//enum Reason {PLAYER1_FLAGS_CAPTURED, PLAYER2_FLAGS_CAPTURED, 
	//	PLAYER1_PIECES_EATEN, PLAYER2_PIECES_EATEN,
	//	TIE_NO_WINNING_MOVE, TIE_ALL_FLAGS_EATEN_IN_POSITIONS_FILES,
	//	BAD_POS_INPUT_FILE_PLAYER1, BAD_POS_INPUT_FILE_PLAYER2,
	//	BAD_POS_INPUT_FILES, BAD_MOVE_INPUT_FILE_PLAYER1, BAD_MOVE_INPUT_FILE_PLAYER2, NO_WINNER};

	Game(PlayerAlgorithm* player1Algorithm, PlayerAlgorithm* player2Algorithm);

	virtual ~Game();

	void RunGame();

	//// Init the Game by positioning the pieces on it's board, 
	//// according to the init files.
	//bool InitGame();

	//// Read The move files and play the game by making the moves, each player at a time. 
	//void Play();
};
#endif //ADTO_TARGIL1_GAME_H
