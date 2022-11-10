#include <thread>
#include <sys/wait.h>
#include <unistd.h>
#include <regex>
#include <fstream>

#include "utils.h"

/**
 * excute a command then get output
 */
std::string Utils::shell_exec(std::string command, const bool inc_stderr = false) {
    int stdout_fds[2];
    int p = pipe(stdout_fds);

    int stderr_fds[2];
    if (!inc_stderr) {
        p = pipe(stderr_fds);
    }

    const pid_t pid = fork();

    if (!pid) {
        close(stdout_fds[0]);
        dup2(stdout_fds[1], 1);
        if (inc_stderr) {
            dup2(stdout_fds[1], 2);
        }

        close(stdout_fds[1]);

        if (!inc_stderr) {
            close(stderr_fds[0]);
            dup2(stderr_fds[1], 2);
            close(stderr_fds[1]);
        }

        std::vector<std::string> args = explode(" ", command);
        int size = args.size();
        char *vc[size+1];
        vc[size] = NULL;
        for (int i=0; i<size; i++) {
            vc[i] = (char*) args[i].c_str();
        }

        execvp(vc[0], vc);
        exit(0);
    }

    close(stdout_fds[1]);

    std::string out;
    const int buf_size = 4096;
    char buffer[buf_size];
    do {
        const ssize_t r = read(stdout_fds[0], buffer, buf_size);
        if(r > 0) {
            out.append(buffer, r);
        }
    } while (errno == EAGAIN || errno == EINTR);

    close(stdout_fds[0]);

    if (!inc_stderr) {
        close(stderr_fds[1]);
        do {
            ssize_t s = read(stderr_fds[0], buffer, buf_size);
        } while (errno == EAGAIN || errno == EINTR);

        close(stderr_fds[0]);
    }

    int r, status;
    do {
        r = waitpid(pid, &status, 0);
    } while (r == -1 && errno == EINTR);

    return out;
}

std::string Utils::shell_exec(std::string command) {
    return shell_exec(command, false);
}

/**
 * set interval
 */
void Utils::setInterval(std::function<void(void)> callback, unsigned int interval) {
	std::thread([callback, interval]() { 
		while (true) {
            sleep(interval);
            callback();
		}
	}).detach();
}

/**
 * sleep
 */
void Utils::sleep(unsigned int interval) {
    // std::this_thread::sleep_for(std::chrono::milliseconds(interval));
    auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(interval);
    std::this_thread::sleep_until(x);
}

/**
 * wait forever (1000 years)
 */
void Utils::waitForever() {
    // wait 1000 years
	int years = 1000;
	do {
		years--;

        // 1 year
		int wait = 365*86400;
		sleep(wait);
	} while(years > 0);
}

/**
 * left trim string
 */
std::string Utils::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    return s;
}

/**
 * right trim string
 */
std::string Utils::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
    return s;
}

/**
 * trim string
 */
std::string Utils::trim(std::string &s) {
    Utils::ltrim(s);
    Utils::rtrim(s);

    return s;
}

/**
 * split string
 */
std::vector<std::string> Utils::explode(std::string delimiter, std::string s) {
    std::vector<std::string> res;

    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    std::string v = s.substr(pos_start);
    res.push_back(trim(v));

    return res;
}

/**
 * json parse
 */
nlohmann::json Utils::json_parse(std::string raw) {
    nlohmann::json res = nlohmann::json::parse(raw, nullptr, false);
    if (res.is_discarded()) {
        return nullptr;
    }

    return res;
}

nlohmann::json Utils::json_parse(std::string raw, bool from_file) {
    if (!from_file) {
        return json_parse(raw);
    }

	std::ifstream f(raw);

	if (f.fail()) {
		return nullptr;
	}

	std::string jsonConfigs((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

    return json_parse(jsonConfigs);
}

/**
 * get nlohmann::json string
 */
std::string Utils::jstr(nlohmann::json raw, std::string def) {
    if (raw == nullptr) {
        return def;
    }

    if (raw.is_string()) {
        return raw.get<std::string>();
    }

    if (raw.is_number_integer()) {
        return std::to_string(raw.get<int>());
    }

    if (raw.is_number_float()) {
        return std::to_string(raw.get<float>());
    }

    if (raw.is_number_unsigned()) {
        return std::to_string(raw.get<unsigned>());
    }

    if (raw.is_boolean()) {
        return raw.get<bool>() ? "true" : "false";
    }

    return def;
}
std::string Utils::jstr(nlohmann::json raw) {
    return jstr(raw, "");
}

/**
 * get nlohmann::json int
 */
int Utils::jint(nlohmann::json raw, int def) {
    if (raw == nullptr) {
        return def;
    }

    if (raw.is_number_integer()) {
        return raw.get<int>();
    }

    if (raw.is_string()) {
        std::string v = raw.get<std::string>();
        if (std::regex_match(v, std::regex("^[0-9]+$"))) {
            return std::stoi(v);
        }
    }

    return def;
}
int Utils::jint(nlohmann::json raw) {
    return jint(raw, 0);
}

/**
 * get nlohmann::json bool
 */
int Utils::jbool(nlohmann::json raw, bool def) {
    if (raw == nullptr) {
        return def;
    }

    if (raw.is_boolean()) {
        return raw.get<bool>();
    }

    if (raw.is_string()) {
        std::string v = raw.get<std::string>();
        return std::regex_match(v, std::regex("^true$", std::regex_constants::icase));
    }

    return def;
}
int Utils::jbool(nlohmann::json raw) {
    return jbool(raw, false);
}
