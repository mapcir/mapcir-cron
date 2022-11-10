#include <iostream>

#include "utils.h"
#include "cron.h"

Cron::Cron(nlohmann::json data) {
	if (data == nullptr || !data.is_array()) {
		data = nlohmann::json::array();
	}

    this->data = data;

    // register
	for (int i=0; i<this->data.size(); i++) {
		this->data[i]["start"] = false;
		this->data[i]["running"] = false;
		std::string cmd = Utils::jstr(this->data[i]["cmd"]);
		int interval = Utils::jint(this->data[i]["interval"], 0);
		bool debug = Utils::jbool(this->data[i]["debug"], false);
		if (cmd.length() < 1 || interval <= 0) {
			continue;
		}

		Utils::setInterval([&, i, cmd, debug](){
			if (this->data[i]["start"] && !this->data[i]["running"]) {
				this->data[i]["running"] = true;
				std::string s = Utils::shell_exec(cmd);
				if (debug) {
					std::cout << s << "\n";
				}
				this->data[i]["running"] = false;
			}
		}, interval);
	}
}

/**
 * start run cronjob
 */
void Cron::run() {
	// set state
	for (int i=0; i<this->data.size(); i++) {
		this->data[i]["start"] = true;
	}

	// WAIT FOREVER
	Utils::waitForever();
}