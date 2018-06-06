#include "Game.h"
#include <iostream>
#include "RSPPlayer_309962264.h"
#include <memory>
//#include "TournamentManager.h"

#define COMMAND_LINE_THREADS_OPTIONAL_PARAMETER "-threads"
#define COMMAND_LINE_PATH_OPTIONAL_PARAMETER "-path"
#define COMMAND_LINE_THREADS_OPTIONAL_PARAMETER_USAGE COMMAND_LINE_THREADS_OPTIONAL_PARAMETER " <num_threads>"
#define COMMAND_LINE_PATH_OPTIONAL_PARAMETER_USAGE COMMAND_LINE_PATH_OPTIONAL_PARAMETER " <location_of_algorithms>"
#define COMMAND_LINE_PARAMETER_ARGC 2
#define DEFAULT_NUM_OF_THREADS 4
#define DEFAULT_SO_FILES_DIRECTORY "./" //working directory

using std::string;
using std::cout;
using std::endl;
using std::make_unique;

void PrintUsage()
{
	cout << "Usage of the program: you can optionally give the following two parameters in any order: "
		<< COMMAND_LINE_THREADS_OPTIONAL_PARAMETER_USAGE
		<< " " << COMMAND_LINE_PATH_OPTIONAL_PARAMETER_USAGE << endl;
}

bool UpdateOptionalParameter(int& numOfThreadsToUpdate, string& soFilesDirectoryToUpdate, string parameter, string value)
{
	if (parameter == COMMAND_LINE_THREADS_OPTIONAL_PARAMETER) {
		try
		{
			numOfThreadsToUpdate = stoi(value);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
			return false;
		}
	}
	else if (parameter == COMMAND_LINE_PATH_OPTIONAL_PARAMETER) {
		soFilesDirectoryToUpdate = value;
	}
	else{
		return false;
	}

	return true;
}

bool UpdateCommandLineParameters(int& numOfThreadsToUpdate, string& soFilesDirectoryToUpdate, int argc, char *argv[]){
	if (argc == 3) { // One command line parameter
		if (!UpdateOptionalParameter(numOfThreadsToUpdate, soFilesDirectoryToUpdate, argv[1], argv[2])) {
			return false;
		}
	}
	else if (argc == 5) { // Two command line parameters
		if (argv[1] == argv[3]) {
			return false;
		}
		else {
			if ((!UpdateOptionalParameter(numOfThreadsToUpdate, soFilesDirectoryToUpdate, argv[1], argv[2])) ||
				(!UpdateOptionalParameter(numOfThreadsToUpdate, soFilesDirectoryToUpdate, argv[3], argv[4]))) {
				return false;
			}
		}
	}
	else if (argc != 1) { // Number of parameters is illegal
		return false;
	}

	return true;
}

int main(int argc, char *argv[])
{
	int numOfThreads = DEFAULT_NUM_OF_THREADS;
	string soFilesDirectory = DEFAULT_SO_FILES_DIRECTORY;

	if (!UpdateCommandLineParameters(numOfThreads, soFilesDirectory, argc, argv))
	{
		PrintUsage();
		return 1;
	}

	//TournamentManager::getTournamentManager().run();

    return 0;
}
