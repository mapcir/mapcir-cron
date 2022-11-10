#ifndef FLAGS_H
	#define FLAGS_H

    #include <string>
    #include <vector>

    class FlagModel {
        std::string name;
        std::string des;
        std::string value;
        std::string defaultVal;

        public:
            void setDefaultValue(std::string name, std::string des, std::string def);
            void setDefaultValue(std::string name, std::string des, int def);

            void setValue(std::string val);
            std::string getValue();

            std::string getName();
            std::string getDescription();
            std::string getDefaultValue();
    };

    class FlagHandler {
        std::string name;
        std::string version;
        std::string description;
        
        std::vector<FlagModel> dict;
        int maxOptLen = 0;

        public:
            FlagHandler(std::string name, std::string version, std::string description);

            void addFlag(std::string name, std::string des, std::string def);
            void addFlag(std::string name, std::string des, int def);

            FlagModel getFlag(std::string opt);

            std::string getString(std::string name);
            int getInt(std::string name);

            void paser(int argc, char *argv[]);

        private:
            void setMaxOptionLen(std::string opt);
            void showHelp();
            void showVersion();
    };

#endif
