/*
 *	FILE: bitvector.c
 *	AUTHOR: Caleb Brose
 *
 *	USE: Defines a bit vector type which is useful 
 *		for generating permutations of sets quickly.
 */

#include <stdlib.h>
#include "bitvector.h"

//----------------------------------------------------
bit_vector_p bit_vector_make(int ele_cnt)
{
	bit_vector_p ret = (bit_vector_p) malloc(sizeof(bit_vector_t));
	bit_vector_init(ret, ele_cnt);
	return ret;
}

//----------------------------------------------------
void bit_vector_init(bit_vector_p bv, int ele_cnt)
{
	bv->size = ele_cnt;
	bv->perm_num = 0;
	bv->loops = 0;
	
	bv->chunks = (bv_chunk*) malloc( sizeof(bv_chunk) * ( ele_cnt + 7 ) / 8 );
	
	int i;
	for ( i = 0; i < (ele_cnt + 7) / 8; ++i )
		bv->chunks[i] = 0;
}

//----------------------------------------------------
void bit_vector_destroy(bit_vector_p bv)
{
	free(bv->chunks);
	free(bv);
}

//----------------------------------------------------
void bit_vector_set(bit_vector_p bv, int idx)
{
	bv_chunk* use = &bv->chunks[idx / 8];
	*use |= 1 << (idx % 8);
}

//----------------------------------------------------
int bit_vector_is_set(bit_vector_p bv, int idx)
{
	bv_chunk use = bv->chunks[idx / 8];
	return use & ( 1 << (idx % 8) );
}

//----------------------------------------------------
void bit_vector_clear(bit_vector_p bv, int idx)
{
	bv_chunk* use = &bv->chunks[idx / 8];
	*use &= ~( 1 << (idx % 8) );
}

//----------------------------------------------------
void bit_vector_clear_all(bit_vector_p bv)
{
	int i;
	for ( i = 0; i < (bv->size + 7) / 8; ++i )
		bv->chunks[i] = 0;
}

//----------------------------------------------------
void bit_vector_get_next(bit_vector_p bv)
{
	int n_chunks = (bv->size + 7) / 8;
	bv_chunk* curr = bv->chunks;
	do
	{
		*curr += 1;
		--n_chunks;
		curr = curr + 1;
	} while ( !(*(curr - 1)) && n_chunks );
	
	++(bv->perm_num);
	
	if ( !(*(curr - 1)) )
	{
		bit_vector_clear_all(bv);
		++(bv->loops);
		bv->perm_num = 0;
	}
}