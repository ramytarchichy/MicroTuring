#pragma once

#include <stdint.h>
#include <vector>

/* We don't need to make a class in this case, but it can make implementing other things such as
 * debuggers easier down the line, if I have time left. If you're reading this then I probably
 * didn't have enough time left. That or I forgot to remove this.
 */

namespace MicroTuring
{
    class Interpreter
    {
    public:

        //Internal state/registers
        std::vector<int64_t> memory = {};
        int64_t acc = 0;
        int64_t address = 0;

        void Step();
        void Run();

        bool isRunning();

    private:
        void doStep();
    };
}
