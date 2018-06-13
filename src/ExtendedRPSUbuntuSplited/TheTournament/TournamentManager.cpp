#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <thread>
//#include <unistd.h>
#include "TournamentManager.h"
#include "Game.h"

#define GAMES_TO_PLAY 30
#define WINNER_SCORES 3
#define LOOSER_SCORES 0
#define TIE_SCORES 1

// Type Alias
using Winner = Game::Winner;

using namespace std;

// size of buffer for reading in directory entries
static const unsigned int BUF_SIZE = 1024;

// define the static variable
TournamentManager TournamentManager::theTournamentManager;

void TournamentManager::eraseString(vector<string>& stringsVec, string toErase){
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

	// TODO: why coredump? look!
	//    for (auto fitr=mId2factory.begin(); fitr!=mId2factory.end();
	//        fitr++){
	//        fitr->second = nullptr;
	//    }
	// close all the dynamic libs we opened
	for(auto itr=mDlList.begin(); itr!=mDlList.end(); itr++){
	    dlclose(*itr);
	    //*itr = nullptr;
	}
}

void TournamentManager::handleUpdateOfScores(const std::pair<std::string, std::string>& gameToPlay, size_t player1AdditionScores, size_t player2AdditionScores){
    lock_guard<mutex> lock(scoresLock);
    mId2scores[gameToPlay.first] += player1AdditionScores;
    mId2scores[gameToPlay.second] += player2AdditionScores;
}

void TournamentManager::runSingleSubSimulation(const std::pair<std::string, std::string>& gameToPlay) {
    Game game(mId2factory[gameToPlay.first](), mId2factory[gameToPlay.second]());
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
        vector <unique_ptr<thread>> threads(numThreads-1);
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

void TournamentManager::createGames() {
    mGames.clear();
    list<void *>::iterator itr;
    vector<string> ids;
    for(auto it = mId2factory.begin(); it != mId2factory.end(); it++) {
        ids.push_back(it->first);
        mId2numberOfGames[it->first] = GAMES_TO_PLAY;
        mId2scores[it->first] = 0;
    }

	for (string id : ids) {
		vector<string> enemies(ids.begin(), ids.end());
		eraseString(enemies, id);
		std::random_shuffle(std::begin(enemies), std::end(enemies));

		for (auto itr = enemies.begin(); mId2numberOfGames[id] > 0;) {
			std::string& enemy = *itr;
			if (mId2numberOfGames[enemy] > 0) {
				mGames.push_back(std::make_pair(id, enemy)); // TODO: ask about the order
				mId2numberOfGames[id]--;
				mId2numberOfGames[enemy]--;
			}

			if (mId2numberOfGames[enemy] == 0) {
				itr = enemies.erase(itr);
				continue;
			}
			else {
				++itr;
				if (itr == enemies.end()) {
					itr = enemies.begin();
				}
			}
		}
	}
}

int TournamentManager::loadAlgoritm(char* inBuf){
    void *dlib;
    char name[BUF_SIZE];
	// TODO:!!!
    // trim off the whitespace
    char *ws = strpbrk(inBuf, " \t\n");
    if(ws) *ws = '\0';
    // append ./ to the front of the lib name
    sprintf(name, "./%s", inBuf);
    dlib = dlopen(name, RTLD_NOW);
    if(dlib == NULL){
        cerr << dlerror() << endl;
        return SO_FILE_CANNOT_BE_LOADED;
    }

    // add the handle to our list
    mDlList.insert(mDlList.end(), dlib);

    return ALGORITHM_REGISTERED_SUCCESSFULLY;
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
    if(!dl){
        perror("popen");
        return FOLDER_COULD_NOT_BE_OPENED;
    }

    while(fgets(inBuf, BUF_SIZE, dl)){
        if (loadAlgoritm(inBuf) == ALGORITHM_REGISTERED_SUCCESSFULLY){
        }
        else{
            // TODO:
        }
    }

    if (mId2factory.size() <= 1){
        return ALMOST_NO_ALGORITHM_REGISTERED;
    }

    return ALGORITHM_REGISTERED_SUCCESSFULLY;
}