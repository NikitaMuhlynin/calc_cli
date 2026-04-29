#pragma once

namespace calc_cli {

class Runner {
public:
    Runner() = delete;

    static int run(int argc, char** argv);

};

}