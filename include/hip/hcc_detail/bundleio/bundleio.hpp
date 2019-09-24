#ifndef BUNDLEIO_H
#define BUNDLEIO_H

#include <string>
#include <vector>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <utility>
#include <cxxabi.h>

enum bundleError {
    SUCCESS = 0,
    INVALID_FILE = 1
};

class BundleIO {
    std::string bundleHeader =  "__CLANG_OFFLOAD_BUNDLE__";
    //std::string bundleHeader =  "ELF";
    std::string fileName;
    std::string text;
    std::vector<std::string> names;
    std::vector<std::pair<std::string, std::string>> demangledPair;
    bool isValidFile() {
        std::string tHeader = std::string(text.begin(), text.begin() + 24); // CLANG_OFFLOAD
        //std::string tHeader = std::string(text.begin() + 1, text.begin() + 4); // fPIC shared
        std::cout << "HEADER:: " << tHeader << std::endl;
        if(tHeader != bundleHeader) {
            return false;
        }
        return true;
    }
    void fillNames() {
        std::string nameBundleStr = "\n  - Name:";
        auto ploc = text.find(nameBundleStr);
        auto loc = ploc;
        while(loc != std::string::npos) {
            loc += nameBundleStr.size();
            std::string tname;
            while(loc < text.size() && text[loc] != '\n') {
                if (text[loc] != ' ' && text[loc] != '\t') {
                    tname += text[loc];
                }
                loc++;
            }
            names.push_back(tname);
            ploc = text.find(nameBundleStr, ploc+1);
            loc = ploc;
        }
    }
    void demangle() {
        fillNames();
        for(auto i: names) {
            int status;
            char *ret = abi::__cxa_demangle(i.c_str(), 0, 0, &status);
            if(ret) {
                std::string tstr;
                for(int i = 0; ret[i] !='\0' && ret[i] !='('; i++) {
                    if (ret[i] == ' ') {
                        tstr.clear();
                        continue;
                    }
                    tstr+=ret[i];
                }
                free(ret);
                demangledPair.push_back(std::make_pair(tstr, i));
            } else {
                demangledPair.push_back(std::make_pair(i, i));
            }
        }
    }
    public:
    BundleIO(const std::string & file) : fileName(file) {
        std::ifstream t(fileName);
        text.assign((std::istreambuf_iterator<char>(t)),
            std::istreambuf_iterator<char>());
    }
    void dumpNames() {
        std::cout << "Names::\n";
        for(auto i: demangledPair) {
            std::cout << i.first << " - " << i.second << std::endl;
        }
    }
    bundleError getFuncNames(std::vector<std::pair<std::string, std::string>> &v) {
        if(!isValidFile()) {
            return INVALID_FILE;
        }
        demangle();
        v = demangledPair;
        dumpNames();
        return SUCCESS;
    }
};

std::string cxxdemangle(const char* c) {
    int status;
    char* ret = abi::__cxa_demangle(c, 0, 0, &status);
    std::string s;
    if (ret) {
        s = std::string(ret);
    }
    return s;
}

#endif
