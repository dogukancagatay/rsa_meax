/**
 * @file
 * @brief General operations for the program.
 *
 * General operations used throughout the project, are implemented in this file.
 */

#ifndef GENERAL_OPTS_H_
#define GENERAL_OPTS_H_

#include "sha256.h"


//void strconcatenate(char* dest,const char* src){
//	int i,j = 0;
//	int dest_size = strlen(dest);
//	int src_size = strlen(src);
//	int flag = 0;
//
//	for (i = 0; i < dest_size + src_size + 1 ; ++i) {
//		if(flag){
//			if(j < src_size){
//				dest[i] = src[j++];
//			}
//			else{
//				dest[i] = '\0';
//				break;
//			}
//		}
//		else if(dest[i] == '\0'){
//			flag = 1;
//			if(j < src_size){
//				dest[i] = src[j++];
//			}
//			else{
//				dest[i] = '\0';
//				break;
//			}
//		}
//	}
//}


/**
 *
 * @param str Input string
 * @param size Size of the string
 * @return Hashed value.
 *
 * @brief Hashes the given string value
 *
 * Creates the hash of the input string using SHA256 (Secure Hash Algorithm 256 Bits)
 * secure hash algorithm. The SHA256's output is a 256bit hexadecimal number. The source
 * code of the used SHA256 can be found in sha256.h file.
 */
char* create_hash_of_string(char* str,int size){
	//unsigned char* output = (unsigned char*)malloc(64*sizeof(unsigned char));
	char* output = (char*)malloc((64+1)*sizeof(char));
	sha256_context ctx;
	unsigned char sha256sum[32];
	int i,j;

	sha256_starts(&ctx);

	for (i = 0; i < size; ++i) {// add each char to hash one by one
		sha256_update(&ctx, str + i, 1);
	}

	sha256_finish(&ctx, sha256sum);

	// write hash to the return value
	for(j = 0; j < 32; j++)
	{
		sprintf(output + (2*j), "%02x", sha256sum[j]);
	}
	output[64] = '\0';
	return output;
}
/**
 *
 * @param filename
 * @return Hashed value.
 *
 * @brief Hashes the given file
 *
 * Creates the hash of the given file using SHA256 (Secure Hash Algorithm 256 Bits)
 * secure hash algorithm. The SHA256's output is a 256bit hexadecimal number. The source
 * code of the used SHA256 can be found in sha256.h file.
 */
char* create_hash_of_file(char* filename){
	//unsigned char* output = (unsigned char*)malloc(64*sizeof(unsigned char));
	char* output = (char*)malloc(64*sizeof(char));
	sha256_context ctx;
	unsigned char sha256sum[32];
	int i,j;
	unsigned char buf[256];
	FILE* f;

	sha256_starts(&ctx);

	if(!(f = fopen(filename, "rb")))
	{
		fprintf(stderr,"fopen failed %s\n",filename);
		exit(0);
	}

    sha256_starts( &ctx );

    // read file buff size
    while((i = fread(buf, 1, sizeof(buf), f)) > 0 )// while there are things to read
    {
        sha256_update(&ctx, buf, i);//add to hash
    }

    sha256_finish(&ctx, sha256sum);

   	// write hash to the return value
	for(j = 0; j < 32; j++)
	{
		sprintf(output + (2*j), "%02x", sha256sum[j]);
	}

	fclose(f);
	return output;
}

/**
 *
 * @param m Plain Text
 * @param key Public key
 * @return The encrypted text
 *
 * @brief Encrypts the given plain text with the given public key
 *
 * This function basically does the RSA encryption. The method used to represent characters
 * as numbers is compression. Every 4 character block is put into an integer and this integer
 * value is encrypted. Encryption is done via exponentiation.
 *
 * C = M^e mod n
 */
char* pub_enc(char* m,rsa_key* key){
	unsigned int compressed; /// will contain the compressed value of characters
	int m_size = strlen(m); /// size of the plain text

	/// cycle_number is the number which determines how many compressions will be made and as a result the encrypted portions number.
	int cycle_number = m_size % 4 > 0 ? (m_size/4) + 1: (m_size/4);
	/// key_length determines the max length of the encrypted value
	size_t key_length = mpz_sizeinbase(key->n,DECIMAL);


	/// temporary values will be used in encryption
	mpz_t enc_base,enc_res;
	int i;

	/// there will be cycle_number encrypted value as a result,which can be at most at the length of n, since they are taken mod of n so we allocate memory according to that values. In addition there will be separators '\n' at the end of each encrypted value.
	char* buf = (char*)malloc(((key_length*cycle_number)+cycle_number)*sizeof(char));
	strcpy(buf,"\0"); /// initialize the string to termination character
	char* temp = (char*)malloc((key_length+1)*sizeof(char));

	mpz_init(enc_base);
	mpz_init(enc_res);

	for (i = 0; i < cycle_number; ++i) {
		/// compress 4 chars to an int compressed
		compressed = compress_chars_to_int(m+(i*4));
		mpz_set_ui(enc_base,compressed);
		take_mod_of_exp_number2(enc_res,enc_base,key->k,key->n); /// exponentiation

		/// add the exponentiated value to the string
		gmp_sprintf(temp,"%Zd\n",enc_res);
		strcat(buf,temp);
	}

	free(temp);
	mpz_clear(enc_base);
	mpz_clear(enc_res);
	return buf;
}

/**
 *
 * @param c Ciphered Text
 * @param key Private Key
 * @return The decrypted text
 *
 * @brief Decrypts the given ciphered text(encrypted with public encryption) with the given private key
 *
 * This function basically does the RSA decryption. Decompression is done to reveal the 4 characters we
 * compressed after decryption.
 *
 * M = C^d mod n
 */
char* pri_dec(char* c,rsa_key* key){
	mpz_t c_val;
	mpz_t result;

	int i,c_size = strlen(c);
	int ciphered_cnt = 0; /// will count the total number of decrypted texts
	int deciphered_int = 0;
	int ciphered_start_index = 0; /// initialized to 0 because first substring of c will start from the
	int ret_index = 0;

	if(c_size == 0){ /// if ciphered text is empty exit
		fprintf(stderr,"No ciphered value to decipher!\nExiting...\n");
		exit(0);
	}

	char *deciphered_block; /// each block contains 4 characters that is compressed into an integer

	/// initialize the gmp numbers
	mpz_init(c_val);
	mpz_init(result);

	char *ret; /// return string

	/// find how many integers/ciphered blocks are there in the ciphered file
	for (i = 0; i < c_size; ++i) {
		if(c[i] == '\n'){
			ciphered_cnt++;
		}
	}

	/// allocate the return value, we know the total number of ciphered blocks of characters
	ret = (char*)malloc(((ciphered_cnt*4)+3)*sizeof(char));

	/// each number in the ciphered text(seperated via newline) is read and have turned into mpz_t and decrypted then added to the return string
	for (i = 0; i < c_size; ++i) {
		if(c[i] == '\n'){/// after we read a new line we will turn thar number into mpz_t and decrypt it.
			ciphered_cnt++;

			gmp_sscanf(c+ciphered_start_index,"%Zd",c_val);/// read the ciphered number into c_val
			take_mod_of_exp_number2(result,c_val,key->k,key->n); /// decrypt the read number;
			deciphered_int = (int)mpz_get_ui(result); /// change gmp_integer to an integer
			deciphered_block = decompress_int_to_char(deciphered_int); /// decompress the int to 4 chars.

			/// add the deciphered block to return string
			ret[ret_index++] = deciphered_block[0];
			ret[ret_index++] = deciphered_block[1];
			ret[ret_index++] = deciphered_block[2];
			ret[ret_index++] = deciphered_block[3];

			free(deciphered_block);
			ciphered_start_index = i + 1; /// the next starting index of next sscanf on c. current index points to "\n" but next index points to the starting of a new ciphered block
		}
	}
	ret[ret_index] = '\0';
	mpz_clear(c_val);
	mpz_clear(result);
	return ret;
}

/**
 *
 * @param c Ciphered Text
 * @param key Public Key
 * @return The decrypted text
 *
 * @brief Decrypts the given ciphered text(encrypted with private encryption) with the given public key
 *
 * The function does the general decryption with public key. It resolves the encryption with private key.
 *
 * M = C^d mod n
 */
char* pub_dec(char* c,rsa_key* key){
	return pri_dec(c,key);
}
/**
 *
 * @param m Plain Text
 * @param key Private key
 * @return The encrypted text
 *
 * @brief Encrypts the given plain text with the given private key
 *
 * Since the RSA encryption used in this project is one type, basically
 * the other encryption function is called. It decrypts the encrypted text with public key.
 *
 * C = M^e mod n
 */
char* pri_enc(char* m,rsa_key* key){
	return pub_enc(m,key);
}
/**
 *
 * @param keys
 * @param username
 *
 * @brief Writes the public key to file with user name
 *
 * Since our keys are user based, each person has a key pair, we need to discrete them in a way.
 * So in this project user names are requested while a pair of RSA key is being created. This way
 * there will be no mix ups in the keys.
 */
void write_public_key_to_file(rsa_keys *keys,char* username){
	FILE *pb_fp;
	char* filename = (char*)malloc((strlen(username)+20)*sizeof(char));
	strcpy(filename,username);
	strcat(filename,"_public_key.txt");

	if((pb_fp = fopen(filename,"w")) == NULL){
		fprintf(stderr,"fopen Failed (write_public_key_to_file)");
		exit(0);
	}

	gmp_fprintf(pb_fp,"%Zd\n%Zd\n",keys->pu,keys->n);

	free(filename);
	fclose(pb_fp);
}
/**
 *
 * @param keys
 * @param username
 *
 * @brief Writes the private key to file with user name
 *
 * Since our keys are user based, each person has a key pair, we need to discrete them in a way.
 * So in this project user names are requested while a pair of RSA key is being created. This way
 * there will be no mix ups in the keys.
 */
void write_private_key_to_file(rsa_keys *keys,char* username){
	FILE *pr_fp;
	char* filename = (char*)malloc((strlen(username)+20)*sizeof(char));
		strcpy(filename,username);
		strcat(filename,"_private_key.txt");

	if((pr_fp = fopen(filename,"w")) == NULL){
		fprintf(stderr,"fopen Failed (write_private_key_to_file)");
		exit(0);
	}

	gmp_fprintf(pr_fp,"%Zd\n%Zd\n",keys->pr,keys->n);

	free(filename);
	fclose(pr_fp);
}

/**
 *
 * @param s1 String 1
 * @param s2 String 2
 * @return Concatenation of s1 and s2 with separator between.
 *
 * This function concatenates 2 strings with a separator in between. The separator is 7 # character.
 * The separator is used for the convenience while extracting the parts.
 */
char* concatenate(char* s1,char* s2){
	int conc_size;
	char* conc_str;
	conc_size = strlen(s1) + strlen(s2);

	conc_str = (char*)malloc((conc_size + 10)*sizeof(char));
	sprintf(conc_str,"%s\n#######\n%s",s1,s2);

	return conc_str;
}
/**
 *
 * @param filename File
 * @return File content
 *
 * @brief Gets file content
 *
 * File contents is read and put into a character array. In the end the character array is returned to user.
 */
char* read_file_to_string(char* filename){
	FILE *fp;
	char *str;
	size_t f_size;

	if((fp = fopen(filename,"rb")) == NULL){
		fprintf(stderr,"fopen Failed (read_file_to_string)");
		exit(0);
	}

	//measure the file size
	fseek(fp, 0, SEEK_END);
	f_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);


	// allocate string according to the file size
	str = (char*)malloc(f_size*sizeof(char));

	// start reading the text character by character
	if(fread(str,1,f_size,fp) != f_size){
		fprintf(stderr,"fread failed. (read_file_to_string)\n");
		exit(0);
	}

	fclose(fp);
	return str;
}
/**
 *
 * @param filename File
 * @param str The string to be written to the file
 *
 * @brief Writes the string to file.
 *
 * Writes the given string to a file with the desired file name.
 */
void write_string_to_file(char* filename,char* str){
	FILE *fp;
	// open the file
	if((fp = fopen(filename,"wb")) == NULL){
		fprintf(stderr,"fopen Failed (write_string_to_file)");
		exit(0);
	}
	// write the string to the file
	if(fwrite(str,1,strlen(str),fp) != strlen(str)){
		fprintf(stderr,"fwrite failed. (write_string_to_file)\n");
		exit(0);
	}

	fclose(fp);
}
/**
 *
 * @param filename Key file's name
 * @return RSA Key
 *
 * @brief Read the RSA key from file.
 *
 * GMP (GNU Mathematical Precision Library) is used in this project to be able to work with very long
 *  integers. Mostly the long integers were the keys and encrypted texts. Since we do our exponentiation
 * with GMP numbers, the key file is read with GMP Library functions for easy manipulation on the number.
 * A key file has 2 components separated via new line. first part is the e or d and the second part is n.
 * Our rsa_key structure has k and n elements to hold these values.
 */
rsa_key* get_key_from_file(char* filename){
	rsa_key* key = (rsa_key*)malloc(sizeof(rsa_key));
	mpz_init(key->k);
	mpz_init(key->n);

	// read the file and get the content
	char *key_file_content = read_file_to_string(filename);

	// get keys out of string
	gmp_sscanf(key_file_content,"%Zd%Zd",key->k,key->n);

	free(key_file_content);
	return key;
}
/**
 *
 * @param id Plain text
 * @param pr_key Private Key used for encryption of hash
 * @return Digital Signature of the given plain text
 *
 * @brief Creates the digital signature of the text.
 *
 * Digital signature is the encrypted hash of a given text via the owners/senders private key because
 * the digital signature is unique value to that text. It enables us to be sure about the sent message is valid.
 */
char* create_ds(char *id,rsa_key *pr_key){
	char *hash,*ds;

	hash = create_hash_of_string(id,strlen(id));
	ds = pri_enc(hash,pr_key);

	free(hash);
	return ds;
}
/**
 *
 * @param msg Decrypted message sent to the receiver
 * @return The plain text.
 *
 * @brief Separates the plain text.
 *
 * Separates the plain text from the concatenation of plain text and digital signature. This operation is done
 * after the decryption of the received file.
 */
char* extract_id(char *msg){
	int msg_size = strlen(msg);
	char *id;
	int i;

	for (i = 0; i < msg_size-6; ++i) {
		if(msg[i] == '#' && msg[i+1] == '#' && msg[i+2] == '#' && msg[i+3] == '#' && msg[i+4] == '#' && msg[i+5] == '#' && msg[i+6] == '#'){
			id = (char*)malloc(i*sizeof(char));
			strncpy(id,msg,i);
			id[i-1] = '\0';
		}
	}
	return id;
}
/**
 *
 * @param msg Decrypted Message sent to the receiver
 * @return The digital signature created by the sender
 *
 * @brief Separates the digital signature.
 *
 * Separates the digital signature from the concatenation of plain text and digital signature. This operation is done
 * after the decryption of the received file.
 */
char* extract_ds(char *msg){
	int msg_size = strlen(msg);
	char *ds;
	int i;
	int ds_size;

	for (i = 0; i < msg_size-6; ++i) {
		if(msg[i] == '#' && msg[i+1] == '#' && msg[i+2] == '#' && msg[i+3] == '#' && msg[i+4] == '#' && msg[i+5] == '#' && msg[i+6] == '#'){
			ds_size = msg_size - (i+8) + 1;
			ds = (char*)malloc(ds_size*sizeof(char));
			strncpy(ds,(msg+i+8),ds_size);

		}
	}
	return ds;
}
/**
 *
 * @param hash1 Hash value value
 * @param hash2 Other hash value
 * @return true(1) on success or false(0) on failure
 *
 * @brief Authenticates the hash values are the same or not.
 *
 * Compares the hash values and decides if the sent message is not played on its way to the receiver.
 */
int authenticate(char* hash1,char* hash2){
	int hash_size = strlen(hash1);
	int i;
	int flag = 1;

	for (i = 0; i < hash_size; ++i) {
		if(hash1[i] != hash2[i]){
			flag = 0;
		}
	}
	return flag;
}

#endif /* GENERAL_OPTS_H_ */
