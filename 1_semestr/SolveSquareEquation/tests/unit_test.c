#include "../calc.c"
#include "unistd.h"

//!----------------------------------
//! Print error message
//!
//! @param [in] test_number test_number-number of failed test
//! @param [in] numbker_of_solutions number_of_solutions-number of solutions program gives
//! @param [in] real_number_of_solutions real_number_of_solutions-correct number of solutions
//! @param [in] coef coef-coefficients
//! @param [in] real_roots real_roots-correct roots
//! @param [in] roots roots-roots from program
//!
//!-----------------------------------
void PrintError(int test_number, int number_of_solutions, int real_number_of_solutions, double* coef, /
                double* real_roots, double* roots)          

//!-----------------------------------
//! Check program on random tests
//!
//! @param [int] test_number test_number-number of failed test
//!
//! @return 0, if all test passed, 1 otherwise
//!-----------------------------------
int TestOnRandomTests(int test_number)

//!-----------------------------------
//! Check one solution
//!
//! @param [in] test_number test_number-number of failed test
//! @param [in] numbker_of_solutions number_of_solutions-number of solutions program gives
//! @param [in] real_number_of_solutions real_number_of_solutions-correct number of solutions
//! @param [in] coef coef-coefficients
//! @param [in] real_roots real_roots-correct roots
//! @param [in] roots roots-roots from program
//!
//! @return 0, if test passed, 1 otherwise
//!-----------------------------------
int CheckSolution(int test_number, int number_of_solutions, int real_number_of_solutions, double* coef, /
                  double* real_roots, double* roots)

//!-----------------------------------
//! Open file
//!
//! @param [in] file_name file_name-name of file
//! @param [out] file file-reference to file
//!
//! @return 0, if files opened, 1 otherwise
//!-----------------------------------
int OpenFile(const char* file_name, FILE** file)

//!-----------------------------------
//! Read coefficients and roots for 1 test
//!
//! @param [in] test_number test_number-number of test
//! @param [in] coef_file coef_file-reference to file with coefficients
//! @param [in] roots_file roots_file-reference to file with roots
//! @param [out] real_number_of_solutions real_number_of_solutions-correct number of solutions
//! @param [out] coef coef-massive with coefficients 
//! @param [out] real_roots real_roots-massive with correct roots
//! 
//!
//! @return 0, if test it read, 1 otherwise
//!-----------------------------------
int ReadTest(int test_number, FILE* coef_file, FILE* roots_file, int* real_number_of_solutions, double* coef, /
             double* real_roots)


int main() {
   FILE* coef_file  = NULL;
   FILE* roots_file = NULL;
   if (OpenFile("coef_file", &coef_file) || OpenFile("coef_file", &coef_file)) {
      printf("Problem with opening files\n");
      return 1;
   }

   double* coef       = (double*) calloc(sizeof(double) * 3);
   double* real_roots = (double*) calloc(sizeof(double) * 2);
   double* roots      = (double*) calloc(sizeof(double) * 2);
   int real_number_of_solutions = 0;
   int test_number = 0;

   while (ReadTest(test_number, coef_file, roots_file, &real_number_of_solutions, coef, real_roots) == 0) {
      int number_of_solutions = SolveSquareEquation(coef, roots);
      int test_passed = CheckSolution(test_number, number_of_solutions, real_number_of_solutions, coef, real_roots, roots);      
      if (test_passed) return 1;
      test_number++;
   }
   
   int test_passed = TestOnRandomTests(test_number);
   if (test_passed) return 1;
   
   free(coef);
   free(real_roots);
   free(roots);

   printf("All tests passed successfully\n\n");
   return 0;
}

void PrintError(int test_number, int number_of_solutions, int real_number_of_solutions, double* coef, /
                double* real_roots, double* roots){
   printf("Error on %d test\n", test_number + 1);
   printf("Input: %lg, %lg, %lg\n", *(coef + 0), *(coef + 1), *(coef + 2));
   printf("Correct number of solutions: %d\n", real_number_of_solutions);
   printf("Your number of solutions: %d\n", number_of_solutions);
   printf("Correct output: %lg, %lg\n", *(real_roots + 0), *(real_roots + 1));
   printf("Your output: %lg, %lg\n", *(roots + 0), *(roots + 1));

   free(coef);
   free(roots);
   free(real_roots);
}  

int TestOnRandomTests(int test_number) {

   for (int i = test_number; i < test_number + 1000; ++i) {
      double* coef = (double*) calloc(sizeof(double) * 3);
      *(coef + 0) = rand();
      *(coef + 1) = rand();
      *(coef + 2) = rand();
      
      double* roots = (double*) calloc(sizeof(double) * 2);

      if (SolveSquareEquation(coef, roots) == -2) {
         PrintError(test_number, -2, -2, coef, roots, roots);
         return 1;
      }
   }
   
   free(coef);
   free(roots);
   return 0;
}

int CheckSolution(int test_number, int number_of_solutions, int real_number_of_solutions, double* coef, /
                  double* real_roots, double* roots){

   if (real_number_of_solutions != number_of_solutions) {
       PrintError(test_number, number_of_solutions, real_number_of_solutions, coef, real_roots, roots);
       return 1;
   }
   if (number_of_solutions == 1) 
       if (compare(*real_roots, *roots) != 0) {
          PrintError(test_number, number_of_solutions, real_number_of_solutions, coef, real_roots, roots);
          return 1;
       }
   if (number_of_solutions == 2) 
       if (  !((compare(*real_roots,       *roots) == 0 && compare(*(real_roots + 1), *(roots + 1)) == 0) ||
               (compare(*(real_roots + 1), *roots) == 0 && compare(*real_roots,       *(roots + 1)) == 0))   ) {
          PrintError(test_number, number_of_solutions, real_number_of_solutions, coef, real_roots, roots);
          return 1;  
       }

   return 0;
}

int OpenFile(const char* file_name, FILE** file){
   assert(access(file_name,  F_OK) != -1);
   assert(access(file_name,  R_OK) != -1);

   *file  = fopen(file_name, "r");

   if (*file == NULL) return 1;

   return 0;
}

int ReadTest(int test_number, FILE* coef_file, FILE* roots_file, int* real_number_of_solutions, double* coef, /
             double* real_roots){
   assert(coef != NULL);
   assert(real_roots != NULL);
   assert(real_number_of_solutions != NULL);
   assert(coef_file != NULL);
   assert(roots_file != NULL);

   char* coef_line  = NULL;
   char* roots_line = NULL;

   size_t len1 = 0; //!< just useless var for getline
   size_t len2 = 0; //!< just useless var for getline
   if (getline(&coef_line,  &len1, coef_file)  == -1) return 1;
   if (getline(&roots_line, &len2, roots_file) == -1) return 1;
   
   int read_from_scanf = sscanf(coef_line, "%d %lg %lg %lg\n", real_number_of_solutions, coef + 0, coef + 1, coef + 2);
   if (read_from_scanf != 4) {
      printf("Invalid format in coefficients file, on test:%d\n", test_number);
      return 1;
   }
   
   read_from_scanf = sscanf(roots_line, "%lg %lg\n", real_roots + 0, real_roots + 1);
   if (read_from_scanf != 2) {
      printf("Invalid format in roots file, on test:%d\n", test_number);
      return 1;
   }

   return 0;
}
