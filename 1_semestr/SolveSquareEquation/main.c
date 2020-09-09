#include "calc.c"

int main(){
   printf("Square solver\n(c) Ruslan\n\n");
     
   double* coef = InputNumbers("Give 3 number", 3);
   double roots[2];

   int number_of_solutions = SolveSquareEquation(coef, roots); 
   
   switch (number_of_solutions) {
      case 0:
         printf("No solutions\n");
         break;
      case 1:
         printf("One solution: %lg\n", *roots);
         break;
      case 2:
         printf("Solutions: %lg, %lg\n", *roots, *(roots + 1));
         break;
      case SSG_INFI:
         printf("Infinity Solutions on R\n");
         break;
      default:
         printf("Error in SolveSquareEquation function\n");
         free(coef);
         return 1;
   }
   
   free(coef);
   return 0;
}
