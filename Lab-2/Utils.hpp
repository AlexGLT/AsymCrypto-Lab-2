#pragma once

#include <random>
#include <ctime>

#include "boost/lexical_cast.hpp"
#include "mpir.h"
#include "mpirxx.h"

struct randomSequence
{
	unsigned int size;

	unsigned char* array;
	
	randomSequence(const unsigned int& size);

	~randomSequence();
};

static std::random_device rd;
static std::mt19937 randomizationEngine;

static std::uniform_int_distribution<unsigned short> bitRandom(0, 1);
static std::uniform_int_distribution<unsigned short> hexRandom(1, 15);
static std::uniform_int_distribution<unsigned short> ucharRandom(1, UCHAR_MAX);
static std::uniform_int_distribution<unsigned int> uintRandom(1, UINT_MAX);

char Number2Char(int number);

unsigned int randomNumberGenerator(const unsigned int& power);
mpz_class randomBigIntegerGenerator(unsigned int size);
std::string convertToString(const unsigned char* numberArray, unsigned int numberByteSize);

mpz_class bigGCD(const mpz_class& firstNumber, const mpz_class& secondNumber);
mpz_class modularInvert(const mpz_class& number, const mpz_class& modulo);
mpz_class modularSquare(const mpz_class& number, const mpz_class& modulo);
mpz_class modularPow(const mpz_class& base, const mpz_class& power, const mpz_class& modulo);