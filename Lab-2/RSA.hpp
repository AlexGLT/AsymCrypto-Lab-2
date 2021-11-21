#pragma once

#include <map>

#include "MillerRabinPrimality.hpp"

std::pair<mpz_class, mpz_class> GenerateKeyPair();

class RSA
{
private:
	mpz_class p;
	mpz_class q;
	mpz_class d;
public:
	mpz_class n;
	mpz_class e;

	RSA(const mpz_class& p, const mpz_class& q);
	std::map<std::string, mpz_class> GetPublicKey();
	mpz_class Encrypt(const mpz_class & M, const mpz_class & e, const mpz_class& n);
	mpz_class Decrypt(const mpz_class & C);
	std::pair<mpz_class, mpz_class> Sign(const mpz_class & M);
	bool Verify(const std::pair<mpz_class, mpz_class> & Message, const mpz_class& e, const mpz_class& n);
	std::pair<mpz_class, mpz_class> GenerateAuthenticityKey(const mpz_class& e, const mpz_class& n);
	bool CheckAuthenticityKey(std::pair<mpz_class, mpz_class> key, const mpz_class& e, const mpz_class& n);
};
