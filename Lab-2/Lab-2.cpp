#include <iostream>

#include "RSA.hpp"

int main()
{
	auto AlicePair = GenerateKeyPair();

	auto rsa = new RSA(AlicePair.first, AlicePair.second);

	auto AlicePublicKey = rsa->GetPublicKey();

	std::string temp;

	printf("\n");

	std::map<std::string, mpz_class> BobPublicKey = {};

	std::cout << "Give n for Bob's RSA:" << std::endl;
	std::cin >> temp;
	BobPublicKey["n"] = mpz_class(temp, 16);

	std::cout << "Give e for Bob's RSA:" << std::endl;
	std::cin >> temp;
	BobPublicKey["e"] = mpz_class(temp, 16);

	printf("\n");

	printf("\033[32mAlice RSA:\033[0m\n");
	gmp_printf("\033[96mAlice p: \033[0m\n%#Zx\n", AlicePair.first.get_mpz_t());
	gmp_printf("\033[96mAlice q: \033[0m\n%#Zx\n", AlicePair.second);
	gmp_printf("\033[96mAlice n: \033[0m\n%#Zx\n", AlicePublicKey["n"]);
	gmp_printf("\033[96mAlice e: \033[0m\n%#Zx\n", AlicePublicKey["e"]);
	gmp_printf("\033[96mAlice d: \033[0m\n%#Zx\n\n", modularInvert(AlicePublicKey["e"], AlicePublicKey["n"]));

	printf("\033[32mBob RSA:\033[0m\n");
	gmp_printf("\033[96mBob n: \033[0m\n%#Zx\n", BobPublicKey["n"]);
	gmp_printf("\033[96mBob e: \033[0m\n%#Zx\n\n", BobPublicKey["e"]);

	auto message = BlumBlumShub(1, 512);

	printf("\033[32mMessage:\033[0m\n");
	gmp_printf("%#Zx\n\n", message);

	printf("\033[32mEncryption and decryption:\033[0m\n");

	auto EncryptedMessageByBobPublicKey = rsa->Encrypt(message, BobPublicKey["e"], BobPublicKey["n"]);
	printf("\033[96mEncrypted message by Bob RSA:\033[0m\n");
	gmp_printf("%#Zx\n\n", EncryptedMessageByBobPublicKey);

	std::cout << "Give encrypted message by AlicePublicKey:" << std::endl;
	std::cin >> temp;
	auto EncryptedMessageByAlicePublicKey = mpz_class(temp, 16);

	printf("\n");

	auto DecryptedMessageByAlicePublicKey = rsa->Decrypt(EncryptedMessageByAlicePublicKey);
	printf("\033[96mDecrypted message by AliceRSA:\033[0m\n");
	gmp_printf("%#Zx\n\n", DecryptedMessageByAlicePublicKey);

	printf("\033[32mMessage sign:\033[0m\n");

	auto AliceSignedMessage = rsa->Sign(message);
	auto BobSignedMessage = std::pair<mpz_class, mpz_class>(message, 0);

	std::cout << "Give sign of message by Bob's RSA:" << std::endl;
	std::cin >> temp;
	BobSignedMessage.second = mpz_class(temp, 16);

	printf("\n");

	auto BobVerifiedAliceSign = rsa->Verify(AliceSignedMessage, AlicePublicKey["e"], AlicePublicKey["n"]);

	printf("\033[96mAlice sign:\033[0m\n");
	gmp_printf("%#Zx\n", AliceSignedMessage.second);
	printf("\033[95mSign verified\033[0m = %s\n", BobVerifiedAliceSign ? "\033[32mtrue\033[0m" : "\033[31mfalse\033[0m");

	auto AliceVerifiedBobSign = rsa->Verify(BobSignedMessage, BobPublicKey["e"], BobPublicKey["n"]);

	printf("\033[96mBob sign:\033[0m\n");
	gmp_printf("%#Zx\n", BobSignedMessage.second);
	printf("\033[95mSign verified\033[0m = %s\n\n", AliceVerifiedBobSign ? "\033[32mtrue\033[0m" : "\033[31mfalse\033[0m");

	printf("\033[32mAuthenticity checking:\033[0m\n");

	auto AliceGeneratedKey = rsa->GenerateAuthenticityKey(BobPublicKey["e"], BobPublicKey["n"]);

	printf("\033[96mAlice generated key:\033[0m\n");
	printf("  1. k_1: ");
	gmp_printf("%#Zx\n", AliceGeneratedKey.first);
	printf("  2. S_1: ");
	gmp_printf("%#Zx\n", AliceGeneratedKey.second);

	auto AliceAuthenticityCheck = rsa->CheckAuthenticityKey(AliceGeneratedKey, AlicePublicKey["e"], AlicePublicKey["n"]);

	printf("\n");

	std::pair<mpz_class, mpz_class> BobGeneratedKey;

	std::cout << "Give first part of Bob authenticity key:" << std::endl;
	std::cin >> temp;
	BobGeneratedKey.first = mpz_class(temp, 16);

	std::cout << "Give second part of Bob authenticity key:" << std::endl;
	std::cin >> temp;
	BobGeneratedKey.second = mpz_class(temp, 16);

	printf("\n");

	printf("\033[96mBob generated key:\033[0m\n");
	printf("  1. k_1: ");
	gmp_printf("%#Zx\n", BobGeneratedKey.first);
	printf("  2. S_1: ");
	gmp_printf("%#Zx\n", BobGeneratedKey.second);

	auto BobAuthenticityCheck = rsa->CheckAuthenticityKey(BobGeneratedKey, BobPublicKey["e"], BobPublicKey["n"]);

	printf("\033[95mAuthenticity verified\033[0m = %s\n\n", BobAuthenticityCheck ? "\033[32mtrue\033[0m" : "\033[31mfalse\033[0m");

}
