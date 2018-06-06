#ifndef __TOURNAMENT_MANAGER_H_
#define __TOURNAMENT_MANAGER_H_

#include <map>
#include <functional>
#include <memory>
#include "PlayerAlgorithm.h"

class TournamentManager
{
	static TournamentManager theTournamentManager;
	std::map<std::string, std::function<std::unique_ptr<PlayerAlgorithm>()>> id2factory;
	// private ctor
	TournamentManager() {}
public:
	static TournamentManager& getTournamentManager() {
		return theTournamentManager;
	}
	void registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod) {
		// TODO: should warn if id is already registered
		id2factory[id] = factoryMethod;
	}
	//int loadAlgorithm(const std::string& path, const std::string& soFileNameWithoutSuffix);
	void run()const {
		//for (auto& pair : id2factory) {
		//	const auto& id = pair.first;
		//	std::cout << id << ": ";
		//	const auto& factoryMethod = pair.second;
		//	factoryMethod()->foo();
		//}
	}
};

#endif
