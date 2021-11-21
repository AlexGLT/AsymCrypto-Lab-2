#include "RSA.hpp"

mpz_class generateGreatPrimeNumber(const unsigned int& bitSize)
{
	mpz_class prime = BlumBlumShub(1, bitSize);

	while (!MillerRabinTest(prime, 100))
	{
		gmp_printf("\033[31mDamn... \033[0m%#Zx\r\n", prime);

		prime = BlumBlumShub(1, bitSize);
	}

	gmp_printf("\033[32mSuccess! \033[0m%#Zx\r\n", prime);

	printf("Now we will try to improve it.\r\n");

	for (unsigned int i = 1; true; i++)
	{
		mpz_class greatPrime = 2 * i * prime + 1;

		gmp_printf("\033[31mDamn... \033[0m%#Zx\r\n", greatPrime);

		if (MillerRabinTest(greatPrime, 100))
		{
			gmp_printf("\033[32mBig success! \033[0m%#Zx\r\n", prime);

			return greatPrime;
		}
	}
}

std::pair<mpz_class, mpz_class> GenerateKeyPair()
{
	return std::pair(generateGreatPrimeNumber(512), generateGreatPrimeNumber(512));
}

RSA::RSA(const mpz_class& p, const mpz_class& q)
{
	mpz_class n = p * q;
	mpz_class eulerFunction = (p - 1) * (q - 1);

	mpz_class e = LimitedNumberGenerator(eulerFunction);

	while (bigGCD(e, eulerFunction) != 1)
	{
		e = LimitedNumberGenerator(eulerFunction);
	}

	auto d = modularInvert(e, eulerFunction);

	this->p = p;
	this->q = q;
	this->n = n;
	this->e = e;
	this->d = d;
}

std::map<std::string, mpz_class> RSA::GetPublicKey()
{
	return std::map<std::string, mpz_class> { {"e", this->e}, { "n", this->n }};
}

mpz_class RSA::Encrypt(const mpz_class& M, const mpz_class& e, const mpz_class& n)
{
	return modularPow(M, e, n);
}

mpz_class RSA::Decrypt(const mpz_class& C)
{
	return modularPow(C, this->d, this->n);
}

std::pair<mpz_class, mpz_class> RSA::Sign(const mpz_class& M)
{
	return std::pair(M, modularPow(M, this->d, this->n));
}

bool RSA::Verify(const std::pair<mpz_class, mpz_class>& Message, const mpz_class& e, const mpz_class& n)
{
	return (Message.first == modularPow(Message.second, e, n));
}

std::pair<mpz_class, mpz_class> RSA::GenerateAuthenticityKey(const mpz_class& e_1, const mpz_class& n_1)
{
	auto k = LimitedNumberGenerator(this->n);

	while (bigGCD(k, n_1) != 1)
	{
		e = LimitedNumberGenerator(this->n);
	}

	auto k_1 = modularPow(k, e_1, n_1);
	auto S = modularPow(k, this->d, this->n);
	auto S_1 = modularPow(S, e_1, n_1);

	return std::pair(k_1, S_1);
}

bool RSA::CheckAuthenticityKey(std::pair<mpz_class, mpz_class> message, const mpz_class& e, const mpz_class& n)
{
	mpz_class k = modularPow(message.first, this->d, this->n);
	mpz_class S = modularPow(message.second, this->d, this->n);

	return (k == modularPow(S, e, n));
}