#include "stdafx.h"
#include "Game.h"
#include <string>
#include <fstream>
#include <iterator>
#include <iostream>
#include "PieceFactory.h"
#include "Piece.h"
#include "ParserInitFile.h"
#include "ParserMoveFile.h"
#include "Player.h"
#include "Joker.h"

#define MAX_MOVES 100
#define MAX_TURNS MAX_MOVES/2

using namespace std;

//bool Game::SetBadInputFileMessageWithWinner(int loserNum, Game::Winner winner, int lineNum, const char * templateBadFormatMessage)
//{
//	mWinner = winner;
//	char tmp_game_over_message[MESSAGE_MAX_LEN];
//	sprintf_s(tmp_game_over_message, templateBadFormatMessage, loserNum, lineNum);
//	this->mGameOverMessage = tmp_game_over_message;
//	return false;
//}
//
//bool Game::HandleBadPositionFilesMessageWithWinner(int lineNum1, int lineNum2)
//{
//	mWinner = Winner::Tie;
//	char tmp_game_over_message[MESSAGE_MAX_LEN];
//	sprintf_s(tmp_game_over_message, BAD_POS_BOTH_PLAYERS, lineNum1, lineNum2);
//	this->mGameOverMessage = tmp_game_over_message;
//	return false;
//}
//
//bool Game::ParseInitFiles() {
//	ParserInitFile initFileParser(this);
//	//checks that file is illegal, and if not
//	//gets file and matrix
//	bool isErrorInPlayerFile[NUM_OF_PLAYERS] = { false, false };
//
//	for (int i = 0; i < NUM_OF_PLAYERS; i++)
//	{
//		//set the pieces for player i
//		// TODO: isErrorInPlayerFile[i] = !initFileParser.ParsePlayerInitFile(mPlayers[i], GetInitializationFileName(i + 1));
//
//		if (isInputFileCannotBeOpened)
//		{
//			return false;
//		}
//
//		isErrorInPlayerFile[i] = (isErrorInPlayerFile[i] ||
//			(!mPlayers[i].DoesPosiotionedAllFlags())); // Missing Flags - Flags are not positioned according to their number
//	}
//
//	if (isErrorInPlayerFile[0] && isErrorInPlayerFile[1])
//	{
//		// Both Positioning files are analyzed at the same stage - so 
//		// if both are bad the result is 0 (no winner).
//		HandleBadPositionFilesMessageWithWinner(mProblematicLineOfPlayer[0], mProblematicLineOfPlayer[1]);
//	}
//	else if (isErrorInPlayerFile[0] && !isErrorInPlayerFile[1])
//	{
//		SetBadInputFileMessageWithWinner(1, Winner::Player2, mProblematicLineOfPlayer[0], BAD_POS_PLAYER);
//	}
//	else if (!isErrorInPlayerFile[0] && isErrorInPlayerFile[1])
//	{
//		SetBadInputFileMessageWithWinner(2, Winner::Player1, mProblematicLineOfPlayer[0], BAD_POS_PLAYER);
//	}
//
//	return true;
//}
//
//void Game::Play()
//{
//	ParserMoveFile moveFileParser(this);
//	std::vector<std::string> playersFilesNames;
//	for (int i = 0; i < NUM_OF_PLAYERS; i++)
//	{
//		playersFilesNames.push_back(GetMovesFileName(i + 1));
//	}
//
//	moveFileParser.ParsePlayersMoveFiles(mPlayers, NUM_OF_PLAYERS, playersFilesNames);
//
//	// In case input file is missing or cannot be opened, the program shall not start the game, no output file
//	// should be created.
//	// Already printed error.
//	if (isInputFileCannotBeOpened)
//	{
//		return;
//	}
//
//	// If both Moves file are done without a winner there is no winner 
//	// and the appropriate reason is selected
//	if (mWinner == Game::Winner::None)
//	{
//		this->mGameOverMessage = TIE_NO_WINNER;
//		mWinner = Game::Winner::Tie;
//	}
//
//	MakeOutputFile();
//}
//
//bool Game::InitGame() {
//	// In case input file is missing or cannot be opened, the program shall not start the game, no output file
//	// should be created.
//	// Already printed error.
//	if (!ParseInitFiles())
//	{
//		return false;
//	}
//
//	if (mWinner != Game::Winner::None)
//	{
//		// There was a bas positions file
//		MakeOutputFile();
//		return false;
//	}
//
//	GetWinnerAfterInitBoard();
//	if (mWinner != Game::Winner::None)
//	{
//		MakeOutputFile();
//		return false;
//	}
//
//	return true;
//}

bool Game::ReportGameOverAfterMove()
{
	bool isGameOver = true;

	if (mPlayers[0]->GetFlagsCount() == 0)
	{
		ReportGameOver(Winner::Player2, FLAGS_CAPTURED);
	}
	else if (mPlayers[1]->GetFlagsCount() == 0)
	{
		ReportGameOver(Winner::Player1, FLAGS_CAPTURED);
	}
	else
	{
		isGameOver = false;
	}

	return isGameOver;

	// TODO: ask Don't check moving pieces, because it is checked before move?
}
//
//Game::Winner Game::GetWinner(int loserNum) const
//{
//	if (loserNum == 1)
//	{
//		return Game::Winner::Player2;
//	}
//	else if (loserNum == 2)
//	{
//		return Game::Winner::Player1;
//	}
//
//	return Game::Winner::None;
//}

bool Game::ReportGameOverAfterInitBoard()
{
	bool isGameOver = true;
	bool noFlagsPlayer1 = (mPlayers[0]->GetFlagsCount() == 0);
	bool noFlagsPlayer2 = (mPlayers[1]->GetFlagsCount() == 0);
	bool noMovingPiecesPlayer1 = (mPlayers[0]->GetCountOfMovingPieces() == 0);
	bool noMovingPiecesPlayer2 = (mPlayers[1]->GetCountOfMovingPieces() == 0);

	if (noFlagsPlayer1 && noFlagsPlayer2)
	{
		ReportGameOver(Winner::Tie, TIE_FLAGS_EATEN);
	}
	else if (noFlagsPlayer1)
	{
		ReportGameOver(Winner::Player2, FLAGS_CAPTURED);
	}
	else if (noFlagsPlayer2)
	{
		ReportGameOver(Winner::Player1, FLAGS_CAPTURED);
	}
	else if (noMovingPiecesPlayer1 && noMovingPiecesPlayer2)
	{
		// ask
		//this->mGameOverMessage = TIE_NO_MOVING_PIECES;
		ReportGameOver(Winner::Tie, PIECES_EATEN);
	}
	// TODO: maybe not needed. (already checked).
	else if (noMovingPiecesPlayer1)
	{
		ReportGameOver(Winner::Player2, PIECES_EATEN);
	}
	else if (noMovingPiecesPlayer2)
	{
		ReportGameOver(Winner::Player1, PIECES_EATEN);
	}
	else
	{
		isGameOver = false;
	}

	return isGameOver;
}

//
////void Game::PrintUsageMessage()
////{
////	std::ostringstream stringStream;
////	stringStream << "Usage of the program must include in the running directory, those file: ";
////	for (int i = 1; i <= NUM_OF_PLAYERS; i++)
////	{
////		stringStream << GetInputFileName(i);
////		stringStream << ", ";
////		stringStream << GetMovesFileName(i);
////		stringStream << ", ";
////	}
////
////	cout << stringStream.str() << endl;
////}
//
// // TODO: fileUsage message
//void Game::PrintUsageMessage()
//{
//	//std::string usageMsg;
//	//usageMsg.reserve(1000);
//	cout << "Usage of the program must include in the running directory, those file: " +
//		GetInitializationFileName(1) + ", " + GetInitializationFileName(2) + ", " + 
//		GetMovesFileName(1) + ", " + GetMovesFileName(2) << endl;
//}

void Game::MakeOutputFile(const std::string& gameOverMessage, bool ifToPrintBoard)
{
    ofstream outFile(OUTPUT_FILE_NAME);
    outFile << "Winner: " << to_string((int)mWinner) << endl;
    outFile << "Reason: " << gameOverMessage << endl;
    outFile << endl;

	if (ifToPrintBoard)
	{
		mGameBoard.Print(outFile);
	}

    outFile.close();
}

void Game::ReportGameOver(Winner winner, const std::string & gameOverMessage, bool ifToPrintBoard)
{
	mWinner = winner;

	MakeOutputFile(gameOverMessage, ifToPrintBoard);
}

Game::Game(PlayerAlgorithm* player1Algorithm, PlayerAlgorithm* player2Algorithm)
{
	mPlayers[0] = new Player(player1Algorithm);
	mPlayers[1] = new Player(player2Algorithm);
}

Game::~Game()
{
	for (int i = 0; i < NUM_OF_PLAYERS; i++)
	{
		delete mPlayers[i];
	}
}

bool Game::PutNonJokerOnBoard(Player & player, std::unique_ptr<PiecePosition>& piecePos, BoardImpl& board)
{
	Piece* piece = PieceFactory::GetPieceFromChar(piecePos->getPiece());
	if (piece == nullptr)
	{
		cout << "PIECE_CHAR in positions file should be one of: R P S B F" << endl;
		return false;
	}

	if (!piece->InitializeOwner(&player))
	{
		cout << "A PIECE type appears in file more than its number" << endl;
		return false;
	}

	//checks if X coordinate and/or Y coordinate of one or more PIECE is not in range
	//Already printed error if any.
	return board.PutPieceOnTempPlayerBoard(piece, piecePos->getPosition());
}

bool Game::ChangeJokerActualType(Joker* joker, char cJokerRepresantation)
{
	Piece* actualPiece = PieceFactory::GetPieceFromChar(cJokerRepresantation, joker->GetOwner());
	if (actualPiece == nullptr)
	{
		cout << "PIECE_CHAR in positions file should be one of: R P S B F" << endl;
		return false;
	}

	// If not a valid PIECE for a Joker
	if (!joker->SetActualPieceType(actualPiece))
	{
		cout << "PIECE_CHAR for joker can be: R P S B" << endl;
		return false;
	}

	return true;
}

bool Game::InitJokerOwnerAndActualType(Joker* joker, char cJokerRepresantation, Player* owner)
{
	if (!joker->InitializeOwner(owner))
	{
		std::cout << "A PIECE type appears in file more than its number" << std::endl;
		return false;
	}

	if (!ChangeJokerActualType(joker, cJokerRepresantation))
	{
		// Already printed error
		return false;
	}

	return true;
}

bool Game::PutJokerOnBoard(Player& player, std::unique_ptr<PiecePosition>& piecePos, BoardImpl& board)
{
	// actualPiece shouldn't have an owner! because we don't want to 
	// count it as one of the player's pieces.
	Joker* joker = new Joker();

	if (!InitJokerOwnerAndActualType(joker, piecePos->getJokerRep(), &player))
	{
		// Already printed error.
		return false;
	}

	//checks if X coordinate and/or Y coordinate of one or more PIECE is not in range
	//Already printed error if any.
	return board.PutPieceOnTempPlayerBoard(joker, piecePos->getPosition());
}

bool Game::PutPlayerPiecesOnBoard(Player& player, std::vector<unique_ptr<PiecePosition>>& playerPiecePositions, BoardImpl& board)
{
	for (std::unique_ptr<PiecePosition>& piecePos : playerPiecePositions)
	{
		// TODO: define
		if (piecePos->getJokerRep() == '#')
		{
			return PutNonJokerOnBoard(player, piecePos, board);
		}
		else
		{
			return PutJokerOnBoard(player, piecePos, board);
		}
	}

	return false;
}

void Game::HandlePositioning()
{
	std::vector<unique_ptr<PiecePosition>> playersPiecePositions[NUM_OF_PLAYERS];

	for (int i = 0; i < NUM_OF_PLAYERS; i++)
	{
		mPlayers[i]->GetPlayerAlgorithm()->getInitialPositions(i + 1, playersPiecePositions[i]);

		// TODO: check if player didn't put piece in the same location
	}

	// Puts the pieces on two temp boards, as suggested in class,
	// in order to avoid missing an illegal case where one player places two weak pieces in
	// a location of the other player's strong piece.
	BoardImpl tempPlayersBoards[NUM_OF_PLAYERS];

	for (int i = 0; i < NUM_OF_PLAYERS; i++)
	{
		if (!PutPlayerPiecesOnBoard(*mPlayers[i], playersPiecePositions[i], tempPlayersBoards[i]))
		{
			// TODO:
			return;
		}
	}

	std::vector<unique_ptr<FightInfo>> fights;

	// Perform all fights on the initial positions
	mGameBoard.InitByTempBoards(tempPlayersBoards[0], tempPlayersBoards[1], fights);

	for (int i = 0; i < NUM_OF_PLAYERS; i++)
	{
		mPlayers[i]->GetPlayerAlgorithm()->notifyOnInitialBoard(mGameBoard, fights);
	}
}

void Game::HandleMoves()
{
	// One turn consists of two moves of the two players.
	int currTurn = 1;

	while (currTurn <= MAX_TURNS)
	{
		for (int i = 0; i < NUM_OF_PLAYERS; i++)
		{
			// For this player
			unique_ptr<Move> currMove = mPlayers[i]->GetPlayerAlgorithm()->getMove();

			FightInfo* toFill = nullptr;

			// Checks if mPlayers[i] i.e player(i+1), can move. If not, he loses the game.
			if (mPlayers[i]->GetCountOfMovingPieces() == 0)
			{
				ReportGameOver((Winner)mPlayers[i]->GetPlayerNum(), PIECES_EATEN);
				return;
			}

			if (!mGameBoard.MovePiece(*mPlayers[i], currMove, toFill))
			{
				// TODO:
				//HandleGameOver();

				return;
			}

			if (ReportGameOverAfterMove())
			{
				return;
			}

			// Notify only of there was a fight
			if (toFill != nullptr)
			{
				mPlayers[i]->GetPlayerAlgorithm()->notifyFightResult(*toFill);
			}

			unique_ptr<JokerChange> currJokerChange = mPlayers[i]->GetPlayerAlgorithm()->getJokerChange();

			// TODO: change joker

			// For the other player.
			PlayerAlgorithm* opponentAlgorithm = mPlayers[GetOpponentIndex(i)]->GetPlayerAlgorithm();
			opponentAlgorithm->notifyOnOpponentMove(*currMove.get());

			// Notify only of there was a fight
			if (toFill != nullptr)
			{
				opponentAlgorithm->notifyFightResult(*toFill);
			}
		}

		currTurn++;
	}

	// TODO: ask reason
	ReportGameOver(Winner::Tie, "No fight for more than " + to_string(MAX_MOVES) + " moves");
}

void Game::RunGame()
{
	HandlePositioning();

	if (ReportGameOverAfterInitBoard())
	{
		return;
	}

	HandleMoves();
}
