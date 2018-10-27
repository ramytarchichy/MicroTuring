#pragma once

#include <gmpxx.h>
#include <vector>

namespace micro_turing
{
    class interpreter
    {
    public:
        //Internal state/registers
        std::vector<mpq_class> memory = {};
        mpq_class acc = 0;
        uint64_t address = 0;

        void step(bool safe);
    };
}
