#ifndef ADTO_TARGIL3_TOURNAMENT_MANAGER_H
#define ADTO_TARGIL3_TOURNAMENT_MANAGER_H

//--------------------------
// TournamentManager
//--------------------------

// Creates and handles a tournament of games among playerAlgorithms which can be
// registered to the tournament, using so files.
// Reports a summary of the games results to the console

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include <map>
#include <functional>
#include <memory>
#include <list>
#include <cstring>
#include <vector>
#include <atomic>
#include <mutex>
#include <random>
#include "PlayerAlgorithm.h"

class TournamentManager
{
	// This enum is needed to the case in which a player has to play against
	// other player which has already played all his games.
	enum class AccumulateGameScores { BothPlayers = 0, Player1 = 1, Player2 = 2 };

	// Represants a Game by the players ids and for which of them to accumulate scores.
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
    std::mutex mScoresLock;
	std::atomic_size_t mGameIndex{0}; // atomic_size_t is a language typedef for std::atomic<size_t>
	std::list<void *> mDlList; // list to hold handles for dynamic libs
	std::vector<GameRepr> mGames;
	std::mt19937 mRandGen;

	// private ctor (of singleton)
	TournamentManager() {}

	// Updates asynchronically the scores of the player who played the game. 
	// The question for whom to accumulate the scores, is decided by the
	// value of the given AccumulateGameScores method.
    void handleUpdateOfScores(const GameRepr& gamePlayed, size_t player1AdditionScores, size_t player2AdditionScores);
    
	// Print the players scores after playing all their games in the tournament. 
	// The print is sorted it according to the funtion cmpPlayersInfo
	void printResults() const;
    
	// Run all the games with one thread
	void runSingleThreaded();

	// Loads a specific algorithm from the given so file name
	// If an error occurs, prints it to console
	// Return true iff the so file was loaded and the algorithm has registerd.
    bool loadAlgoritm(char* inBuf);

	// Fill in a vector of all enemies of the given player id (i.e all other players), in a shuffled manner
	void fillEnemiesInRandomOrder(const std::string& playerId, const std::vector<std::string>& allIds, std::vector<std::string>& enemiesToFill);

	// If a player has to play more games and don't have whom to play with.
	// As demanded, this player should play against other, when accumulating points only to
	// the player who hasn't played 30 games yet.
	void handleNeglectedPlayer(const std::vector<std::string>& ids);

	// Tries to create 30 games for the given player against other players
	// who haven't yet played all their 30 games.
	void createGamesForPlayer(const std::string& playerId, const std::vector<std::string>& allIds);

	// Creates a vector of all the games in the tournament 
	// (vector of GameRepr, who's against whom)
    void createGames();

	// Play a game and update the players' scores according to it's result
    void runSingleSubSimulation(const GameRepr& gameToPlay);

	// While there are uncommited games, thread should take a new game, if available, and run it
	// if no game is available, thread is done
    void runSingleSubSimulationThread();

	// Erase the given string from the given vector
	void eraseString(std::vector<std::string>& stringsVec, std::string toErase);

public:
	~TournamentManager();

	// Get the instance of TournamentManager singleton
	static TournamentManager& getTournamentManager() {
		return theTournamentManager;
	}

	void registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod) {
		// TODO: should warn if id is already registered
		mId2factory[id] = factoryMethod;
	}

	// This static function is used to order the players with the same score
    static bool cmpPlayersInfo(const std::pair<const std::string, int>  &player1Info, const std::pair<const std::string, int> &player2Info);

	// Run the tournament, given the number of threads to use
	void runMultiThreaded(size_t numThreads);

	// Loads all the so files in the given directory
	int loadAlgorithms(int, const std::string& soFilesDirectory);

	enum {ALGORITHM_REGISTERED_SUCCESSFULLY = 0, FOLDER_COULD_NOT_BE_OPENED = -1, ALMOST_NO_ALGORITHM_REGISTERED = -2};
};

#endif //ADTO_TARGIL3_TOURNAMENT_MANAGER_H
