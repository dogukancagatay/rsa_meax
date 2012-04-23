/**
 * @file
 * @brief Bit operations on C integers and characters.
 *
 * This file contains the implementations to bit manipulation functions for int and char types in C.
 */

#ifndef BIT_OPT_H_
#define BIT_OPT_H_

#define INTBITLENGTH 32
#define CHARBITLENGTH 8

#include <stdlib.h>

/**
 * @struct INTBIT
 * @brief INTBIT is used to represent and manipulate the bits of an integer number.
 */
typedef struct INTBIT{
	unsigned int b0:2,b1:2,b2:2,b3:2,b4:2,b5:2,b6:2,b7:2,b8:2,b9:2;
	unsigned int b10:2,b11:2,b12:2,b13:2,b14:2,b15:2,b16:2,b17:2,b18:2,b19:2;
	unsigned int b20:2,b21:2,b22:2,b23:2,b24:2,b25:2,b26:2,b27:2,b28:2,b29:2,b30:2,b31:2;
}intbit;
/**
 * @struct CHARBIT
 * @brief CHARBIT is used to represent and manipulate the bits of an character.
 */
typedef struct CHARBIT {
	unsigned int b0:1,b1:1,b2:1,b3:1,b4:1,b5:1,b6:1,b7:1;
}charbit;

/**
 *
 * @param c Input character
 * @return The bits of the input character in a CHARBIT structure.
 *
 * @brief The function reveals the bits of the input character.
 *
 * The function extract the bits of the input character and put them into a CHARBIT structure for easy manipulation.
 */
charbit* char_to_bits(unsigned char c){
	charbit* bitArr = (charbit*)malloc(sizeof(charbit));
	unsigned char ch = (unsigned char)c;

		bitArr->b0 = ch%2==1?1:0;
		ch>>=1;
		bitArr->b1 = ch%2==1?1:0;
		ch>>=1;
		bitArr->b2 = ch%2==1?1:0;
		ch>>=1;
		bitArr->b3 = ch%2==1?1:0;
		ch>>=1;
		bitArr->b4 = ch%2==1?1:0;
		ch>>=1;
		bitArr->b5 = ch%2==1?1:0;
		ch>>=1;
		bitArr->b6 = ch%2==1?1:0;
		ch>>=1;
		bitArr->b7 = ch%2==1?1:0;

	return bitArr;
}
/**
 *
 * @param cb Character represented in bits as CHARBIT structure
 * @return Character value
 *
 * @brief This function converts bit to character
 *
 * The function takes the bit representation in CHARBIT structure and turns them into the corresponding character.
 */
char bits_to_char(charbit* cb){
	char a = 0;
	if(cb->b0){
		a=a^1;
	}
	if(cb->b1){
		a=a^2;
	}
	if(cb->b2){
		a=a^4;
	}
	if(cb->b3){
		a=a^8;
	}
	if(cb->b4){
		a=a^16;
	}
	if(cb->b5){
		a=a^32;
	}
	if(cb->b6){
		a=a^64;
	}
	if(cb->b7){
		a=a^128;
	}

	return a;
}
/**
 *
 * @param cb Character represented in bits as CHARBIT structure
 * @param i Index of the bit that will be gotten.
 * @return Bit value of the given bit index.
 *
 * @brief Reveals the bit value at the given index.
 *
 * The function reveals the bit value of the given character given as CHARBIT structure at the given index.
 */
int getbit_of_charbit(charbit* cb,int i){
	if(i == 0){
		return cb->b0;
	}else if(i == 1){
		return cb->b1;
	}else if(i == 2){
		return cb->b2;
	}else if(i == 3){
		return cb->b3;
	}else if(i == 4){
		return cb->b4;
	}else if(i == 5){
		return cb->b5;
	}else if(i == 6){
		return cb->b6;
	}else if(i == 7){
		return cb->b7;
	}
	else {
		printf("No index to get 0-7 exiting...\n");
		exit(0);
	}
	return 0;
}
/**
 *
 * @param cb Character represented in bits as CHARBIT structure
 * @param i Index of the bit that will be manipulated
 * @param setbit New bit value.
 *
 * @brief The function does the manipulation at the given index of the character.
 *
 * This function enables us to manipulate the bits of the given character as CHARBIT.
 */
void setbit_of_charbit(charbit* cb,unsigned int i,unsigned int setbit){
	if(i == 0){
		cb->b0 = setbit % 2;
	}else if(i == 1){
		cb->b1 = setbit % 2;
	}else if(i == 2){
		cb->b2 = setbit % 2;
	}else if(i == 3){
		cb->b3 = setbit % 2;
	}else if(i == 4){
		cb->b4 = setbit % 2;
	}else if(i == 5){
		cb->b5 = setbit % 2;
	}else if(i == 6){
		cb->b6 = setbit % 2;
	}else if(i == 7){
		cb->b7 = setbit % 2;
	}
	else {
		printf("No index to get 0-7 exiting...\n");
		exit(0);
	}
}
/**
 *
 * @param bin_num Integer represented in bits as INTBIT structure
 * @param i Index of the bit that will be manipulated
 * @param setbit New bit value
 *
 * @brief The function does the manipulation at the given index of the integer.
 *
 * This function enables us to manipulate the bits of the given integer as INTBIT.
 */
void setbit_of_intbit(intbit* bin_num,unsigned int i,unsigned int setbit){
	if(i == 0){
		bin_num->b0 = setbit % 2;
	}else if(i == 1){
		bin_num->b1 = setbit % 2;
	}else if(i == 2){
		bin_num->b2 = setbit % 2;
	}else if(i == 3){
		bin_num->b3 = setbit % 2;
	}else if(i == 4){
		bin_num->b4 = setbit % 2;
	}else if(i == 5){
		bin_num->b5 = setbit % 2;
	}else if(i == 6){
		bin_num->b6 = setbit % 2;
	}else if(i == 7){
		bin_num->b7 = setbit % 2;
	}else if(i == 8){
		bin_num->b8 = setbit % 2;
	}else if(i == 9){
		bin_num->b9 = setbit % 2;
	}else if(i == 10){
		bin_num->b10 = setbit % 2;
	}else if(i == 11){
		bin_num->b11 = setbit % 2;
	}else if(i == 12){
		bin_num->b12 = setbit % 2;
	}else if(i == 13){
		bin_num->b13 = setbit % 2;
	}else if(i == 14){
		bin_num->b14 = setbit % 2;
	}else if(i == 15){
		bin_num->b15 = setbit % 2;
	}else if(i == 16){
		bin_num->b16 = setbit % 2;
	}else if(i == 17){
		bin_num->b17 = setbit % 2;
	}else if(i == 18){
		bin_num->b18 = setbit % 2;
	}else if(i == 19){
		bin_num->b19 = setbit % 2;
	}else if(i == 20){
		bin_num->b20 = setbit % 2;
	}else if(i == 21){
		bin_num->b21 = setbit % 2;
	}else if(i == 22){
		bin_num->b22 = setbit % 2;
	}else if(i == 23){
		bin_num->b23 = setbit % 2;
	}else if(i == 24){
		bin_num->b24 = setbit % 2;
	}else if(i == 25){
		bin_num->b25 = setbit % 2;
	}else if(i == 26){
		bin_num->b26 = setbit % 2;
	}else if(i == 27){
		bin_num->b27 = setbit % 2;
	}else if(i == 28){
		bin_num->b28 = setbit % 2;
	}else if(i == 29){
		bin_num->b29 = setbit % 2;
	}else if(i == 30){
		bin_num->b30 = setbit % 2;
	}else if(i == 31){
		bin_num->b31 = setbit % 2;
	}
	else{
		printf("No index to get 0-31 exiting...\n");
		exit(0);
	}
}
/**
 *
 * @param bin_num Integer represented in bits as INTBIT structure
 * @param i Index of the bit that will be gotten
 * @return Bit value at the given bit index.
 *
 * @brief Reveals the bit value at the given index.
 *
 * The function reveals the bit value of the given integer given as INTBIT structure at the given index.
 */
int getbit_of_intbit(intbit* bin_num,int i){
	if(i == 0){
		return bin_num->b0;
	}else if(i == 1){
		return bin_num->b1;
	}else if(i == 2){
		return bin_num->b2;
	}else if(i == 3){
		return bin_num->b3;
	}else if(i == 4){
		return bin_num->b4;
	}else if(i == 5){
		return bin_num->b5;
	}else if(i == 6){
		return bin_num->b6;
	}else if(i == 7){
		return bin_num->b7;
	}else if(i == 8){
		return bin_num->b8;
	}else if(i == 9){
		return bin_num->b9;
	}else if(i == 10){
		return bin_num->b10;
	}else if(i == 11){
		return bin_num->b11;
	}else if(i == 12){
		return bin_num->b12;
	}else if(i == 13){
		return bin_num->b13;
	}else if(i == 14){
		return bin_num->b14;
	}else if(i == 15){
		return bin_num->b15;
	}else if(i == 16){
		return bin_num->b16;
	}else if(i == 17){
		return bin_num->b17;
	}else if(i == 18){
		return bin_num->b18;
	}else if(i == 19){
		return bin_num->b19;
	}else if(i == 20){
		return bin_num->b20;
	}else if(i == 21){
		return bin_num->b21;
	}else if(i == 22){
		return bin_num->b22;
	}else if(i == 23){
		return bin_num->b23;
	}else if(i == 24){
		return bin_num->b24;
	}else if(i == 25){
		return bin_num->b25;
	}else if(i == 26){
		return bin_num->b26;
	}else if(i == 27){
		return bin_num->b27;
	}else if(i == 28){
		return bin_num->b28;
	}else if(i == 29){
		return bin_num->b29;
	}else if(i == 30){
		return bin_num->b30;
	}else if(i == 31){
		return bin_num->b31;
	}
	else{
		printf("No index to set 0-31 exiting...\n");
		exit(0);
	}
}
/**
 *
 * @param bin_num Integer represented in bits as INTBIT structure
 * @brief Prints the bits of INTBIT structure on the screen
 *
 * This function is not used in the project. It is put for the debugging purposes.
 */
void print_bits_of_intbits(intbit* bin_num){
	int i;
	for (i = INTBITLENGTH-1; i >= 0; --i) {
		printf("%d ",getbit_of_intbit(bin_num,i));
	}
	printf("\n");
}
/**
 *
 * @param x Input integer
 * @return The bits of the input integer in a INTBIT structure.
 *
 * @brief The function reveals the bits of the input integer.
 *
 * The function extract the bits of the input integer and put them into a INTBIT structure for easy manipulation.
 */
intbit* int_to_bits(unsigned int x){
	intbit* bin_num = (intbit*)malloc(sizeof(intbit));
	unsigned int ch = x;
	int i;

	for(i = 0; i < INTBITLENGTH; ++i){
		setbit_of_intbit(bin_num,i,ch%2==1?1:0);
		ch>>=1;
	}

	return bin_num;
}
/**
 *
 * @param bin_num Integer represented in bits as INTBIT structure
 * @return Integer value
 *
 * @brief This function converts bit to integer
 *
 * The function takes the bit representation in INTBIT structure and turns them into the corresponding integer.
 *
 */
unsigned int bits_to_int(intbit* bin_num){
	int i;
	unsigned int dec_num = 0;

	for(i = 0; i<INTBITLENGTH; i++){
		dec_num += (getbit_of_intbit(bin_num,i) * pow(2.0,(double)i));
	}

	return dec_num;
}
/**
 *
 * @param str Input string
 * @param size Size of the string
 * @return The last characters index
 */
int find_right_most_char(char* str,int size){
	int i;
	for (i = 0; i < size; ++i) {
		if(str[i] == '\0')
			return i == 0 ? 0 : i-1;
	}
	return size-1;
}
/**
 *
 * @param str Input string
 * @return Compressed integer value
 *
 * @brief The function does the compression on the given 4 characters.
 *
 * The compression that compresses 4 characters into an integer is done in this function.
 * It reveals the bits of the characters and puts returns them as an integer.
 */
int compress_chars_to_int(char* str){ // every time it will get 4 characters
	int i,j;
	int res = 0;
	int bit_cnt = 31; // starting from the 31st bit
	intbit* ib = int_to_bits(0); // initialize to zero
	charbit* cb;



	// starting from the rightmost character visit each char
	for (i = 0; i < 4; ++i) {
		if(str[i] != '\0'){// if it is not termination char
			cb = char_to_bits(str[i]);// convert character to bits
			for (j = CHARBITLENGTH - 1; j >=0 ; --j) {// for each bit of the character
				setbit_of_intbit(ib,bit_cnt--,getbit_of_charbit(cb,j)); // set the bit of integer
			}
			free(cb);// free charbit because next time it will be reallocated
		}
		else { // it is termination char fill with zeros
			break;
//			for (j = 0; j < CHARBITLENGTH; ++j) {// for each bit of the character
//				setbit_of_intbit(ib,bit_cnt--,0); // set the bit of integer to zero
//			}
		}
	}

	res = bits_to_int(ib);
	free(ib);
	return res;
}
/**
 *
 * @param x Compressed integer
 * @return Decompressed characters
 *
 * @brief Decompresses the integer to its initial character values.
 *
 * The decompression is done in this function. The function basically reads the bits of integer and extracts them as 4 characters.
 */
char* decompress_int_to_char(unsigned int x){
	char* str = (char*)malloc(4*sizeof(char));
	int i,j;
	char temp;
	int bit_cnt = 31;
	intbit* ib = int_to_bits(x);
	charbit* cb;

	for (i = 0; i < 4 && bit_cnt >= 0; ++i) {// for each 8 bit
		cb = char_to_bits(0); // allocate space to the charbit
		for (j = CHARBITLENGTH - 1; j >= 0 ; --j) { // loop for each bit of charbit
			setbit_of_charbit(cb,j,getbit_of_intbit(ib,bit_cnt--)); // copy bit to the character
		}
		temp = bits_to_char(cb); // convert bits to character
		free(cb);// deallocate memory since it will be reallocated
		str[i] = temp;
	}
	free(ib);
	return str;
}

#endif /* BIT_OPT_H_ */
