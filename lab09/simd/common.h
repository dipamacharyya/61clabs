#ifndef COMMON_H
#define COMMON_H

#include <x86intrin.h>

#define NUM_ELEMS ((1 << 16) + 10)
#define OUTER_ITERATIONS (1 << 16)

long long int sum(unsigned int vals[NUM_ELEMS]) {
	clock_t start = clock();

	long long int sum = 0;
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		for(unsigned int i = 0; i < NUM_ELEMS; i++) {
			if(vals[i] >= 128) {
				sum += vals[i];
			}
		}
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return sum;
}

long long int sum_unrolled(unsigned int vals[NUM_ELEMS]) {
	clock_t start = clock();
	long long int sum = 0;

	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) { 
		for(unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
			if(vals[i] >= 128) sum += vals[i];
			if(vals[i + 1] >= 128) sum += vals[i + 1];
			if(vals[i + 2] >= 128) sum += vals[i + 2];
			if(vals[i + 3] >= 128) sum += vals[i + 3];
		}

		//This is what we call the TAIL CASE
		//For when NUM_ELEMS isn't a multiple of 4
		//NONTRIVIAL FACT: NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
		for(unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
			if (vals[i] >= 128) {
				sum += vals[i];
			}
		}
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return sum;
}

long long int sum_simd(unsigned int vals[NUM_ELEMS]) {
	clock_t start = clock();
	__m128i _127 = _mm_set1_epi32(127);		// This is a vector with 127s in it... Why might you need this?
	long long int result = 0;				// This is where you should put your final result!
											// DO NOT DO NOT DO NOT DO NOT WRITE ANYTHING ABOVE THIS LINE.
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		/* YOUR CODE GOES HERE */
		__m128i zeroes = _mm_setzero_si128();

		for (unsigned i = 0; i < NUM_ELEMS/4*4; i+=4) {
			__m128i fours = _mm_loadu_si128(&vals[i]);
			__m128i comp = _mm_cmpgt_epi32(fours, _127);
			__m128i resultc = _mm_and_si128(fours,comp);
			zeroes = _mm_add_epi32(resultc, zeroes);
		
		}
		int *bob = malloc(sizeof(int)*4);
		_mm_storeu_si128(bob, zeroes);
		result += bob[0];
		result += bob[1];
		result += bob[2];
		result += bob[3];
		free(bob); 

		for(unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
			if (vals[i] >= 128) {
				result += vals[i];
			}
		}
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return result;
}

long long int sum_simd_unrolled(unsigned int vals[NUM_ELEMS]) {
	clock_t start = clock();
	__m128i _127 = _mm_set1_epi32(127);
	long long int result = 0;
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		/* COPY AND PASTE YOUR sum_simd() HERE */
		/* MODIFY IT BY UNROLLING IT */
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		/* YOUR CODE GOES HERE */
		__m128i zeroes = _mm_setzero_si128();

		for (unsigned i = 0; i < NUM_ELEMS/16*16; i+=16) {
			__m128i fours1 = _mm_loadu_si128(&vals[i]);
			__m128i fours2 = _mm_loadu_si128(&vals[i+4]);
			__m128i fours3 = _mm_loadu_si128(&vals[i+8]);
			__m128i fours4 = _mm_loadu_si128(&vals[i+12]);
			__m128i comp1 = _mm_cmpgt_epi32(fours1, _127);
			__m128i comp2 = _mm_cmpgt_epi32(fours2, _127);
			__m128i comp3 = _mm_cmpgt_epi32(fours3, _127);
			__m128i comp4 = _mm_cmpgt_epi32(fours4, _127);
			__m128i result1 = _mm_and_si128(fours1,comp1);
			__m128i result2 = _mm_and_si128(fours2,comp2);
			__m128i result3 = _mm_and_si128(fours3,comp3);
			__m128i result4 = _mm_and_si128(fours4,comp4);
			zeroes = _mm_add_epi32(result1, zeroes);
			zeroes = _mm_add_epi32(result2, zeroes);
			zeroes = _mm_add_epi32(result3, zeroes);
			zeroes = _mm_add_epi32(result4, zeroes);
		}
		int *bob = malloc(sizeof(int)*4);
		_mm_storeu_si128(bob, zeroes);
		result += bob[0];
		result += bob[1];
		result += bob[2];
		result += bob[3];
		free(bob); 

		for(unsigned int i = NUM_ELEMS / 16 * 16; i < NUM_ELEMS; i++) {
			if (vals[i] >= 128) {
				result += vals[i];
			}
		}
	}
		/* You'll need 1 or maybe 2 tail cases here. */

	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return result;
}

/* DON'T TOUCH THIS FUNCTION */
int int_comparator(const void* a, const void* b) {
	if(*(unsigned int*)a == *(unsigned int*)b) return 0;
	else if(*(unsigned int*)a < *(unsigned int*)b) return -1;
	else return 1;
}

#endif
