#include <math.h>

#define POP StackPop(&(cpu.stack), &error);                                   \
            if (error != 0) { \
               ExecutionError("Can not pop element", file_name, number_line, cpu, PC); \
                                                                              \
               number_line++;                                                 \
               break;                                                         \
            }                                                                 \


#define PUSH(push_element) StackPush(&(cpu.stack), (push_element));             \
            if (error == 0) {                                                   \
               ExecutionError("Can not push element", file_name, number_line, cpu);  \
                                                                                \
               number_line++;                                                   \
               break;                                                           \
            }

#define JUMP_IF(SIGN) int error = 0;              \
                                                  \
                  Elem_t first_op  = POP;         \
                  Elem_t second_op = POP;         \
                                                  \
                  if (first_op SIGN second_op)    \
                     PC = *((int*)(buffer + PC)); \
                  else                            \
                     PC += sizeof(int);           \



//! DEF_CMD( <Name of the command>, 
//!          <Number of command>, 
//!          <Code for CPU> 
//!        )
//!

DEF_CMD(PUSH, 1, { 
   int error = 0;

   Elem_t push_value = *( (Elem_t*)(buffer + PC) );

   PC += sizeof(Elem_t);
   error = PUSH(push_value);
})

DEF_CMD(PUSH, 51, { 
   int error = 0;

   char* registe = buffer + PC;
   PC += LEN_REGISTER_NAME;
   
   int number_var = 0;
   assert(sscanf(registe + 1, "%02d", &number_var));
   number_var = *(registe + 2) - '0';
   
   error = PUSH(cpu.operativ[number_var + int(cpu.dx)]);
})


DEF_CMD(PUSH, 101, { 
   int error = 0;

   char* registe = buffer + PC;
   PC += LEN_REGISTER_NAME - 1;
      
   if (registe[1] == 'x') {
      
      if (registe[0] == 'a')
         { error = PUSH(cpu.ax); }

      if (registe[0] == 'b')
         { error = PUSH(cpu.bx); }

      if (registe[0] == 'c')
         { error = PUSH(cpu.cx); }
     
      if (registe[0] == 'd')
         { error = PUSH(cpu.dx); }

   }
})



DEF_CMD(POP, 2, {
   int error = 0;

   Elem_t temp_var = POP

})

DEF_CMD(POP, 102, { 
   int error = 0;
      
   char* registe = buffer + PC;
   PC += LEN_REGISTER_NAME - 1;

   if (registe[1] == 'x') {
      
      if (registe[0] == 'a')
         { cpu.ax = POP; }

      if (registe[0] == 'b')
         { cpu.bx = POP; }

      if (registe[0] == 'c')
         { cpu.cx = POP; }
     
      if (registe[0] == 'd')
         { cpu.dx = POP; }
   }
})


DEF_CMD(POP, 52, { 
   int error = 0;
      
   char* registe = buffer + PC;
   PC += LEN_REGISTER_NAME;
 
   int number_var = 0;
   assert(sscanf(registe + 1, "%02d", &number_var));
    
   cpu.operativ[number_var + int(cpu.dx)] = POP;
})


DEF_CMD(ADD, 3, { 
   int error = 0;

   Elem_t first_op  = POP
   Elem_t second_op = POP

   error = PUSH(first_op + second_op);

})

DEF_CMD(PRINT, 4, {
   int error = 0;

   Elem_t last_element = POP;

   PRINT_MAGENTA_E(N(STACK_SYMBOL "\n"), last_element);
   error = PUSH(last_element);
})

DEF_CMD(DIV, 5, {
   int error = 0;

   Elem_t first_op  = POP;
   Elem_t second_op = POP;

   error = PUSH(second_op / first_op);
})

DEF_CMD(MULT, 6, { 
   int error = 0;

   Elem_t first_op  = POP;
   Elem_t second_op = POP;
    
   error = PUSH(first_op * second_op);
})

DEF_CMD(SUB, 7, { 
   int error = 0;

   Elem_t first_op  = POP;
   Elem_t second_op = POP;

   error = PUSH(second_op - first_op);
})

DEF_CMD(PRINTA, 8, {
   printf("%lu: \nax: " STACK_SYMBOL "\nbx: " STACK_SYMBOL "\ncx: " STACK_SYMBOL "\ndx: " STACK_SYMBOL "\n", \
          PC, cpu.ax, cpu.bx, cpu.cx, cpu.dx);
   PrintStack(&(cpu.stack));
   
   printf("---------------------------------------------\n");
   for (int i = 10; i < 30; ++i)
     printf("var_%d: %lf\n", i, cpu.operativ[i]);

})

DEF_CMD(JMP, 9, {
   PC = *((int*)(buffer + PC));

})

DEF_CMD(JA, 10, {
   JUMP_IF(>);
})

DEF_CMD(JB, 11, {
   JUMP_IF(<);
})

DEF_CMD(JAE, 12, {
   JUMP_IF(>=);
})

DEF_CMD(JBE, 13, {
   JUMP_IF(<=);
})

DEF_CMD(JE, 14, {
   JUMP_IF(==);
})

DEF_CMD(JNE, 15, {
   JUMP_IF(!=);
})

DEF_CMD(CALL, 16, {
   int error = 0; 
   cpu.dx += 10;   

   error = PUSH((Elem_t)(PC + sizeof(Label_t)));

   PC = *((Label_t*)(buffer + PC));
})

DEF_CMD(RETURN, 17, {
   int error = 0;
   cpu.dx -= 10;
   
   Elem_t ret_position = POP

   PC = (Label_t) ret_position;
})

DEF_CMD(PWR, 18, {
   int error = 0;

   Elem_t first_op  = POP
   Elem_t second_op = POP

   error = PUSH((Elem_t)pow(second_op, first_op));
})

DEF_CMD(PUT, 19, {
   int error = 0;

   Elem_t read_data = 0;
   scanf("%lg", &read_data);
   error = PUSH(read_data);
})

DEF_CMD(END, 20, {
   return;
})
