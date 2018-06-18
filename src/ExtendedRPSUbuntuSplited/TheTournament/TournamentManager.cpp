#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <thread>
#include "TournamentManager.h"
#include "Game.h"
#include <cstdlib>
#include <ctime>

#define GAMES_TO_PLAY 30
#define WINNER_SCORES 3
#define LOOSER_SCORES 0
#define TIE_SCORES 1

// Type Alias
using Winner = Game::Winner;

using namespace std;

// size of buffer for reading in directory entries
static const unsigned int BUF_SIZE = 4096;

// define the static variable
TournamentManager TournamentManager::theTournamentManager;

void TournamentManager::eraseString(vector<string>& stringsVec, string toErase) const{
    for (auto it = stringsVec.begin(); it != stringsVec.end();) {
        if (*it == toErase) {
            it = stringsVec.erase(it);
        } else {
            ++it;
        }
    }
}

TournamentManager::~TournamentManager()
{
	mId2factory.clear();

	// close all the dynamic libs we opened
	for(auto itr=mDlList.begin(); itr!=mDlList.end(); itr++){
	    dlclose(*itr);
	}

	mDlList.clear();
}

void TournamentManager::handleUpdateOfScores(const GameRepr& gamePlayed, size_t player1AdditionScores, size_t player2AdditionScores){
	switch (gamePlayed.accumulateMethod) {
		case AccumulateGameScores::BothPlayers:
		{
			lock_guard<mutex> lock(mScoresLock);
			mId2scores[gamePlayed.player1Id] += player1AdditionScores;
			mId2scores[gamePlayed.player2Id] += player2AdditionScores;
			break;
		}
		case AccumulateGameScores::Player1:
		{
			lock_guard<mutex> lock(mScoresLock);
			mId2scores[gamePlayed.player1Id] += player1AdditionScores;
			break;
		}
		case AccumulateGameScores::Player2:
		{
			lock_guard<mutex> lock(mScoresLock);
			mId2scores[gamePlayed.player2Id] += player2AdditionScores;
			break;
		}
		default:
		{
			break;
		}
	}
}

void TournamentManager::runSingleSubSimulation(const GameRepr& gameToPlay) {
    Game game;
    game.InitGame(mId2factory[gameToPlay.player1Id](), mId2factory[gameToPlay.player2Id]());
    game.RunGame();
    Winner winner = game.GetWinner();
    switch (winner){
        case Winner::Player1:{
            handleUpdateOfScores(gameToPlay, WINNER_SCORES, LOOSER_SCORES);
            break;
        }
        case Winner::Player2:{
            handleUpdateOfScores(gameToPlay, LOOSER_SCORES, WINNER_SCORES);
            break;
        }
        case Winner::Tie:{
            handleUpdateOfScores(gameToPlay, TIE_SCORES, TIE_SCORES);
            break;
        }
        default:{
            // Shouldn't happen
            break;
        }
    }
}

void TournamentManager::runSingleThreaded() {
    for (const auto& game: mGames) {
        runSingleSubSimulation(game);
    }
}

void TournamentManager::runSingleSubSimulationThread() {
    // Thread should take a new game, if available, and run it
    // if no game is available, thread is done
    for(size_t index = mGameIndex++; // fetch old value, then add. equivalent to: fetch_add(1)
        index < mGames.size();
        index = mGameIndex++) {
        runSingleSubSimulation(mGames[index]);
    }
}

bool TournamentManager::cmpPlayersInfo(const pair<const std::string, int>  &player1Info, const pair<const std::string, int> &player2Info)
{
    if (player1Info.second == player2Info.second) {
        return player1Info.first < player2Info.first;
    }

    return player1Info.second > player2Info.second;
}

void TournamentManager::printResults() const{
    vector<pair<std::string, int> > scoresVec;
    copy(mId2scores.begin(), mId2scores.end(), back_inserter(scoresVec));
    sort(scoresVec.begin(), scoresVec.end(), TournamentManager::cmpPlayersInfo);

    for(size_t i = 0; i < scoresVec.size(); ++i) {
        cout << scoresVec[i].first << " " << scoresVec[i].second << endl;
    }
}

void TournamentManager::runMultiThreaded(size_t numThreads){
	createGames();

    if (numThreads == 1){
        runSingleThreaded();
    }
    else {
        vector<unique_ptr<thread>> threads(numThreads-1);
        for (auto &thread_ptr : threads) {
            thread_ptr = make_unique<thread>(&TournamentManager::runSingleSubSimulationThread,
                                             this); // create and run the thread
        }

        // Join all the threads to finish nicely (i.e. without crashing / terminating threads)
        for (auto &thread_ptr : threads) {
            thread_ptr->join();
        }
    }

    printResults();
}

void TournamentManager::createSpecificGame(const std::string& player1ID, const std::string& player2ID, 
	AccumulateGameScores accumulateIfPlayer1First, AccumulateGameScores accumulateIfPlayer2First) {
	int rand01 = rand() % 2;
	if (rand01 == 0) {
		mGames.emplace_back(player1ID, player2ID, accumulateIfPlayer1First);
	}
	else {
		mGames.emplace_back(player2ID, player1ID, accumulateIfPlayer2First);
	}
}

void TournamentManager::handleNeglectedPlayer(size_t playerIndex, const std::vector<std::string>& allIds)
{
	// As demanded, this player should play against other, when accumulating points only to
	// the player who hasn't played 30 games yet.
	if ((playerIndex < allIds.size()) && (allIds.size() > 2)) {
		size_t enemyIndex = 0;

		if (playerIndex == 0) {
			enemyIndex = 1;
		}

		const string& playerId = allIds.at(playerIndex);
		const string& enemyId = allIds.at(enemyIndex);
		createSpecificGame(playerId, enemyId, AccumulateGameScores::Player1, AccumulateGameScores::Player2);
	}
}

void TournamentManager::fillMatixWithUniformPartOfGamesCount(vector<vector<size_t>>& gamesMat, size_t numOfPlayers) {
	if (numOfPlayers > 0) {
		size_t uniformDistGames = GAMES_TO_PLAY / (numOfPlayers - 1);
		gamesMat.reserve(numOfPlayers);

		// Init a matrix of size numOfPlayers X numOfPlayers with zeros
		for (size_t i = 0; i < numOfPlayers; i++) {
			vector<size_t> onePlayerVec;
			onePlayerVec.reserve(numOfPlayers);
			for (size_t j = 0; j < numOfPlayers; j++) {
				if (i == j) {
					onePlayerVec.push_back(0);
				}
				else {
					onePlayerVec.push_back(uniformDistGames);
				}
			}

			gamesMat.push_back(onePlayerVec);
		}
	}
}

void TournamentManager::handleLastGameForPlayersWithNeglected(std::vector<std::vector<size_t>>& gamesMat, size_t numOfPlayers, 
	const std::vector<string>& ids, vector<bool>& isAddedOneGame)
{
	// In this case a k (reminderOfGames) regular graph doesn't exist. So one player will have to play 
	// with other who already played all his games.
	size_t hasToPlayIndex = 0;

	// Let's first add one game for each of the players except one
	for (size_t i = 0; i < numOfPlayers; i++) {
		for (size_t j = 0; (j < numOfPlayers) && (!isAddedOneGame[i]); j++) {
			if ((i != j) && (!isAddedOneGame[j])) {
				gamesMat[i][j]++;
				gamesMat[j][i]++;
				isAddedOneGame[i] = true;
				isAddedOneGame[j] = true;
			}
		}

		if (!isAddedOneGame[i]) {
			hasToPlayIndex = i;
		}
	}

	handleNeglectedPlayer(hasToPlayIndex, ids);
}

void TournamentManager::handleOddReminderPartOfGamesCount(std::vector<std::vector<size_t>>& gamesMat, size_t numOfPlayers, 
	size_t reminderOfGames, const std::vector<string>& ids)
{
	if (((reminderOfGames % 2) != 0) && (reminderOfGames > 0)) {
		// Lets first handle it for even k (reminderOfGames)
		handleEvenReminderPartOfGamesCount(gamesMat, numOfPlayers, reminderOfGames - 1);

		vector<bool> isAddedOneGame;
		isAddedOneGame.reserve(numOfPlayers);
		for (size_t i = 0; i < numOfPlayers; i++) {
			isAddedOneGame.push_back(false);
		}

		if ((numOfPlayers % 2) == 0) {
			// Connect each node to the direcly opposite one
			for (size_t i = 0; i < numOfPlayers; i++) {
				if (!isAddedOneGame[i]) {
					size_t opppositeNodeIndex = (i + (numOfPlayers / 2)) % numOfPlayers;
					gamesMat[i][opppositeNodeIndex]++;
					gamesMat[opppositeNodeIndex][i]++;
					isAddedOneGame[i] = true;
					isAddedOneGame[opppositeNodeIndex] = true;
				}
			}
		}
		else {
			handleLastGameForPlayersWithNeglected(gamesMat, numOfPlayers, ids, isAddedOneGame);
		}
	}
}

int TournamentManager::calcCyclicDistance(int vecLength, int index1, int index2)
{
	int dist1 = ((index1 - index2) + vecLength) % vecLength;
	int dist2 = ((index2 - index1) + vecLength) % vecLength;
	return min(dist1, dist2);
}

void TournamentManager::handleEvenReminderPartOfGamesCount(std::vector<std::vector<size_t>>& gamesMat, 
	size_t numOfPlayers, size_t reminderOfGames)
{
	if (((reminderOfGames % 2) == 0) && (reminderOfGames > 0)) {
		for (size_t i = 0; i < numOfPlayers; i++) {
			for (size_t j = 0; j < numOfPlayers; j++) {
				size_t closeMeasure = calcCyclicDistance(static_cast<int>(numOfPlayers), static_cast<int>(i), static_cast<int>(j));
				// Connect each node to its (reminderOfGames / 2) nearest neighbors on either side,
				// to a total of reminderOfGames neighbors.
				if ((closeMeasure > 0) && (closeMeasure <= reminderOfGames / 2)) {
					gamesMat[i][j]++;
				}
			}
		}
	}
}

void TournamentManager::fillMatixWithReminderPartOfGamesCount(vector<vector<size_t>>& gamesMat, 
	size_t numOfPlayers, const vector<string>& ids)
{
	if (numOfPlayers > 0) {
		size_t reminderOfGames = GAMES_TO_PLAY % (numOfPlayers - 1);

		// We now want to build a k regular graph on n nodes 
		// (where k = reminderOfGames, and n = numOfPlayers)
		// It is impossible for the case when k and n are both odd numbers

		// If k (reminderOfGames) is even
		if ((reminderOfGames % 2) == 0) {
			handleEvenReminderPartOfGamesCount(gamesMat, numOfPlayers, reminderOfGames);
		}
		else {
			handleOddReminderPartOfGamesCount(gamesMat, numOfPlayers, reminderOfGames, ids);
		}
	}
}

void TournamentManager::createGamesFromMat(std::vector<std::vector<size_t>>& gamesMat, size_t numOfPlayers, const vector<string>& ids)
{
	// Create games out of the matrix
	for (size_t i = 0; i < numOfPlayers; i++) {
		for (size_t j = i; j < numOfPlayers; j++) {
			size_t currCombinationNumOfGames = gamesMat[i][j];
			for (size_t k = 0; k < currCombinationNumOfGames; k++) {
				createSpecificGame(ids[i], ids[j], AccumulateGameScores::BothPlayers, AccumulateGameScores::BothPlayers);
				gamesMat[i][j]--;
				gamesMat[j][i]--;
			}
		}
	}
}

void TournamentManager::createGames() {
	srand((unsigned int)time(0)); // use current time as seed for random generator
	mGames.clear();
    list<void *>::iterator itr;
    vector<string> ids;
	if (mId2factory.size() > 0) { // Should be > 1
		size_t numOfPlayers = mId2factory.size();

		for(auto it = mId2factory.begin(); it != mId2factory.end(); it++) {
			ids.push_back(it->first);
			mId2scores[it->first] = 0;
		}

		// This matrix is symetric and has in location(i,j) the number of games between players i and j
		vector<vector<size_t>> gamesMat;
		fillMatixWithUniformPartOfGamesCount(gamesMat, numOfPlayers);
		fillMatixWithReminderPartOfGamesCount(gamesMat, numOfPlayers, ids);
		createGamesFromMat(gamesMat, numOfPlayers, ids);

		//logFile.open("log.txt");
		//for (auto game : mGames) {
		//	logFile << game.player1Id << " " << game.player2Id << endl;
		//}
	}
}

// We decided to continue the tournament (and print error to console) if we encounter an so
// file that has a problem. No other instruction about this case
bool TournamentManager::loadAlgoritm(char* inBuf){
    void *dlib;
    char name[BUF_SIZE];
	size_t oldMapSize = mId2factory.size();
	// TODO:!!!
    // trim off the whitespace
    char *ws = strpbrk(inBuf, " \t\n");
    if(ws) *ws = '\0';
    // append ./ to the front of the lib name
    sprintf(name, "%s", inBuf);
    dlib = dlopen(name, RTLD_LAZY);
    if(dlib == NULL){
        cerr << dlerror() << endl;
		cout << "so file of algorithm: " << name << " cannot be loaded" << endl;
		return false;
    }

	if (mId2factory.size() == oldMapSize)
	{
		cout << "Algorithm: " << name << " hasn't registered" << endl;
		return false;
	}

	// add the handle to our list
	mDlList.insert(mDlList.end(), dlib);

	return true;
}

int TournamentManager::loadAlgorithms(int, const std::string& soFilesDirectory) {
    FILE *dl;   // handle to read directory
	std::string soFilesDirectoryWithSlash = soFilesDirectory;

	if (soFilesDirectoryWithSlash[soFilesDirectoryWithSlash.size() - 1] != '/') {
		soFilesDirectoryWithSlash += '/';
	}

    std::string lsCommandStr = "ls " + soFilesDirectoryWithSlash + "*.so";  // command string to get dynamic lib names
    char inBuf[BUF_SIZE]; // input buffer for lib names
    map<std::string, std::function<std::unique_ptr<PlayerAlgorithm>()>>::iterator fitr;

    // get the names of all the dynamic libs (.so  files) in the current dir
    dl = popen(lsCommandStr.c_str(), "r");
    if (!dl){
        perror("popen");
        return FOLDER_COULD_NOT_BE_OPENED;
    }

	// If an error occurs in an so file, we continue loading other so files
	// (and loadAlgoritm is responsible to print the error to console)
    while (fgets(inBuf, BUF_SIZE, dl)){
		loadAlgoritm(inBuf);
    }

    if (mId2factory.size() <= 1){
        return ALMOST_NO_ALGORITHM_REGISTERED;
    }

    return ALGORITHM_REGISTERED_SUCCESSFULLY;
}