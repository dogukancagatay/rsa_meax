/**
 *
 * @file
 * @brief Blum blum shub Pseudo RNG.
 *
 * Blum Blum Shub Pseudo Random Number Generator implementation is in this header file.
 * It is used to create random numbers in the creation of RSA key pairs.
 */


#ifndef BLUMBLUMSHUB_H_
#define BLUMBLUMSHUB_H_

#include <math.h>
#include <string.h>

#define RAND_MOD_1 300
#define RAND_MOD_2 600

/**
 *
 * @param x
 * @return True(1): Prime or False(0): Not Prime
 *
 * @brief Determines if the number is prime or not.
 *
 * This function uses the deterministic primality function of Euler.
 */
int is_prime(int x){// normal integer is_prime function
	int i;
	int flag = 1;
	if(x == 1)
		return 0;
	else if(x == 2)
		return 1;
	else if((x % 2) == 0)
		return 0;

	//checks till the square root of the number
	for(i = 2; i <= sqrt(x); i++){
		if(x % i == 0){
			flag = 0;
			break;
		}
	}

	return flag;
}
/**
 *
 * @param bit_number How many bits will be generated.
 * @param mod Mod of bits.(generally 2 for creation of a bit)
 * @return Generated bit string.
 *
 * @brief Blum Blum Shub Random Number Generator
 *
 * This version of Blum blum shub generates given number of bits and puts them into a bit string.
 */
char* bbs_bit_string(int bit_number, int mod){// gives random bitstream as specified length
    unsigned long long i,n,s,temp,temp2;
    char* bit_string = (char*)malloc(bit_number*sizeof(char));

    int random_number_1 = rand() % RAND_MOD_1;
    int random_number_2 = rand() % RAND_MOD_2;

    while(1){
        if(is_prime(random_number_1) && random_number_1 % 4 == 3){
            break;
        }
        random_number_1++;
    }
    while(1){
        if(is_prime(random_number_2) && random_number_2 % 4 == 3){
            break;
        }
        random_number_2++;
    }

    n = random_number_1 * random_number_2;
    s = (random_number_1 < random_number_2 ? random_number_2 : random_number_1) + 100;

    while(1){
        if(s % random_number_1 != 0 && s % random_number_2 != 0)
            break;
        s++;
    }

    temp = (s * s) % n;
    bit_string[0] = (temp % mod) + '0'; //initial bit is set

    for(i = 1; i < bit_number; i++){
        temp2 = (temp * temp) % n;
        bit_string[i] = (temp2 % mod) + '0';
        temp = temp2;
    }

    return bit_string;
}

#endif /* BLUMBLUMSHUB_H_ */
