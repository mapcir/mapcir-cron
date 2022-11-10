#ifndef UTILS_H
	#define UTILS_H

    #include <string>
    #include <vector>
    #include <nlohmann/json.hpp>

    class Utils {
        public:
            static std::string shell_exec(std::string command, const bool inc_stderr);
            static std::string shell_exec(std::string command);

            static void setInterval(std::function<void(void)> func, unsigned int interval);
            static void sleep(unsigned int interval);
            static void waitForever();

            static std::string ltrim(std::string &s);
            static std::string rtrim(std::string &s);
            static std::string trim(std::string &s);

            static std::vector<std::string> explode(std::string delimiter, std::string s);

            static nlohmann::json json_parse(std::string raw);
            static nlohmann::json json_parse(std::string raw, bool from_file);

            static std::string jstr(nlohmann::json raw);
            static std::string jstr(nlohmann::json raw, std::string def);
            static int jint(nlohmann::json raw);
            static int jint(nlohmann::json raw, int def);
            static int jbool(nlohmann::json raw);
            static int jbool(nlohmann::json raw, bool def);

    };

#endif
