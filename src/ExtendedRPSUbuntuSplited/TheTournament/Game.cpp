#include "Game.h"
#include <string>
#include "PieceFactory.h"
#include "Piece.h"
#include "FightInfoImpl.h"
#include "PiecePositionImpl.h"
#include "PlayerAlgorithm.h"
#include "Player.h"

#define MAX_MOVES 100

using namespace std;
using namespace HilaAndJaelExtendedRPS;

Game::Winner Game::GetWinnerOutOfLooser(int loserNum) const
{
	if (loserNum == 1)
	{
		return Game::Winner::Player2;
	}
	else if (loserNum == 2)
	{
		return Game::Winner::Player1;
	}

	return Game::Winner::None;
}

bool Game::ReportAllMovingPiecesEaten()
{
	bool noMovingPiecesPlayer1 = (mPlayersVec[0]->GetCountOfMovingPieces() == 0);
	bool noMovingPiecesPlayer2 = (mPlayersVec[1]->GetCountOfMovingPieces() == 0);

	// TODO: ask if also tie if moving pieces of both players are eaten in the moving stage.
	if (noMovingPiecesPlayer1 && noMovingPiecesPlayer2)
	{
		// As Written in the forum
		ReportGameOver(Winner::Tie);
		//ReportGameOverToFile(Winner::Tie, PIECES_EATEN_BOTH_PLAYERS);
		return true;
	}

	return false;
}

bool Game::ReportGameOverAfterInitBoard()
{
	bool isGameOver = true;
	bool noFlagsPlayer1 = (mPlayersVec[0]->GetFlagsCount() == 0);
	bool noFlagsPlayer2 = (mPlayersVec[1]->GetFlagsCount() == 0);

	if (noFlagsPlayer1 && noFlagsPlayer2)
	{
		ReportGameOver(Winner::Tie);
		//ReportGameOverToFile(Winner::Tie, TIE_FLAGS_EATEN);
	}
	else if (noFlagsPlayer1)
	{
		ReportGameOver(Winner::Player2);
		//ReportGameOverToFile(Winner::Player2, FLAGS_CAPTURED);
	}
	else if (noFlagsPlayer2)
	{
		ReportGameOver(Winner::Player1);
		//ReportGameOverToFile(Winner::Player1, FLAGS_CAPTURED);
	}
	else if (!ReportAllMovingPiecesEaten())
	{
		isGameOver = false;
	}

	return isGameOver;
}

void Game::ReportGameOverToFile(Winner winner, const std::string& gameOverMessage, bool ifToPrintBoard) const
{
    ofstream outFile(OUTPUT_FILE_NAME);
    outFile << "Winner: " << to_string((int)winner) << endl;
    outFile << "Reason: " << gameOverMessage << endl;

	if (ifToPrintBoard)
	{
		outFile << endl;
		mGameBoard.Print(outFile);
	}

    outFile.close();
}

void Game::InitGame(unique_ptr<PlayerAlgorithm> player1Algorithm, unique_ptr<PlayerAlgorithm> player2Algorithm)
{
	mPlayersVec.push_back(make_shared<Player>(NUM_OF_PLAYERS - 1));
	mPlayersVec.push_back(make_shared<Player>(NUM_OF_PLAYERS));
	mAlgorithmsVec.push_back(std::move(player1Algorithm));
	mAlgorithmsVec.push_back(std::move(player2Algorithm));
	mIsInitialized = true;
}

bool Game::PutNonJokerOnBoard(int playerNum, const std::unique_ptr<PiecePosition>& piecePos, BoardImpl<Piece>& board) const
{
	unique_ptr<Piece> piece = PieceFactory::GetPieceFromChar(piecePos->getPiece());
	if (piece == nullptr)
	{
		// Written in the forum to put in comment
		//cout << "PIECE_CHAR in positions file should be one of: R P S B F" << endl;
		return false;
	}

	if (!piece->InitializeOwner(mPlayersVec[playerNum]))
	{
		// Written in the forum to put in comment
		//cout << "A PIECE type appears in file more than its number" << endl;
		return false;
	}

	//checks if X coordinate and/or Y coordinate of one or more PIECE is not in range
	//Already printed error if any.
	return board.PutPieceOnSinglePlayerBoard(std::move(piece), piecePos->getPosition());
}

bool Game::ChangeJokerActualType(Joker* joker, char cJokerRepresantation) const
{
	unique_ptr<Piece> actualPiece = PieceFactory::GetPieceFromChar(cJokerRepresantation, joker->GetOwner());
	if (actualPiece == nullptr)
	{
		// Written in the forum to put in comment
		//cout << "PIECE_CHAR for joker can be: R P S B" << endl;
		return false;
	}

	// If not a valid PIECE for a Joker
	if (!joker->SetActualPiece(std::move(actualPiece)))
	{
		// Written in the forum to put in comment
		//cout << "PIECE_CHAR for joker can be: R P S B" << endl;
		return false;
	}

	return true;
}

bool Game::InitJokerOwnerAndActualType(Joker* joker, char cJokerRepresantation, int playerNum) const
{
	if (!joker->InitializeOwner(mPlayersVec[playerNum]))
	{
		// Written in the forum to put in comment
		//std::cout << "A PIECE type appears in file more than its number" << std::endl;
		return false;
	}

	if (!ChangeJokerActualType(joker, cJokerRepresantation))
	{
		// Already printed error
		return false;
	}

	return true;
}

bool Game::PutJokerOnBoard(int playerNum, const std::unique_ptr<PiecePosition>& piecePos, BoardImpl<Piece>& board) const
{
	// actualPiece shouldn't have an owner! because we don't want to 
	// count it as one of the player's pieces.
	unique_ptr<Joker> joker = make_unique<Joker>();
	// TODO: change!
	if (!InitJokerOwnerAndActualType(joker.get(), piecePos->getJokerRep(), playerNum))
	{
		// Already printed error.
		return false;
	}

	//checks if X coordinate and/or Y coordinate of one or more PIECE is not in range
	//Already printed error if any.
	return board.PutPieceOnSinglePlayerBoard(std::move(joker), piecePos->getPosition());
}

bool Game::PutPlayerPiecesOnBoard(int playerNum, const std::vector<unique_ptr<PiecePosition>>& playerPiecePositions, BoardImpl<Piece>& board) const
{
	for (const std::unique_ptr<PiecePosition>& piecePos : playerPiecePositions)
	{
		// TODO: define
		if (piecePos->getJokerRep() == NON_JOKER_REP)
		{
			if (!PutNonJokerOnBoard(playerNum, piecePos, board))
			{
				return false;
			}
		}
		else
		{
			if (!PutJokerOnBoard(playerNum, piecePos, board))
			{
				return false;
			}
		}
	}

	return true;
}

void Game::SetBadInputMessageWithWinner(int loserNum, Game::Winner winner, const char * templateBadFormatMessage)
{
	char tmp_game_over_message[MESSAGE_MAX_LEN];
	sprintf(tmp_game_over_message, templateBadFormatMessage, loserNum);
	ReportGameOver(winner);
	//ReportGameOverToFile(winner, tmp_game_over_message, ifToPrintBoard);
}

bool Game::PutPiecePositionsOnBoard(const std::vector<unique_ptr<PiecePosition>>& player1PiecePositions, 
	const std::vector<unique_ptr<PiecePosition>>& player2PiecePositions, 
	BoardImpl<Piece>& tempPlayer1Board, BoardImpl<Piece>& tempPlayer2Board) {

	bool isErrorInPlayer1Positioning = !PutPlayerPiecesOnBoard(0, player1PiecePositions, tempPlayer1Board) ||
		(!mPlayersVec[0]->DoesPosiotionedAllFlags()); // Missing Flags - Flags are not positioned according to their number

	bool isErrorInPlayer2Positioning = !PutPlayerPiecesOnBoard(1, player2PiecePositions, tempPlayer2Board) ||
		(!mPlayersVec[1]->DoesPosiotionedAllFlags()); // Missing Flags - Flags are not positioned according to their number

	// Positions invalid.
	// Position for both players are invalid
	if (isErrorInPlayer1Positioning && isErrorInPlayer2Positioning)
	{
		// Positioning for both players are analyzed at the same stage - so 
		// if both are bad the result is 0 (no winner).
		ReportGameOver(Winner::Tie);
		//ReportGameOverToFile(Winner::Tie, BAD_POS_BOTH_PLAYERS, false);
		return false;
	}
	// Position for one player is invalid
	else if (isErrorInPlayer1Positioning)
	{
		SetBadInputMessageWithWinner(1, Winner::Player2, BAD_POS_PLAYER);
		return false;
	}
	else if (isErrorInPlayer2Positioning)
	{
		SetBadInputMessageWithWinner(2, Winner::Player1, BAD_POS_PLAYER);
		return false;
	}

	return true;
}

bool Game::HandlePositioning()
{
	std::vector<unique_ptr<PiecePosition>> playersPiecePositions[NUM_OF_PLAYERS];

	for (int i = 0; i < NUM_OF_PLAYERS; i++)
	{
		mAlgorithmsVec[i]->getInitialPositions(i + 1, playersPiecePositions[i]);
	}

	// Puts the pieces on two temp boards, as suggested in class,
	// in order to avoid missing an illegal case where one player places two weak pieces in
	// the same location where the other player has strong piece.
	BoardImpl<Piece> tempPlayersBoards[NUM_OF_PLAYERS];

	if (!PutPiecePositionsOnBoard(playersPiecePositions[0], playersPiecePositions[1], tempPlayersBoards[0], tempPlayersBoards[1]))
	{
		return false;
	}

	// TODO: check tie

	std::vector<unique_ptr<FightInfo>> fights;

	// Perform all fights on the initial positions
	mGameBoard.InitByTempBoards(tempPlayersBoards[0], tempPlayersBoards[1], fights);

	for (int i = 0; i < NUM_OF_PLAYERS; i++)
	{
		mAlgorithmsVec[i]->notifyOnInitialBoard(mGameBoard, fights);
	}

	return true;
}

// Written in the forum to put printing to console in comment
bool Game::ChangeJokerRepresentation(const JokerChange& jokerChange, int playerNum)
{
	if (!mGameBoard.CheckGetPieceOfPlayer(jokerChange.getJokerChangePosition(), playerNum)){
		SetBadInputMessageWithWinner(playerNum, GetWinnerOutOfLooser(playerNum), BAD_MOVE_PLAYER);
		return false;
	}

	Piece* jokerPiece = mGameBoard.GetPiece(jokerChange.getJokerChangePosition());

	if (jokerPiece == nullptr){
		//std::cout << "The joker change is illegal because given position is illegal." << std::endl;
		SetBadInputMessageWithWinner(playerNum, GetWinnerOutOfLooser(playerNum), BAD_MOVE_PLAYER);
		return false;
	}

	Joker* joker = dynamic_cast<Joker*>(jokerPiece);
	if (joker == nullptr){
		//cout << "Joker position doesn't have a piece other than a joker" << endl;
		SetBadInputMessageWithWinner(playerNum, GetWinnerOutOfLooser(playerNum), BAD_MOVE_PLAYER);
		return false;
	}
	// Joker position that doesn't have a Joker owned by this player
	//else if (joker->GetOwner()->GetPlayerNum() != playerNum)
	//{
	//	SetBadInputFileMessageWithWinner(playerNum, GetWinnerOutOfLooser(playerNum), BAD_MOVE_PLAYER);
	//	return false;
	//}
	else if (!ChangeJokerActualType(joker, jokerChange.getJokerNewRep())){
		// Already printed error.
		SetBadInputMessageWithWinner(playerNum, GetWinnerOutOfLooser(playerNum), BAD_MOVE_PLAYER);
		return false;
	}

	return true;
}
//
//void Game::LogAfterMove(int countMoves)
//{
//	logFile << "Count Move: " << countMoves << endl;
//	logFile << "\n" << endl;
//	mGameBoard.Print(logFile);
//	logFile << "###############################################" << endl;
//	logFile << "\n" << endl;
//	logFile.flush();
//}

unique_ptr<Move> Game::CheckGetMove(int playerIndex, FightInfoImpl& fightToFill)
{
	if (mPlayersVec[playerIndex]->GetCountOfMovingPieces() == 0){
		ReportGameOver((Winner)mPlayersVec[GetOpponentIndex(playerIndex)]->GetPlayerNum());
		//ReportGameOverToFile((Winner)mPlayersVec[GetOpponentIndex(playerIndex)]->GetPlayerNum(), PIECES_EATEN_PLAYER);
		return nullptr;
	}

	unique_ptr<Move> theMove = mAlgorithmsVec[playerIndex]->getMove();

	// Written in the forum that we can return nullptr for invalid line/ end of file
	// In both cases we are allowed to refer to it as a lose.
	if (theMove == nullptr){
		int playerNum = mPlayersVec[playerIndex]->GetPlayerNum();
		SetBadInputMessageWithWinner(playerNum, GetWinnerOutOfLooser(playerNum), BAD_MOVE_PLAYER);
		return nullptr;
	}

	if (!mGameBoard.MovePiece(*mPlayersVec[playerIndex], theMove, fightToFill)){
		int playerNum = mPlayersVec[playerIndex]->GetPlayerNum();
		SetBadInputMessageWithWinner(playerNum, GetWinnerOutOfLooser(playerNum), BAD_MOVE_PLAYER);
		return nullptr;
	}

	// If the player who just moved ate the last flag of the oponnent, he wins the game.
	// Note that the flags number of the current player haven't changed.
	if (mPlayersVec[GetOpponentIndex(playerIndex)]->GetFlagsCount() == 0){
		ReportGameOver((Winner)mPlayersVec[playerIndex]->GetPlayerNum());
		//ReportGameOverToFile((Winner)mPlayersVec[playerIndex]->GetPlayerNum(), FLAGS_CAPTURED);
		return nullptr;
	}

	return theMove;
}

void Game::NotifyOtherPlayer(int otherPlayerIndex, const FightInfoImpl& fightToFill, const Move& move) const
{
	// For the other player.
	PlayerAlgorithm& opponentAlgorithm = *mAlgorithmsVec[otherPlayerIndex];
	opponentAlgorithm.notifyOnOpponentMove(move);

	// Notify only of there was a fight
	if (fightToFill.isInitialized())
	{
		opponentAlgorithm.notifyFightResult(fightToFill);
	}
}

void Game::HandleMoves(){
	// One turn consists of two moves of the two players.
	int countNoFightMoves = 0;
	//int countMoves = 1;// TODO: delete!!!

	while (countNoFightMoves < MAX_MOVES)
	{
		for (int i = 0; i < NUM_OF_PLAYERS; i++)
		{
			FightInfoImpl fightToFill;
			unique_ptr<Move> currMove = CheckGetMove(i, fightToFill);
			if (currMove == nullptr){ // if Game is over
				return;
			}

			// Notify only of there was a fight
			if (fightToFill.isInitialized()){ // There was fight
				mAlgorithmsVec[i]->notifyFightResult(fightToFill);
				countNoFightMoves = 0;
			}
			else{ // There was no fight
				countNoFightMoves++;
			}

			//LogAfterMove(countMoves);// TODO: delete!!!
			unique_ptr<JokerChange> currJokerChange = mAlgorithmsVec[i]->getJokerChange();
			if (currJokerChange != nullptr){ // if a change is requested
				if (!ChangeJokerRepresentation(*currJokerChange, i + 1)){
					return;
				}
			}

			NotifyOtherPlayer(GetOpponentIndex(i), fightToFill, *currMove);
		}

		//countMoves++;// TODO: delete!!!
	}

	ReportGameOver(Winner::Tie);
	//ReportGameOverToFile(Winner::Tie, TIE_NO_FIGHTS);
}

void Game::RunGame()
{
	if (mIsInitialized) {
		if (!HandlePositioning())
		{
			return;
		}

		// TODO: delete!!!
		//logFile.open("log.txt");
		//mGameBoard.Print(logFile);
		//logFile << "###############################################" << endl;
		//logFile << "\n\n\n" << endl;
		//logFile.flush();

		if (ReportGameOverAfterInitBoard())
		{
			return;
		}

		HandleMoves();
	}

}
