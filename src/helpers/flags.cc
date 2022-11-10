#include <iostream>
#include <regex>

#include "flags.h"

void FlagModel::setDefaultValue(std::string name, std::string des, std::string def) {
    this->name = name;
    this->des = des;
    this->defaultVal = def;
}

void FlagModel::setDefaultValue(std::string name, std::string des, int def) {
    this->name = name;
    this->des = des;
    this->defaultVal = std::to_string(def);
}

void FlagModel::setValue(std::string val) {
    value = val;
}

std::string FlagModel::getValue() {
    if (value.length() < 1) {
        return defaultVal;
    }

    return value;
}

std::string FlagModel::getName() {
    return name;
}

std::string FlagModel::getDescription() {
    return des;
}

std::string FlagModel::getDefaultValue() {
    return defaultVal;
}

FlagHandler::FlagHandler(std::string name, std::string version, std::string description) {
    this->version = version;
    this->name = name;
    this->description = description;
}

void FlagHandler::addFlag(std::string name, std::string des, std::string def) {
    FlagModel f;
    f.setDefaultValue(name, des, def);
    this->dict.push_back(f);

    // set max option length
    setMaxOptionLen(name);
}

void FlagHandler::addFlag(std::string name, std::string des, int def) {
    FlagModel f;
    f.setDefaultValue(name, des, def);
    this->dict.push_back(f);

    // set max option length
    setMaxOptionLen(name);
}

void FlagHandler::setMaxOptionLen(std::string opt) {
    int len = opt.length();
    if (len > this->maxOptLen) {
        this->maxOptLen = len;
    }
}

FlagModel FlagHandler::getFlag(std::string opt) {
    FlagModel res;

    for (FlagModel &d : this->dict) {
        if (opt.compare(d.getName()) == 0) {
            return d;
        }
    }

    return res;
}

std::string FlagHandler::getString(std::string opt) {
    FlagModel d = getFlag(opt);
    return d.getValue();
}

int FlagHandler::getInt(std::string opt) {
    FlagModel d = getFlag(opt);
    return atoi(d.getValue().c_str());
}

void FlagHandler::paser(int argc, char *argv[]) {
    if (argc < 2) {
        return;
    }

    std::regex regexp("--([a-zA-Z0-9-_]+)(=(.*))?");
    std::string opt;
    for (int k = 1; k < argc; k++) {
        opt = argv[k];

        if (opt == "--help") {
            showHelp();
            exit(0);
            return;
        }

        if (opt == "--version" || opt == "-v") {
            showVersion();
            exit(0);
            return;
        }

        std::smatch m;
        std::regex_search(opt, m, regexp);

        for (FlagModel &d : this->dict) {
            if (m[1].length() > 0 && m[1].compare(d.getName()) == 0) {
                d.setValue(m[3]);
                opt = "OK";
                break;
            }
        }
    }

    if (opt != "OK") {
        std::cout << this->name + " error: Unknow argument " << opt << "\n";
        std::cout << this->name + " error: Run '" + this->name + " --help' for all supported options.\n";

        exit(0);
        return;
    }
}

void FlagHandler::showHelp() {
    std::cout << "OVERVIEW: " + this->description + "\n";
    std::cout << "USAGE: " + this->name + " [options]\n";
    std::cout << "OPTIONS:\n";

    int tab = 4;
    int maxLen = maxOptLen + tab;
    int len = this->dict.size();
    for (int i = 0; i < len; i++) {
        FlagModel d = this->dict.at(i);
        std::string name = d.getName();
        std::string des = d.getDescription();
        int oLen = name.length();
        std::cout << "  --" << name << std::string(maxLen - oLen, ' ') << des << "\n";

        std::string defaultVal = d.getDefaultValue();
        if (defaultVal.length() > 0) {
            std::cout << std::string(maxLen + tab, ' ') << "(default: " << defaultVal << ")\n\n";
        }
    }

    exit(0);
}

void FlagHandler::showVersion() {
    std::cout << "version: " << this->version << "\n";
}