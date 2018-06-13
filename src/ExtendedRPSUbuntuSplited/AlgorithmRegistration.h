#ifndef __ALGORITHM_REGISTRATION_H_
#define __ALGORITHM_REGISTRATION_H_

//--------------------------
// Algorithm Registration
//--------------------------

// This class represants the way to register algorithms to the tournament.

// @author Hila Tsfadia, Jael Fafner TAU - Advanced Topics in Programming - 2018 Semester B

#include <functional>
#include <memory>

#include "PlayerAlgorithm.h"

class AlgorithmRegistration {
public:
	AlgorithmRegistration(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()>);
};

#define REGISTER_ALGORITHM(ID) \
AlgorithmRegistration register_me_##ID \
	(#ID, []{return std::make_unique<RSPPlayer_##ID>();} );

#endif