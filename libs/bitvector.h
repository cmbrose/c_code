/*
 *	FILE: bitvector.h
 *	AUTHOR: Caleb Brose
 *
 *	USE: Defines the bit_vector_t type which is useful 
 *		for generating permutations of sets quickly.
 */

#ifndef BITVECTOR_H
#define BITVECTOR_H

// Bit vector is made of a string of these chunks
typedef unsigned char bv_chunk;

// Bit vector type definition
typedef struct
{
	bv_chunk* chunks;
	int size, perm_num;
	unsigned long long loops;
} bit_vector_t, *bit_vector_p;

/*
 *	Mallocs a new bit_vector of the given size.
 *		NOTE: This bit_vector is already initialized.
 *
 *	PARAMS:
 *		ele_cnt	- Number of bits the bit_vector will have.
 *
 *	RETURN:
 *		A new bit_vector of the given size.
 */
bit_vector_p bit_vector_make(int ele_cnt);

/*
 *	Initializes a bit_vector of the given size.
 *
 *	PARAMS:
 *		ele_cnt	- Number of bits the bit_vector will have.
 */
void bit_vector_init(bit_vector_p bv, int ele_cnt);

/*
 *	Frees the memory of the given bit_vector.
 *
 *	PARAMS:
 *		bv	- bit_vector to destroy.
 */
void bit_vector_destroy(bit_vector_p bv);

/*
 *	Sets the idx-th bit in bv.
 *
 *	PARAMS:
 *		bv	- bit_vector to modify.
 *		idx - index of the bit to set.
 */
void bit_vector_set(bit_vector_p bv, int idx);

/*
 *	Checks whether the idx-th bit of bv is set.
 *
 *	PARAMS:
 *		bv	- bit_vector to check.
 *		idx - index of the bit to check.
 *
 *	RETURN:
 *		1 if bit is set, 0 otherwise.
 */
int bit_vector_is_set(bit_vector_p bv, int idx);

/*
 *	Clears the idx-th bit in bv.
 *
 *	PARAMS:
 *		bv	- bit_vector to modify.
 *		idx - index of the bit to clear.
 */
void bit_vector_clear(bit_vector_p bv, int idx);

/*
 *	Clears all bits in bv.
 *
 *	PARAMS:
 *		bv	- bit_vector to clear.
 */
void bit_vector_clear_all(bit_vector_p bv);

/*
 *	Sets bits in bv to represent the next permutation.
 *
 *	PARAMS:
 *		bv	- bit_vector to modify.
 */
void bit_vector_next_perm(bit_vector_p bv);

#endif // BITVECTOR_H