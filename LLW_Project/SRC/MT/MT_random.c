#include"MT_random.h"
#include".\Lib\includes.h"
/************************************************************************/
/*                  马特赛特旋转算法                                    */
/************************************************************************/
#define MERS_N   624
#define MERS_M   397
#define MERS_R   31
#define MERS_U   11
#define MERS_S   7
#define MERS_T   15
#define MERS_L   18
#define MERS_A   0x9908B0DF
#define MERS_B   0x9D2C5680
#define MERS_C   0xEFC60000

//typedef unsigned long uint32_t;
//typedef unsigned long int32_t;

//////////////////////////////////////////////////////////////////////////
void RandomInit(int seed);          // Re-seed  
void RandomInitByArray(int const seeds[], int NumSeeds); // Seed by more than 32 bits
int IRandom (int min, int max);     // Output random integer
int IRandomX(int min, int max);     // Output random integer, exact
double Random_mt(void);                    // Output random float
uint32_t BRandom(void);                 // Output random bits

void Init0(int seed);               // Basic initialization procedure
uint32_t mt[MERS_N];                // State vector
int mti;                            // Index into mt
uint32_t LastInterval;              // Last interval length for IRandomX
uint32_t RLimit;                    // Rejection limit used by IRandomX
 
/************************************************************************/
/*                                                                      */
/************************************************************************/
void Init0(int seed) 
{
	// Seed generator
	const uint32_t factor = 1812433253UL;
	mt[0]= seed;
	for (mti=1; mti < MERS_N; mti++) {
		mt[mti] = (factor * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void RandomInit(int seed) 
{
	INT16U i;
	// Initialize and seed
	Init0(seed);
	
	// Randomize some more
	for( i = 0; i < 624; i++) BRandom();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void RandomInitByArray(int const seeds[], int NumSeeds) 
{
	// Seed by more than 32 bits
	int i, j, k;
	
	// Initialize
	Init0(19650218);
	
	if (NumSeeds <= 0) return;
	
	// Randomize mt[] using whole seeds[] array
	i = 1;  j = 0;
	k = (MERS_N > NumSeeds ? MERS_N : NumSeeds);
	for (; k; k--) {
		mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL)) + (uint32_t)seeds[j] + j;
		i++; j++;
		if (i >= MERS_N) {mt[0] = mt[MERS_N-1]; i=1;}
		if (j >= NumSeeds) j=0;}
	for (k = MERS_N-1; k; k--) {
		mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL)) - i;
		if (++i >= MERS_N) {mt[0] = mt[MERS_N-1]; i=1;}}
	mt[0] = 0x80000000UL;  // MSB is 1; assuring non-zero initial array
	
	// Randomize some more
	mti = 0;
	for ( i = 0; i <= MERS_N; i++) BRandom();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
uint32_t BRandom() 
{
	// Generate 32 random bits
	uint32_t y;
	
	if (mti >= MERS_N) {
		// Generate MERS_N words at one time
		const uint32_t LOWER_MASK = (1LU << MERS_R) - 1;       // Lower MERS_R bits
		const uint32_t UPPER_MASK = 0xFFFFFFFF << MERS_R;      // Upper (32 - MERS_R) bits
		static const uint32_t mag01[2] = {0, MERS_A};
		
		int kk;
		for (kk=0; kk < MERS_N-MERS_M; kk++) {    
			y = (mt[kk] & UPPER_MASK) | (mt[kk+1] & LOWER_MASK);
			mt[kk] = mt[kk+MERS_M] ^ (y >> 1) ^ mag01[y & 1];}
		
		for (; kk < MERS_N-1; kk++) {    
			y = (mt[kk] & UPPER_MASK) | (mt[kk+1] & LOWER_MASK);
			mt[kk] = mt[kk+(MERS_M-MERS_N)] ^ (y >> 1) ^ mag01[y & 1];}      
		
		y = (mt[MERS_N-1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
		mt[MERS_N-1] = mt[MERS_M-1] ^ (y >> 1) ^ mag01[y & 1];
		mti = 0;
	}
	y = mt[mti++];
	
	// Tempering (May be omitted):
	y ^=  y >> MERS_U;
	y ^= (y << MERS_S) & MERS_B;
	y ^= (y << MERS_T) & MERS_C;
	y ^=  y >> MERS_L;
	
	return y;
}
#if 0
/* generates a random number on [0,0x7fffffff]-interval */
long genrand_int31(void)
{
    return (long)(genrand_int32()>>1);
}

/* generates a random number on [0,1]-real-interval */
double genrand_real1(void)
{
    return genrand_int32()*(1.0/4294967295.0);
    /* divided by 2^32-1 */
}

/* generates a random number on [0,1)-real-interval */
double genrand_real2(void)
{
    return genrand_int32()*(1.0/4294967296.0);
    /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
double genrand_real3(void)
{
    return (((double)genrand_int32()) + 0.5)*(1.0/4294967296.0);
    /* divided by 2^32 */
}

/* generates a random number on [0,1) with 53-bit resolution*/
double genrand_res53(void)
{
    unsigned long a=genrand_int32()>>5, b=genrand_int32()>>6;
    return(a*67108864.0+b)*(1.0/9007199254740992.0);
}
#endif
/************************************************************************/
/*                                                                      */
/************************************************************************/
double Random_mt(void) 
{
	// Output random float number in the interval 0 <= x < 1
	// Multiply by 2^(-32)
	return (double)BRandom() * (1./(4294967296.0));
}
/************************************************************************/
/* 产生(0,1]之间的随机数                                                                     */
/************************************************************************/
double Random_1()
{
	double r;
	do 
	{
		r=Random_mt() ;	
	} while(r==0);
	return r;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int IRandom(int min, int max)
 {
 	int r;
	// Output random integer in the interval min <= x <= max
	// Relative error on frequencies < 2^-32
	if (max <= min) {
		if (max == min) return min; 
		else return 0x80000000;
	}
	// Multiply interval with random and truncate
	r = (int)((double)(uint32_t)(max - min + 1) * Random_mt() + min); 
	if (r > max) r = max;
	return r;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int IRandomX(int min, int max) 
{

	// 64 bit integers not available. Use modulo method
	uint32_t interval;                    // Length of interval
	uint32_t bran;                        // Random bits
	uint32_t iran;                        // bran / interval
	uint32_t remainder;                   // bran % interval
	
	// Output random integer in the interval min <= x <= max
	// Each output value has exactly the same probability.
	// This is obtained by rejecting certain bit values so that the number
	// of possible bit values is divisible by the interval length
	if (max <= min) {
		if (max == min) return min; 
		else return 0x80000000;
	}

	interval = (uint32_t)(max - min + 1); 
	if (interval != LastInterval) {
		// Interval length has changed. Must calculate rejection limit
		// Reject when iran = 2^32 / interval
		// We can't make 2^32 so we use 2^32-1 and correct afterwards
		RLimit = (uint32_t)0xFFFFFFFF / interval;
		if ((uint32_t)0xFFFFFFFF % interval == interval - 1) RLimit++;
	}
	do { // Rejection loop
		bran = BRandom();
		iran = bran / interval;
		remainder = bran % interval;
	} while (iran >= RLimit);
	// Convert back to signed and return result
	return (int32_t)remainder + min;
	
}

/*************    函数名：GetGaussianDistribution()   ******************
【功能】：
		产生12个（0，1）平均分布的随机函数，用大数定理可以模拟出正态分布。
		高斯分布 
【参数】：expect :数学期望  sigma:方差
【返回】：
		满足高斯分布的随机数
【说明】：
*********************************************************************************/
u8 GetGaussianDistribution(double expect,double sigma) 
{ 
	u8 i; 
	double sum=0.0; 
	u8 r=0;

	/*方差恒大于0*/
	if(sigma<=0.0) { return 0; } 

	/*产生12个随机函数*/
	for(i=1;i<=12;i++) 
		sum = sum + Random_1(); 

	/*大数定理模拟高斯分布*/
	r=(u8)((sum-6.00)*sigma+expect); 
	
	return r; 	
} 



















