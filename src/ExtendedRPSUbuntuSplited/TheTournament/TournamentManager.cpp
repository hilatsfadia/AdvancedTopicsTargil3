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

void TournamentManager::fillEnemiesInRandomOrder(const std::string & playerId, const vector<string>& ids, std::vector<std::string>& enemiesToFill)
{
	enemiesToFill = ids;
	eraseString(enemiesToFill, playerId);

	std::shuffle(enemiesToFill.begin(), enemiesToFill.end(), mRandGen);
}

void TournamentManager::removeFinishedFromVector(std::vector<std::string>& idsVec) const {
	for (auto idsItr = idsVec.begin(); idsItr < idsVec.end();) {
		if (mId2numberOfGames.at(*idsItr) == 0) {
			idsItr = idsVec.erase(idsItr);
			continue;
		}
		else {
			++idsItr;
		}
	}
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

void TournamentManager::handleNeglectedPlayer(const std::string& id, const std::vector<std::string>& allIds)
{
	// Checks if a player has to play more games and don't have whom to play with.
	// As demanded, this player should play against other, when accumulating points only to
	// the player who hasn't played 30 games yet.
	if (mId2numberOfGames[id] > 0) {
		vector<string> enemies;
		fillEnemiesInRandomOrder(id, allIds, enemies);
		size_t i = 0;
		while (mId2numberOfGames[id] > 0) {
			createSpecificGame(id, enemies[i], AccumulateGameScores::Player1, AccumulateGameScores::Player2);
			mId2numberOfGames[id]--;
			(i == enemies.size() - 1) ? i = 0 : i++;
		}
	}
}

void TournamentManager::createGamesForPlayers(vector<string>& hasToPlayIds){
	size_t playerIdsSize = hasToPlayIds.size();
	while (playerIdsSize >= 2) {
		for (auto idsItr = hasToPlayIds.begin(); idsItr < hasToPlayIds.end();){
			if (mId2numberOfGames[*idsItr] > 0) { // it can be 0 because it could be an enemy previously
				vector<string> enemies;
				fillEnemiesInRandomOrder(*idsItr, hasToPlayIds, enemies);
				for (auto enemy : enemies) { // Try to create games for current *idsItr and it's enemies
					if (mId2numberOfGames[*idsItr] == 0) {
						break;
					}
					if ((mId2numberOfGames[enemy] > 0) && (mId2numberOfGames[*idsItr] > 0)) {
						createSpecificGame(*idsItr, enemy, AccumulateGameScores::BothPlayers, AccumulateGameScores::BothPlayers);
						mId2numberOfGames[*idsItr]--;
						mId2numberOfGames[enemy]--;
					}
				}
			}

			if (mId2numberOfGames.at(*idsItr) == 0) {
				idsItr = hasToPlayIds.erase(idsItr);
				continue;
			}
			else {
				++idsItr;
			}
		}

		removeFinishedFromVector(hasToPlayIds);
		playerIdsSize = hasToPlayIds.size();
		std::shuffle(hasToPlayIds.begin(), hasToPlayIds.end(), mRandGen);
	}
}

void TournamentManager::createGames() {
	try {
		std::random_device rd;
		mRandGen.seed(rd());
	}
	catch (...) {
		// If there was a problem, don't seed the random gen.
	}

	srand((unsigned int)time(0)); // use current time as seed for random generator
	mGames.clear();
    list<void *>::iterator itr;
    vector<string> ids;
    for(auto it = mId2factory.begin(); it != mId2factory.end(); it++) {
		ids.push_back(it->first);
        mId2numberOfGames[it->first] = GAMES_TO_PLAY;
        mId2scores[it->first] = 0;
    }

	vector<string> hasToPlayIds = ids;
	createGamesForPlayers(hasToPlayIds);

	//cout << "hasToPlayIds.size() " << hasToPlayIds.size() << endl;
	// hasToPlayIds size is now 1
	handleNeglectedPlayer(hasToPlayIds.front(), ids);

	//logFile.open("log.txt");
	//for (auto game : mGames) {
	//	logFile << game.player1Id << " " << game.player2Id << endl;
	//}
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
    sprintf(name, "./%s", inBuf);
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