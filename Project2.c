/* CSCI3240: Project 2
 * Abigail Kelly
 * Dr. Sainju
 * Due 3/2/23
 * This program is a compilation of functions that were reverse engineered from
 * assembly code
 */

/* This function calculates a to b power */
long MysteryFunction1(long a, int b){
   long ret_val = 1;

   while(b > 0){                       /* to keep track of how many times you're multiplying a */
      ret_val *= a;  
      b--;
   }

   return ret_val;                     /* return the value */
}

/* This function reverses the bit pattern of a number */
unsigned int MysteryFunction2(unsigned int num){
   unsigned int temp;                  /* temporary value used for bit manipulations */
   unsigned int d = 0;                 /* to store value of d | temp */
   unsigned int TMIN = -2147483648;    /* value of T_MIN */

   for (int i = 0; i <= 31; i++) {     /* loop through 32 bits */
      temp = 1 << i;                   /* multiply the 1 by 2 to the i power */

      if((temp & num) != 0){
         temp = TMIN >> i;             /* divide TMIN by 2 to the i power */
         d = d | temp;
      }
   }

   return d;                           /* return the result */
}

/* This function will find the maximum value in an array */
long MysteryFunction3(long *a, int n){
   long count = 1;                     /* counter to keep track of index in array */
   long max = a[0];                    /* set max value (use in comparison) */

   while(count < n){                   /* iterate through array */
      if(a[count] > max){              /* find max value */
         max = a[count];
      }

      count++;                         /* go to next element in array */
   }

   return max;                         /* return the max value in the array */
}

/* This function counts the number of bits that are 1 */
int MysteryFunction4(unsigned long n){
   int ret_val = 0;                    /* holds the number of bits that are 1 */
   while(n != 0){                      /* iterate through each bit of n */
      ret_val += 1 & n;                /* increment the number of 1s by 1 or 0 */
      n = n >> 1;                      /* right shift the bits by 1 (multiply n by 2)*/
   }

   return ret_val;                     /* return the number of bits that are 1 */
}

/* Counts number of bits that the two numbers differ in */
unsigned int MysteryFunction5(unsigned int A, unsigned int B){
   A = A ^ B;                          /* xor A and B */
   unsigned int ret_val = 0;           /* to count number of bits that are 1 */

   while(A != 0){                      /* iterate through every bit of A ^ B */
      ret_val = ret_val + (A & 1);     /* count the number of bits set to 1 (will add 0 or 1) */
      A = A >> 1;                      /* right shift A by 1 (multiply A by 2) */
   }

   return ret_val;                     /* return the number of bits that are 1 in the result of A ^ B*/
}