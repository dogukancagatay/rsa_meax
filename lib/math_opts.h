/**
 * @file
 * @brief Header containing number generators.
 *
 * The header file contains the random number generator and primerizing functions.
 * The random numbers generators work with blum blum shub and GMP library functions.
 */

#ifndef MATH_OPTS_H_
#define MATH_OPTS_H_

#include <gmp.h>
#include <time.h>
#include <string.h>
#include "blumblumshub.h"
#include "bit_opts.h"

#define BINARY 2
#define DECIMAL 10
/**
 *
 * @param bit_length Bit length for the random number.
 * @return Prime random number with desired bit length.
 *
 * @brief Generates random prime numbers with bbs algorithm.
 *
 * The function computes a random number with blum blum shub with desired bit length and
 * primerize the number. The return value is a GMP number.
 */
mpz_t* create_random_prime_gmp_number_with_bbs(int bit_length){
	mpz_t* num = (mpz_t*)malloc(sizeof(mpz_t));
	char* random_bitstring;

	random_bitstring = bbs_bit_string(bit_length,BINARY);//default 512 bit

	mpz_init(*num);
	mpz_init_set_str(*num,random_bitstring,BINARY);//initialize to our
	mpz_nextprime(*num,*num); //find the next prime number greater than our random number

	free(random_bitstring);//free char buffer

	return num;
}

//mpz_t* take_mod_of_exp_number(mpz_t* base, mpz_t* power, mpz_t* mod){
//	mpz_t* f = (mpz_t*)malloc(sizeof(mpz_t));
//	mpz_t c,ftemp;
//	int i,size;
//	char* bits_of_num;
//
//	//initialize
//	//mpz_init(c);
//	//mpz_init(*f);
//	mpz_init(ftemp);
//	mpz_init_set_ui(c,0); // c = 0
//	mpz_init_set_ui(*f,1); // f = 1
//
//	//convert base to bits
//	bits_of_num = mpz_get_str(NULL,BINARY,*power);
//	if(bits_of_num == NULL){
//		printf("mpz_get_str returned NULL pointer in the take_mod_of_exp_number function");
//		exit(0);
//	}
//	//find the bit length
//	size = mpz_sizeinbase(*power,BINARY);
//
//	//do the loop
//	for(i = 0; i < size; i++){
//		mpz_mul_ui(c,c,2); // c<-(cx2)
//		mpz_pow_ui(ftemp,*f,2); // ftemp = f^2
//		mpz_mod(*f,ftemp,*mod); // take ftemp(f x f) mod n
//		if(bits_of_num[i] == '1'){
//			mpz_add_ui(c,c,1); // c<-c+1
//			mpz_mul(ftemp,*base,*f); // ftemp = f x base
//			mpz_mod(*f,ftemp,*mod); // take ftemp(f x base) mod n
//		}
//	}
//
//	//free the temporarily allocated memory
//	free(bits_of_num);
//	mpz_clear(c);
//	mpz_clear(ftemp);
//
//	return f;
//}

/**
 *
 * @param f The remaining.
 * @param base Base number
 * @param power Power of the number
 * @param mod Modulo number
 *
 * @brief Does exponentiation for the encryption process to the very large numbers.
 *
 * The function implemented with GMP functions for working easily with big numbers. It does modulo operation to
 * exponential functions quickly. Since the for loop inside the function loops for the bit length of the exponential number,
 * the function runs very fast. For example if we have 1024 bit key and we are doing the encryption, the loop only loops for 1024 times.
 * It turns out O(n) is the run time for the whole encryption.
 */
void take_mod_of_exp_number2(mpz_t f, mpz_t base, mpz_t power, mpz_t mod){
	//mpz_t* f = (mpz_t*)malloc(sizeof(mpz_t));
	mpz_t c,ftemp;
	int i,size;
	char* bits_of_num;

	//initialize
	//mpz_init(c);
	//mpz_init(*f);
	mpz_init(ftemp);
	mpz_init_set_ui(c,0); // c = 0
	mpz_init_set_ui(f,1); // f = 1

	//convert base to bits
	bits_of_num = mpz_get_str(NULL,BINARY,power);
	if(bits_of_num == NULL){
		printf("mpz_get_str returned NULL pointer in the take_mod_of_exp_number function");
		exit(0);
	}
	//find the bit length
	size = mpz_sizeinbase(power,BINARY);

	//do the loop
	for(i = 0; i < size; i++){
		mpz_mul_ui(c,c,2); // c<-(cx2)
		mpz_pow_ui(ftemp,f,2); // ftemp = f^2
		mpz_mod(f,ftemp,mod); // take ftemp(f x f) mod n
		if(bits_of_num[i] == '1'){
			mpz_add_ui(c,c,1); // c<-c+1
			mpz_mul(ftemp,base,f); // ftemp = f x base
			mpz_mod(f,ftemp,mod); // take ftemp(f x base) mod n
		}
	}

	//free the temporarily allocated memory
	free(bits_of_num);
	mpz_clear(c);
	mpz_clear(ftemp);
}
/**
 *
 * @param rop Generated random number
 * @param mod Defines the max value for the random number
 * @param seed Seed for the random number
 *
 * @brief Generates a random number with GMP Library functions.
 *
 * The function generates a random number by using GMP random number functions.
 */
void generate_gmp_rand_num(mpz_t rop,mpz_t mod,unsigned long int seed){
	//mpz_t* rand_Num = (mpz_t*)malloc(sizeof(mpz_t));

	if(seed == 0)//if seed is not specified by the user
		seed = time(NULL);

	gmp_randstate_t r_state;

	gmp_randinit_default (r_state);
	gmp_randseed_ui(r_state, seed);

	mpz_urandomm(rop,r_state,mod);

	gmp_randclear(r_state);
}


#endif /* MATH_OPTS_H_ */
