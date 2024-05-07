#pragma once


namespace GoL {

class Configuration {

public:
    virtual void Load() = 0;
    virtual bool operator==(const Configuration*) = 0;
};

};