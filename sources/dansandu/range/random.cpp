#include "dansandu/range/random.hpp"

#include <random>

namespace dansandu::range::random
{

int generateRandomNumber(std::uniform_int_distribution<int>& distribution)
{
    static std::mt19937 random(std::random_device{}());
    return distribution(random);
}

}
