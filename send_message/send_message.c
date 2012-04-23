/**
 * @file
 * @brief The main function for the sending message operation.
 *
 * The file contains the instructions that will be used to create encrypted digital signature and the the other parts. It will create
 * a msg_to_send.txt file that will be sent to the receiver and will be an input to authenticate_msg program.
 */
#include <stdio.h>
#include <stdlib.h>
#include "../lib/blumblumshub.h"
#include "../lib/math_opts.h"
#include "../lib/rsa_opts.h"
#include "../lib/general_opts.h"
#include "../lib/bit_opts.h"

int main(int argc,char** argv) {
	rsa_key *r_pu, *s_pr;
	char *id,*ds,*sender_msg,*id_ds_concat;

	if(argc != 4){
		fprintf(stderr,"Usage : ./send_message input_message sender's_private_key receiver's_public_key\n");
		exit(0);
	}

	id = read_file_to_string(argv[1]);
	s_pr = get_key_from_file(argv[2]);
	r_pu = get_key_from_file(argv[3]);

	ds = create_ds(id,s_pr);
	id_ds_concat = concatenate(id,ds);

	sender_msg = pub_enc(id_ds_concat,r_pu);
	write_string_to_file("message_to_send.txt",sender_msg);

	free(id);
	free(ds);
	free(sender_msg);
	free(id_ds_concat);
	return EXIT_SUCCESS;
}
