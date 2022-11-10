#include <iostream>

#include "helpers/flags.h"
#include "helpers/utils.h"
#include "helpers/cron.h"

// RUN: $ make && ./mapcir-cron --f=../configs.json

int main(int argc, char *argv[]) {
	std::string appName = "mapcir-cron";
	std::string appVersion = "1.0.1";
	std::string appDescription = "Lightweight job scheduling for Linux";

	// ================================================
	// PASER FLAGS
	FlagHandler flags(appName, appVersion, appDescription);
	flags.addFlag("f", "Configs file path (.json).", "./configs.json");
	flags.paser(argc, argv);

	// ================================================
	// GET CONFIGS
	std::string debugMode = flags.getString("d");
	std::string configsFile = flags.getString("f");

	nlohmann::json configs = Utils::json_parse(configsFile, true);

	if (configs == nullptr) {
		std::cout << appName + " - error: JSON parse error!";
		exit(0);
	}
	
	// ================================================
	// RUN APPLICATION
	Cron cron(configs);
	cron.run();

	return 0;
}