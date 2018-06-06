#ifndef __TOURNAMENT_MANAGER_H_
#define __TOURNAMENT_MANAGER_H_

#include <map>
#include <functional>
#include <memory>
#include <list>
#include <cstring>
#include <vector>
#include "PlayerAlgorithm.h"

class TournamentManager
{
	static TournamentManager theTournamentManager;
	std::map<std::string, std::function<std::unique_ptr<PlayerAlgorithm>()>> id2factory;
	std::map<std::string, int> id2fnumberOfGames;
	// private ctor
	TournamentManager() {}

	void eraseID(std::vector<std::string>& ids, std::string id);
public:
	static TournamentManager& getTournamentManager() {
		return theTournamentManager;
	}
	void registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod) {
		// TODO: should warn if id is already registered
		id2factory[id] = factoryMethod;
	}
	//int loadAlgorithm(const std::string& path, const std::string& soFileNameWithoutSuffix);
	void run();
	//void run()const;

	int loadAlgorithms(int, const std::string& soFilesDirectory);

	enum {ALGORITHM_REGISTERED_SUCCESSFULLY = 0, FOLDER_COULD_NOT_BE_OPENED = 1, SO_FILE_CANNOT_BE_LOADED = 2, ALMOST_NO_ALGORITHM_REGISTERED = 3};
};

#endif
