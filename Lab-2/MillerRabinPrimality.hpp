#pragma once

#include "Utils.hpp"

#include "BlumBlumShubGenerator.hpp"

bool TrialDivision();
bool MillerRabinTest(mpz_class modulo, unsigned int iterations);