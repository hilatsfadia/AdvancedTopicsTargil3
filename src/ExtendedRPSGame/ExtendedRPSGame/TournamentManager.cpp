#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
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

void TournamentManager::run() {
    //for (auto& pair : id2factory) {
    //	const auto& id = pair.first;
    //	std::cout << id << ": ";
    //	const auto& factoryMethod = pair.second;
    //	factoryMethod()->foo();
    //}
    vector<string> ids;
    for(auto it = id2factory.begin(); it != id2factory.end(); ++it) {
        ids.push_back(it->first);
        id2fnumberOfGames[it->first] = 0;
    }

    // TODO: move it
    Game game(id2factory[ids[0]](), id2factory[ids[1]]());
    game.RunGame();

    for (string id : ids){
        vector<string> enemies(ids.begin(), ids.end());
        eraseID(enemies, id);
        std::random_shuffle(std::begin(enemies), std::end(enemies));
        int countConcatenations = GAMES_TO_PLAY / enemies.size();
        int modVal = GAMES_TO_PLAY % enemies.size();
        cout << "countConcatenations: " << countConcatenations << endl;
        cout << "modVal: " << modVal << endl;

        list<string> enemiesToPlayWith;
        for (int i = 0; i < countConcatenations; i++){
            enemiesToPlayWith.insert(enemiesToPlayWith.end(), enemies.begin(), enemies.end());
        }

        cout << "enemiesToPlayWith.size() : " << enemiesToPlayWith.size() << endl;

        //enemiesToPlayWith.insert(enemiesToPlayWith.end(), enemies.begin(), std::advance(enemies.begin(), modVal)); // TODO:

//        for (string enemy : enemiesToPlayWith) {
//
//        Game game(id2factory[id](), id2factory[enemiesToPlayWith[0]]());
//        }
    }
}

int TournamentManager::loadAlgorithms(int, const std::string& soFilesDirectory) {
    FILE *dl;   // handle to read directory
    std::string lsCommandStr = "ls " + soFilesDirectory + "*.so";  // command string to get dynamic lib names
    char inBuf[BUF_SIZE]; // input buffer for lib names
    list<void *> dl_list; // list to hold handles for dynamic libs
    list<void *>::iterator itr;
    map<std::string, std::function<std::unique_ptr<PlayerAlgorithm>()>>::iterator fitr;
    int countSOFiles = 0;

    // get the names of all the dynamic libs (.so  files) in the current dir
    dl = popen(lsCommandStr.c_str(), "r");
    if(!dl){
        perror("popen");
        return FOLDER_COULD_NOT_BE_OPENED;
    }
    void *dlib;
    char name[BUF_SIZE];
    while(fgets(inBuf, BUF_SIZE, dl)){
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

        countSOFiles++;
        // add the handle to our list // TODO: delete!
        dl_list.insert(dl_list.end(), dlib);
    }

    if (countSOFiles <= 1){
        return ALMOST_NO_ALGORITHM_REGISTERED;
    }

    cout << "stam" << endl;
    // create an array of the shape names
    for(fitr=id2factory.begin(); fitr!=id2factory.end();
        fitr++){
        cout << fitr->first << endl;
    }

    // TODO: why coredump?
    // close all the dynamic libs we opened
//    for(itr=dl_list.begin(); itr!=dl_list.end(); itr++){
//        //dlclose(*itr); // TODO: why coredump?
//    }

    return ALGORITHM_REGISTERED_SUCCESSFULLY;
}