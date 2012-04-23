/**
 * @file
 * @brief The receiver's main file.
 *
 * The file contains the main function for the receiver to authenticate the received message.
 */

#include <stdio.h>
#include <stdlib.h>
#include "../lib/blumblumshub.h"
#include "../lib/math_opts.h"
#include "../lib/rsa_opts.h"
#include "../lib/general_opts.h"
#include "../lib/bit_opts.h"

/**
 * @mainpage RSA Message Encryption and Authentication with X-509
 *
 * @section intro6 Licence
 *
 * Dogukan Cagatay 2010 @ Izmir University of Economics
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * 
 * The program basicly implements RSA key creation, encryption, decryption and authentication. It has 3 modules key creation, message encryption and message authentication.
 *
 * @section intro Implementation and Libraries
 * The only externally used library is GMP (GNU Mathematical Precision) Library. It is a library which makes it possible to maintain and use big numbers
 * in the code. So most of the mathematical computation, number generation, exponentiation, encryption, decryption, key generation etc. functions are written
 * using the GMP Library functions. The GMP library can be obtained from @link http://www.gmplib.org @endlink . The Secure Hash Algorithm implementation used in
 * the project is taken from another open source project Aescrypt @link http://www.aescrypt.com @endlink . The hash that SHA generates, is 256 bits.
 *
 * @section intro2 Compiling The Project
 * There are 3 parts in the project that has to be compiled separately. First file is create_rsa_keys.c file. Second file is sender.c file and the third
 * is receiver.c file. To be able to compile them we need to install the GMP library from the package manager of your Linux/Unix distribution or download
 * and install from its web site @link http://www.gmplib.com @endlink . To compile the program;
 * @code
 * gcc -O2 file.c -o file -lm -lgmp
 * @endcode
 *
 * After the compilation you can run according to the description below.
 *
 * @section intro3 Running The Programs
 * @subsection sb1 Creating Keys
 * As we said before the project consists of 3 parts. First part is the key generation, you generate keys with a given user name. User names are required
 * to distinguish the generated keys because each user is creating his own key. Throughout the program the user names are not important, they are just used for
 * the naming of the generated key files. In order to be able to run the create_rsa_keys.c you need 2 arguments. First is the user name, the second is the bit
 * length for the keys. An example to run the program would be like this;
 * @code
 * ./create_rsa_keys dogukan 1024
 * @endcode
 * It would create 2 files with names "username_public_key.txt" and "username_private_key.txt". These are the public and private key files that will be used while
 * running the other 2 C program. we need 2 pairs of RSA keys since we assume we have 2 users. A pair of 1024 bit default sender and receiver key files are put in the related
 * directories.
 * @subsection sb2 Creating Message to send
 * The second part is the part we do the encryption using the generated keys. This time we need to give 3 arguments.First is the message file in our case it is the id.txt
 * in the directory, the second argument is the sender's private key that is going to be used in creation of digital signature. And the third argument is the receivers
 * public key. Example run command should be like;
 * @code
 * ./send_message input_message_file sender's_private_key receiver's_public_key
 * @endcode
 * After running the program it creates a msg_to_send file in the same directory it is the file that will be sent to the receiver.
 * @subsection sb3 Authenticating the received message
 * The final, third part is the authenticating the received encrypted message. The authenticate_msg.c file also takes 3 arguments. First is the received message file,
 * the second is the receiver's private key file and the third is the sender's public key file. Example run would be like;
 * @code
 * ./authenticate_msg received_msg_file receiver's_private_key_file sender's_public_key_file
 * @endcode
 *
 *
 *
 *
 */

int main(int argc,char** argv) {
	rsa_key *r_pr, *s_pu;
	char *received_msg, *decrypted_msg, *id,*ds,*sender_hash,*receiver_hash;
	int id_size;

	if(argc != 4){
		fprintf(stderr,"Usage : ./authenticate_msg message_file receiver's_private_key sender's_public_key\n");
		exit(0);
	}

	received_msg = read_file_to_string(argv[1]);
	r_pr = get_key_from_file(argv[2]);
	s_pu = get_key_from_file(argv[3]);

	decrypted_msg = pri_dec(received_msg,r_pr);

	id = extract_id(decrypted_msg);
	ds = extract_ds(decrypted_msg);


	receiver_hash = create_hash_of_string(id,strlen(id));
	sender_hash = pub_dec(ds,s_pu);

	if(!authenticate(receiver_hash,sender_hash)){
		printf("Authentication failed!!\n");
	}
	else{
		printf("Authentication Successful!\n");
	}

	free(received_msg);
	free(r_pr);
	free(s_pu);
	free(decrypted_msg);
	free(id);
	free(ds);
	free(receiver_hash);
	free(sender_hash);

	return EXIT_SUCCESS;
}
