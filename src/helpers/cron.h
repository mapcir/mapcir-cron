#ifndef CRON_H
	#define CRON_H

    #include <nlohmann/json.hpp>

    class Cron {
        nlohmann::json data = nullptr;

        public:
            Cron(nlohmann::json data);
            void run();
    };

#endif
