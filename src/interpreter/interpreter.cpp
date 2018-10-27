#include "interpreter.hpp"
#include "common.hpp"

#include <string>
#include <stdint.h>
#include <stdexcept>

using namespace micro_turing;


uint64_t mpz_get_uint64_t(const mpz_t in)
{
    uint64_t result;
    mpz_export(&result, 0, -1, sizeof(uint64_t), 0, 0, in);
    return result;
}


void interpreter::step(bool safe)
{
    //Get instruction arguments
    mpq_class* aq = &memory[address];
    mpq_class* bq = &memory[address+1];

    mpz_class az = aq->get_num();
    mpz_class bz = bq->get_num();

    //Don't trust the input!
    if (safe)
    {
        mpz_class aqd = aq->get_den();
        mpz_class bqd = bq->get_den();

        //TODO: Proper classes for each type of error
        if (aqd != 1)               throw std::runtime_error("A address is not an integer: " + az.get_str() + "/" + aqd.get_str() + " at " + std::to_string(address) + ".");
        if (bqd != 1)               throw std::runtime_error("B address is not an integer: " + bz.get_str() + "/" + bqd.get_str() + " at " + std::to_string(address) + ".");
        if (az < 0)                 throw std::runtime_error("Negative A address " + az.get_str() + " at " + std::to_string(address) + ".");
        if (bz < 0)                 throw std::runtime_error("Negative B address " + bz.get_str() + " at " + std::to_string(address+1) + ".");
        if (az > memory.size() - 1) throw std::runtime_error("Out of bounds A address " + az.get_str() + " at " + std::to_string(address) + ".");
        if (bz > memory.size() - 1) throw std::runtime_error("Out of bounds B address " + bz.get_str() + " at " + std::to_string(address) + ".");
    }

    //Convert `a` and `b` to a uint64_t for vector purposes
    uint64_t a = mpz_get_uint64_t(az.get_mpz_t());
    uint64_t b = mpz_get_uint64_t(bz.get_mpz_t());

    //Emulate the instruction
    acc -= memory[a];
    memory[a] = acc;
    if (acc >= 0) address = b;
    else address += 2;
}
