/**
 * @file
 * @brief The header containing the RSA key generators.
 *
 * Header file contains the functions and structures needed for generating public and private
 * RSA keys.
 */

#ifndef RSA_OPTS_H_
#define RSA_OPTS_H_

#include <stdlib.h>
#include <gmp.h>
#include "math_opts.h"

/**
 * Default bit length for p and q.
 */
#define PQBITLENGTH 512

/**
 * @struct RSA_KEY_BASE
 * @brief RSA_KEY_BASE holds the numbers necessary for the creation of public and private keys.
 */
typedef struct RSA_KEY_BASE {
	mpz_t p; // p as GMP integer
	mpz_t q; // q as GMP integer
	mpz_t phi; // phi as GMP integer
	mpz_t n; // n as GMP integer
}rsa_key_base;
/**
 * @struct RSA_KEYS
 * @brief RSA_KEYS used to hold public and private key together. It is needed because in the creation process the 2 keys are created sequentially
 * so we needed this structure.
 */
typedef struct RSA_KEYS {
	mpz_t pu;// Public key as GMP integer
	mpz_t pr;// Private key as GMP integer
	mpz_t n; // n as GMP integer

}rsa_keys;
/**
 * @struct RSA_KEY
 * @brief RSA_KEY is general key holder for public or private key.
 */
typedef struct RSA_KEY {
	mpz_t k; // e(public) or d(private) key as GMP integer
	mpz_t n; // n as GMP integer
}rsa_key;

/**
 * @param key_length Indicates the key length for p and q
 * @return The base for the public and private keys.
 *
 * @brief Generates key base(p and q), for generation of public and private keys.
 *
 * The function generates big prime numbers p and q with the help of blum blum shub RNG with the desired bit length.
 * Then calculates n and phi according to formulas and packs all calculated values in a rsa_key_base structure.
 */
rsa_key_base* generate_rsa_key_base(int key_length){
	rsa_key_base* key_base = (rsa_key_base*)malloc(sizeof(rsa_key_base));

	if(key_length == 0){
		key_length = PQBITLENGTH;
	}
	else{
		key_length /= 2;
	}
	mpz_t* temp;

	//initialize gmp numbers
	mpz_init(key_base->p);
	mpz_init(key_base->q);
	mpz_init(key_base->n);
	mpz_init(key_base->phi);

	// generate random prime numbers p and q with bit length PQBITLENGTH
	temp = create_random_prime_gmp_number_with_bbs(key_length);
	mpz_set(key_base->p,*temp);
	free(temp);
	temp = create_random_prime_gmp_number_with_bbs(key_length);
	mpz_set(key_base->q,*temp);
	free(temp);

	///n = p x q
	mpz_mul(key_base->n,key_base->p,key_base->q);

	///phi = (p-1) x (q-1) = p x q - p - q + 1
	mpz_mul(key_base->phi,key_base->p,key_base->q);//p x q
	mpz_sub(key_base->phi,key_base->phi,key_base->p); // (p x q) - p
	mpz_sub(key_base->phi,key_base->phi,key_base->q); // ((p x q) - p) -q
	mpz_add_ui(key_base->phi,key_base->phi,1); // (((p x q) - p) -q) + 1

	return key_base;
}
/**
 *
 * @param key_base The calculated p and q values.
 * @return Public and Private keys.
 *
 * @brief Computes public and private keys according to the given rsa_key_base.
 *
 * Although its name is create_pub_key the function generate both public and private keys.
 * The public and private keys are computed via GMP Library functions since we are dealing with big
 * numbers. The e is the public key's part and generated randomly. And d is generated according to e by inverse modular operation.
 */
rsa_keys* create_pub_key(rsa_key_base* key_base){
	rsa_keys* keys = (rsa_keys*)malloc(sizeof(rsa_keys));
	mpz_t e,d,temp;
	unsigned long int seed = time(NULL);

	//initialize gmp numbers to 300
	mpz_init_set_ui(e,300);
	mpz_init_set_ui(d,300);
	mpz_init(temp);

	mpz_init(keys->pr);
	mpz_init(keys->pu);
	mpz_init(keys->n);

	/* determine a random number less than phi, e, and gcd(e,phi) = 1
	 * then conduct a modular inversion operation to e in order to calculate d
	 * if e is not modularly invertable find another e value and calculate d again
	 */
	do {
		do {
			generate_gmp_rand_num(e,key_base->phi,seed++); //a random number mod phi
			mpz_gcd(temp,e,key_base->phi);// take the gcd(e,phi)

			while(mpz_cmp_ui(temp,1) != 0){// if gcd(e,phi) != 1 change e
				mpz_sub_ui(e,e,1);// each time check is not satisfy our condition subtract 1
				mpz_gcd(temp,e,key_base->phi);// take the gcd(e,phi)
			}
		}while(mpz_cmp_ui(e,1) == 0);// if e == 1 generate another random number with another seed
	}while(mpz_invert(temp,e,key_base->phi) == 0); //if e is not invertable do the same operations again

	mpz_set(d,temp); // both keys are generated at this point

	//set keys
	mpz_set(keys->pu,e);
	mpz_set(keys->pr,d);
	mpz_set(keys->n,key_base->n);


	mpz_clear(e);
	mpz_clear(d);
	mpz_clear(temp);
	return keys;
}


#endif /* RSA_OPTS_H_ */
