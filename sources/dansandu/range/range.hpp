#pragma once

#include "dansandu/range/concatenate.hpp"
#include "dansandu/range/consume.hpp"
#include "dansandu/range/cycle.hpp"
#include "dansandu/range/filter.hpp"
#include "dansandu/range/fold.hpp"
#include "dansandu/range/integers.hpp"
#include "dansandu/range/map.hpp"
#include "dansandu/range/pipe.hpp"
#include "dansandu/range/random.hpp"
#include "dansandu/range/repeat.hpp"
#include "dansandu/range/shuffle.hpp"
#include "dansandu/range/take.hpp"
#include "dansandu/range/to_vector.hpp"
#include "dansandu/range/zip.hpp"

namespace dansandu::range::range
{

using dansandu::range::concatenate::concatenate;
using dansandu::range::consume::consume;
using dansandu::range::filter::filter;
using dansandu::range::fold::fold;
using dansandu::range::integers::integers;
using dansandu::range::map::map;
using dansandu::range::repeat::repeat;
using dansandu::range::pipe::operator|;
using dansandu::range::cycle::cycle;
using dansandu::range::random::random;
using dansandu::range::shuffle::shuffle;
using dansandu::range::take::take;
using dansandu::range::to_vector::toVector;
using dansandu::range::zip::zip;

}
