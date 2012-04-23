/**
 * @file
 * @brief Main function for the creation of RSA Keys.
 *
 * The file contains the instruction for creating a pair of RSA keys.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../lib/blumblumshub.h"
#include "../lib/math_opts.h"
#include "../lib/rsa_opts.h"
#include "../lib/general_opts.h"
#include "../lib/bit_opts.h"

int main(int argc,char** argv) {
	int key_length = 1024;
	if(argc != 3 && argc != 2){
		printf("Usage : ./create_rsa_keys username (key_bit_length) \nDefault key length is 1024bit");
		exit(0);
	}

	if(argc == 3){
		key_length = atoi(argv[2]);
	}

	srand(time(NULL));
	rsa_key_base* key_base = generate_rsa_key_base(key_length);
	rsa_keys* keys = create_pub_key(key_base);

//	gmp_printf("p = %Zd\n",key_base->p);
//	gmp_printf("q = %Zd\n",key_base->q);
//	gmp_printf("n = %Zd\n",key_base->n);
//	gmp_printf("phi = %Zd\n\n",key_base->phi);
//	gmp_printf("pu = %Zd\n",keys->pu);
//	gmp_printf("pu %ld bits\n",mpz_sizeinbase(keys->pu,2));
//	gmp_printf("pr = %Zd\n",keys->pr);
//	gmp_printf("pr %ld bits\n\n",mpz_sizeinbase(keys->pr,2));

	write_public_key_to_file(keys,argv[1]);
	write_private_key_to_file(keys,argv[1]);

	printf("%dbit RSA keys for the %s user is generated.\n",key_length,argv[1]);

	free(key_base);
	free(keys);
	return EXIT_SUCCESS;
}
