#ifndef BASE_H
#define BASE_H

#include <string>

class Base {
    public:
        Base() {}
        virtual bool execute(int in, int out) = 0;
        virtual bool getInfo(std::string& file) = 0;
};

#endif