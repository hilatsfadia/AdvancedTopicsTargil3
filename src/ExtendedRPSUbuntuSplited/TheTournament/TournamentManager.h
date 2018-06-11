#ifndef __TOURNAMENT_MANAGER_H_
#define __TOURNAMENT_MANAGER_H_

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
	static TournamentManager theTournamentManager;
	std::map<std::string, std::function<std::unique_ptr<PlayerAlgorithm>()>> mId2factory;
	std::map<std::string, int> mId2numberOfGames;
	std::map<std::string, int> mId2scores;
    std::mutex scoresLock;
	std::atomic_size_t mGameIndex{0}; // atomic_size_t is a language typedef for std::atomic<size_t>
	std::list<void *> mDlList; // list to hold handles for dynamic libs
	std::vector<std::pair<std::string, std::string>> mGames;
	// private ctor
	TournamentManager() {}

    void handleUpdateOfScores(const std::pair<std::string, std::string>& gameToPlay, size_t player1AdditionScores, size_t player2AdditionScores);
    void printResults() const;
    void runSingleThreaded();
    int loadAlgoritm(char* inBuf);
    void createGames();
    void runSingleSubSimulation(const std::pair<std::string, std::string>& gameToPlay);
    void runSingleSubSimulationThread();
	void eraseID(std::vector<std::string>& ids, std::string id);
public:
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

	int loadAlgorithms(int, const std::string& soFilesDirectory);

	enum {ALGORITHM_REGISTERED_SUCCESSFULLY = 0, FOLDER_COULD_NOT_BE_OPENED = -1, SO_FILE_CANNOT_BE_LOADED = -2, ALMOST_NO_ALGORITHM_REGISTERED = -3};
};

#endif
