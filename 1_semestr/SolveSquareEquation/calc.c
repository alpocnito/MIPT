#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

const int MAXIMUM_ATTEMPTS = 100;  //!< for InputNumbers function
const int IN_NUMBER        = 100;  //!< number of fail attempts in writing number (InputNumbers function) 
const double EPSILON       = 1e-7; //!< to compare two double values

#define SSG_INFI -1 //!< for infinity number of solutions 

//-------------------------------------------------- 
//! Clear stdin
//-------------------------------------------------- 
void ClearStdin() {
   while (getchar() != '\n');
}


//-------------------------------------------------- 
//! Compare two double values
//!
//! @param [in] a a-first double value
//! @param [in] b b-second double value
//!
//! @return 1, if a > b; -1, if a < b; 0, if a == b 
//-------------------------------------------------- 
int Compare(double a, double b);

//-------------------------------------------------- 
//! Solves linear equation
//!
//! @param [in] coef coef-pointer to the coefficients
//! @param [out] roots roots-pointer to the roots
//!
//! @return Number of roots
//! 
//! @note In case fo infinity number of roots,
//!       returns SSG_INFI
//-------------------------------------------------- 
int SolveLinearEquation(const double* const coef, double* const roots);

//-------------------------------------------------- 
//! Solves square equation
//!
//! @param [in] coef coef-pointer to the coefficients
//! @param [out] roots roots-pointer to the roots
//!
//! @return Number of roots
//! 
//! @note In case fo infinity number of roots,
//!       returns SSG_INFI
//-------------------------------------------------- 
int SolveSquareEquation(const double* const coef, double* const roots);

//-------------------------------------------------- 
//! Read double values from user
//!
//! @param [in] prompt[] prompt[]-explanation for user (How much numbers to write)
//! @param [in] number_of_input_elements number_of_input_elements - how much double values program should read
//!
//! @return reference to array of double values
//! 
//! @note if user could not write numbers with 100 attempts, program return NULL
//! @note number_of_input_elements should be less than IN_NUMBER
//--------------------------------------------------
double* InputNumbers(const char prompt[], int number_of_input_elements);

int Compare(double a,double b) {
   assert(isfinite(a));
   assert(isfinite(b));

   if (a - b > EPSILON) return 1;
   if (b - a > EPSILON) return -1;
   return 0;
}

int SolveLinearEquation(const double* const coef, double* const roots){
   assert(coef  != NULL);
   assert(roots != NULL);
   assert(isfinite(coef[0]));
   assert(isfinite(coef[1]));
   
   if (Compare(coef[0], 0) == 0) {  // Equation bx = 0
      if (coef[1] == 0) 
         return SSG_INFI;
      else
         return 0;
   }

   double solution = -(coef[1]) / (coef[0]);
   assert(isfinite(solution));

   *roots = solution;
   return 1;
}

int SolveSquareEquation(const double* const coef, double* const roots){
   assert(coef != NULL);
   assert(roots != NULL);

   assert(isfinite(coef[0]));
   assert(isfinite(coef[1]));
   assert(isfinite(coef[2]));
   
   double a = coef[0];
   double b = coef[1];
   double c = coef[2];

   if (Compare(a, 0) == 0) {
     int number_of_solutions = SolveLinearEquation((coef + 1), roots);
     return number_of_solutions;
   }

   double D = (b * b - 4 * a * c); //Discriminant
   if (Compare(D, 0) == -1) {  //No solutions
      return 0;
   }
   
   if (Compare(D, 0) == 0) {  //One solution
      double first_solution = -b / (2 * a);
      
      *roots = first_solution;
      return 1;
   }
   
   if (D > 0) {  //Two solutions
      D = sqrt(D);

      double first_solution  = (-b + D) / (2 * a);
      double second_solution = (-b - D) / (2 * a);
      
      roots[0] = first_solution;
      roots[1] = second_solution;
      return 2;
   }

   return -2;
}

double* InputNumbers(const char prompt[], int number_of_input_elements){
   assert(prompt != NULL);
   assert(number_of_input_elements > 0);
   assert(number_of_input_elements <= IN_NUMBER);

   printf("%s\n", prompt);
   printf("Give numbers through Enters\n");
   
   double* user_input = (double*) calloc(IN_NUMBER, sizeof(double));

   int number_of_read = 0;
   int fail_attempts = 0;
   while (number_of_read < number_of_input_elements) {
      
      if (scanf("%lg", &user_input[number_of_read]) == 1) {
         number_of_read++;
      }
      else {
         printf("Incorrect double value, please try again\n");
         ClearStdin();
         fail_attempts++;
      }
      
      if (fail_attempts >= MAXIMUM_ATTEMPTS) {
         printf("Something wrong with input\n");
         return NULL;
      }
   }
   
   return user_input;
}

