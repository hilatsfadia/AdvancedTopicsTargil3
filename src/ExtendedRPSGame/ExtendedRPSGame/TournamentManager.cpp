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

using namespace std;

// size of buffer for reading in directory entries
static const unsigned int BUF_SIZE = 1024;

// define the static variable
TournamentManager TournamentManager::theTournamentManager;

void TournamentManager::eraseID(vector<string>& ids, string id){
    for (auto it = ids.begin(); it != ids.end();) {
        if (*it == id) {
            it = ids.erase(it);
        } else {
            ++it;
        }
    }
}

void TournamentManager::runSingleSubSimulation(const std::pair<std::string, std::string>& gameToPlay) {
    Game game(id2factory[gameToPlay.first](), id2factory[gameToPlay.second]());
    game.RunGame();
}

void TournamentManager::runSingleSubSimulationThread() {
    // Thread should take a new game, if available, and run it
    // if no game is available, thread is done
    for(size_t index = gameIndex++; // fetch old value, then add. equivalent to: fetch_add(1)
        index < games.size();
        index = gameIndex++) {
        runSingleSubSimulation(games[index]);
    }
}
//
//void run() {
//    for(const auto& game: games) {
//        runSingleSubSimulation(game);
//    }
//}


void TournamentManager::runMultiThreaded(size_t numThreads){
    vector<unique_ptr<thread>> threads(numThreads);
    for(auto& thread_ptr : threads) {
        thread_ptr = make_unique<thread>(&TournamentManager::runSingleSubSimulationThread, this); // create and run the thread
    }

    // Join all the threads to finish nicely (i.e. without crashing / terminating threads)
    for(auto& thread_ptr : threads) {
        thread_ptr->join();
    }
}

void TournamentManager::createGames() {
    games.clear();
    list<void *>::iterator itr;
    vector<string> ids;
    for(auto it = id2factory.begin(); it != id2factory.end(); ++it) {
        ids.push_back(it->first);
        id2numberOfGames[it->first] = GAMES_TO_PLAY;
    }

    // TODO: move it
//    Game game(id2factory[ids[0]](), id2factory[ids[1]]());
//    game.RunGame();

    for (string id : ids){
        vector<string> enemies(ids.begin(), ids.end());
        eraseID(enemies, id);
        std::random_shuffle(std::begin(enemies), std::end(enemies));

        for(auto itr = enemies.begin(); id2numberOfGames[id] > 0 ;){
            std::string& enemy = *itr;
            if (id2numberOfGames[enemy] > 0) {
                    games.push_back(std::make_pair(id, enemy)); // TODO: ask about the order
                    id2numberOfGames[id]--;
                    id2numberOfGames[enemy]--;
            }
            itr++;
            if (itr == enemies.end()){
                itr = enemies.begin();
            }
        }

//        int countConcatenations = id2numberOfGames[id] / enemies.size();
//        int modVal = id2numberOfGames[id] % enemies.size();
//        cout << "countConcatenations: " << countConcatenations << endl;
//        cout << "modVal: " << modVal << endl;
//        for (int i = 0; i < countConcatenations; i++){
//            for (auto enemy : enemies){
//                if (id2numberOfGames[enemy] > 0) {
//                    games.push_back(std::make_pair(id, enemy)); // TODO: ask about the order
//                    id2numberOfGames[id]--;
//                    id2numberOfGames[enemy]--;
//                }
//            }
//        }
//        int countModAdditions = 0;
//        for(auto itr=enemies.begin(); (itr!=enemies.end() && countModAdditions < modVal) ; itr++){
//            games.push_back(std::make_pair(id, *itr));
//            id2numberOfGames[id]--;
//            id2numberOfGames[itr]--;
//            countModAdditions++;
//        }
//        for (string enemy : enemiesToPlayWith) {
//        Game game(id2factory[id](), id2factory[enemiesToPlayWith[0]]());
//        }
    }

    // TODO: why coredump?
//    for (auto fitr=id2factory.begin(); fitr!=id2factory.end();
//        fitr++){
//        fitr->second = nullptr;
//    }
//    // close all the dynamic libs we opened
//    for(itr=dlList.begin(); itr!=dlList.end(); itr++){
//        dlclose(*itr);
//        *itr = nullptr;
//    }
}

int TournamentManager::loadAlgoritm(char* inBuf){
    void *dlib;
    char name[BUF_SIZE];

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
    dlList.insert(dlList.end(), dlib);

    return ALGORITHM_REGISTERED_SUCCESSFULLY;
}

int TournamentManager::loadAlgorithms(int, const std::string& soFilesDirectory) {
    FILE *dl;   // handle to read directory
    std::string lsCommandStr = "ls " + soFilesDirectory + "*.so";  // command string to get dynamic lib names
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

    if (id2factory.size() <= 1){
        return ALMOST_NO_ALGORITHM_REGISTERED;
    }

    //cout << "stam" << endl;
    // create an array of the shape names
    for(fitr=id2factory.begin(); fitr!=id2factory.end();
        fitr++){
        cout << fitr->first << endl;
    }

    return ALGORITHM_REGISTERED_SUCCESSFULLY;
}