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

//#define MAX_MOVES 100
#define MAX_TURNS 50

using namespace std;
//
//Game::Game()
//{
//}

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
//
//Game::Winner Game::CheckGameOverAfterMove(){
//
//	if (mPlayers[0].GetFlagsCount() == 0)
//	{
//		this->mGameOverMessage = FLAGS_CAPTURED;
//		mWinner = Winner::Player2;
//	}
//	else if (mPlayers[1].GetFlagsCount() == 0)
//	{
//		this->mGameOverMessage = FLAGS_CAPTURED;
//		mWinner = Winner::Player1;
//	}
//
//	// ask: Don't check moving pieces, because it is checked before move?
//
//	return mWinner;
//}
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
//
//Game::Winner Game::GetWinnerAfterInitBoard()
//{
//	bool noFlagsPlayer1 = (mPlayers[0].GetFlagsCount() == 0);
//	bool noFlagsPlayer2 = (mPlayers[1].GetFlagsCount() == 0);
//	bool noMovingPiecesPlayer1 = (mPlayers[0].GetCountOfMovingPieces() == 0);
//	bool noMovingPiecesPlayer2 = (mPlayers[1].GetCountOfMovingPieces() == 0);
//
//	if (noFlagsPlayer1 && noFlagsPlayer2)
//	{
//		this->mGameOverMessage = TIE_FLAGS_EATEN;
//		mWinner = Winner::Tie;
//	}
//	else if (noFlagsPlayer1)
//	{
//		this->mGameOverMessage = FLAGS_CAPTURED;
//		mWinner = Winner::Player2;
//	}
//	else if (noFlagsPlayer2)
//	{
//		this->mGameOverMessage = FLAGS_CAPTURED;
//		mWinner = Winner::Player1;
//	}
//	else if (noMovingPiecesPlayer1 && noMovingPiecesPlayer2)
//	{
//		// ask
//		//this->mGameOverMessage = TIE_NO_MOVING_PIECES;
//		this->mGameOverMessage = PIECES_EATEN;
//		mWinner = Winner::Tie;
//	}
//	else if (noMovingPiecesPlayer1)
//	{
//		this->mGameOverMessage = PIECES_EATEN;
//		mWinner = Winner::Player2;
//	}
//	else if (noMovingPiecesPlayer2)
//	{
//		this->mGameOverMessage = PIECES_EATEN;
//		mWinner = Winner::Player1;
//	}
//
//	return mWinner;
//}
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
//
//void Game::MakeOutputFile()
//{
//    ofstream outFile(OUTPUT_FILE_NAME);
//    outFile << "Winner: " << to_string((int)mWinner) << endl;
//    outFile << "Reason: " << this->mGameOverMessage << endl;
//    outFile << endl;
//
//	mGameBoard.Print(outFile);
//
//    outFile.close();
//}

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

bool Game::MakeMove(const Player& player, unique_ptr<Move>& move, FightInfo* toFill)
{
	if (!mGameBoard.MovePiece(player, move, toFill))
	{
		// Already printed error.
		// TODO: SetBadInputFileMessageWithWinner(playerNum, mGame->GetWinner(playerNum), lineNum, BAD_MOVE_PLAYER);
		return false;
	}
	// TODO: 
	//if (CheckGameOverAfterMove() != Game::Winner::None)
	//{
	//	// The input is ok, no need to print anything.
	//	// TODO: return false;
	//}

	return true;
}

bool Game::HandlePositioning()
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
			return false;
		}
	}

	std::vector<unique_ptr<FightInfo>> fights;

	// Perform all fights on the initial positions
	mGameBoard.InitByTempBoards(tempPlayersBoards[0], tempPlayersBoards[1], fights);

	for (int i = 0; i < NUM_OF_PLAYERS; i++)
	{
		mPlayers[i]->GetPlayerAlgorithm()->notifyOnInitialBoard(mGameBoard, fights);
	}

	return true;
}

bool Game::HandleMoves()
{
	int countTurnes = 0;

	while (countTurnes < MAX_TURNS)
	{
		for (int i = 0; i < NUM_OF_PLAYERS; i++)
		{
			// For this player
			unique_ptr<Move> currMove = mPlayers[i]->GetPlayerAlgorithm()->getMove();

			FightInfo* toFill = nullptr;

			// Checks if mPlayers[i] i.e player(i+1), can move. If not, he loses the game.
			if (mPlayers[i]->GetCountOfMovingPieces() == 0)
			{
				// TODO: mGame->mGameOverMessage = PIECES_EATEN;
				// TODO: mGame->mWinner = Game::Winner::Player2;
				return false;
			}

			if (!MakeMove(*mPlayers[i], currMove, toFill))
			{
				// TODO:
			}

			// Notify only of there was a fight
			if (toFill != nullptr)
			{
				mPlayers[i]->GetPlayerAlgorithm()->notifyFightResult(*toFill);
			}

			unique_ptr<JokerChange> currJokerChange = mPlayers[i]->GetPlayerAlgorithm()->getJokerChange();

			// TODO: change joker


			// For the other player.
			// Note that if this player index is 1 than the other player index is 0 and vice versa.
			mPlayers[1 - i]->GetPlayerAlgorithm()->notifyOnOpponentMove(*currMove.get());

			// Notify only of there was a fight
			if (toFill != nullptr)
			{
				mPlayers[1 - i]->GetPlayerAlgorithm()->notifyFightResult(*toFill);
			}
		}

		countTurnes++;
	}

	return true;
}

void Game::RunGame()
{
	HandlePositioning();

	HandleMoves();

	// TODO: tie
}
