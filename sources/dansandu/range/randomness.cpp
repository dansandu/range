#include "dansandu/range/randomness.hpp"

#include <random>

namespace dansandu::range::randomness
{

std::mt19937& getRandomGenerator()
{
    static std::mt19937 generator(std::random_device{}());
    return generator;
}

}
