#ifndef __TOURNAMENT_MANAGER_H_
#define __TOURNAMENT_MANAGER_H_

//--------------------------
// TournamentManager
//--------------------------

// Creates and handles all instances of piece of type flag. 

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include <map>
#include <functional>
#include <memory>
#include <list>
#include <cstring>
#include <vector>
#include <atomic>
#include <mutex>
#include "PlayerAlgorithm.h"

class TournamentManager
{
	// This enum is needed to the case in which a player has to play against
	// other player which has already played all his games.
	enum class AccumulateGameScores { BothPlayers = 0, Player1 = 1, Player2 = 2 };

	struct GameRepr {
		std::string player1Id;
		std::string player2Id;
		AccumulateGameScores accumulateMethod;
		GameRepr(const std::string& thePlayer1Id, const std::string& thePlayer2Id, AccumulateGameScores theAccumulateMethod) :
			player1Id(thePlayer1Id), player2Id(thePlayer2Id), accumulateMethod(theAccumulateMethod) {}
	};

	static TournamentManager theTournamentManager;
	std::map<std::string, std::function<std::unique_ptr<PlayerAlgorithm>()>> mId2factory;
	std::map<std::string, int> mId2numberOfGames;
	std::map<std::string, size_t> mId2scores;
    std::mutex scoresLock;
	std::atomic_size_t mGameIndex{0}; // atomic_size_t is a language typedef for std::atomic<size_t>
	std::list<void *> mDlList; // list to hold handles for dynamic libs
	std::vector<GameRepr> mGames;
	// private ctor (of singleton)
	TournamentManager() {}

	// Updates asynchronically the scores of the player who played the game. 
	// The question for whom to accumulate the scores, is decided by the
	// value of the given AccumulateGameScores method.
    void handleUpdateOfScores(const GameRepr& gamePlayed, size_t player1AdditionScores, size_t player2AdditionScores);
    void printResults() const;
    void runSingleThreaded();

	// Loads a specific algorithm from the given so file name
    int loadAlgoritm(char* inBuf);

	void FillEnemiesInRandomOrder(const std::string& playerId, const std::vector<std::string>& allIds, std::vector<std::string>& enemiesToFill);

	// Tries to create 30 games for the given player against other players
	// who haven't yet played all their 30 games.
	void createGamesForPlayer(const std::string& playerId, const std::vector<std::string>& allIds);

	// Creates a vector of all the games in the tournament 
	// (vector of pairs, who's against whom)
    void createGames();
    void runSingleSubSimulation(const GameRepr& gameToPlay);
    void runSingleSubSimulationThread();

	// Erase the given string from the given vector
	void eraseString(std::vector<std::string>& stringsVec, std::string toErase);

public:
	~TournamentManager();
	static TournamentManager& getTournamentManager() {
		return theTournamentManager;
	}
	void registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod) {
		// TODO: should warn if id is already registered
		mId2factory[id] = factoryMethod;
	}

    static bool cmpPlayersInfo(const std::pair<const std::string, int>  &player1Info, const std::pair<const std::string, int> &player2Info);

	void runMultiThreaded(size_t numThreads);

	//void run()const;

	// Loads all the so files in the given directory
	int loadAlgorithms(int, const std::string& soFilesDirectory);

	enum {ALGORITHM_REGISTERED_SUCCESSFULLY = 0, FOLDER_COULD_NOT_BE_OPENED = -1, SO_FILE_CANNOT_BE_LOADED = -2, NO_ALGORITHM_REGISTERED = -3, ALMOST_NO_ALGORITHM_REGISTERED = -4};
};

#endif
