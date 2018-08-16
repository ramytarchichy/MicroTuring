#include "Interpreter.hpp"

#include <string>
#include <stdexcept>

using namespace MicroTuring;

void Interpreter::doStep()
{
    //Get the instruction arguments
    int64_t *a = &memory[address];
    int64_t b = memory[address+1];

    //Don't trust user input!
    if (*a < 0) throw std::runtime_error("Negative A address " + std::to_string(*a) + " at " + std::to_string(address) + ".");
    if (b < 0) throw std::runtime_error("Negative B address " + std::to_string(b) + " at " + std::to_string(address+1) + ".");
    if (*a > memory.size() - 1) throw std::runtime_error("Out of bounds A address " + std::to_string(*a) + " at " + std::to_string(address) + ".");

    //Emulate the `uSUBGEQ+` instruction
    acc -= memory[*a];
    memory[*a] = acc;
    if (acc >= 0) address = b;
    else address += 2;
}

bool Interpreter::isRunning()
{
    return address <= memory.size() - 2;
}

void Interpreter::Step()
{
    if (isRunning()) doStep();
}

void Interpreter::Run()
{
    while (isRunning()) doStep();
}
