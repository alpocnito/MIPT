/* All commands of my DSL language
 
  - PUT_BYTE()
  - PUT_WORD()
  - PUT_5BYTE()
  - CALL
  - FIXUP
  - MOV_XMM_RSP()
  - MOV_RSP_XMM7
  - ADD_RSP
  - SUB_RSP 
  - JUMP_IF()
  - JMP_OVER_QWORD
  - MATH

*/


#define PUT_BYTE(BYTE)    \
  output[x86_c++] = BYTE;

#define PUT_QWORD(BYTE1, BYTE2, BYTE3, BYTE4)\
  PUT_BYTE(BYTE1);        \
  PUT_BYTE(BYTE2);        \
  PUT_BYTE(BYTE3);        \
  PUT_BYTE(BYTE4);        

#define PUT_5BYTE(BYTE1, BYTE2, BYTE3, BYTE4, BYTE5)\
  PUT_BYTE(BYTE1);        \
  PUT_BYTE(BYTE2);        \
  PUT_BYTE(BYTE3);        \
  PUT_BYTE(BYTE4);        \
  PUT_BYTE(BYTE5);
  

#define CALL \
  PUT_BYTE(0xE8)

#define FIXUP                 \
  fixups.push_back({          \
                              \
    /*x86_location*/          \
    x86_c - 1,                \
                              \
    /*ams_location from code*/\                     
    *(int*)(input + ams_c),   \
                              \
    /*spot for inject jump*/  \                    
    output + x86_c,           \
                              \
    /*Size of jump*/          \
    5                         \
  });

#define MOV_XMM_RSP(NUM) \
  PUT_5BYTE(0xF2, 0x0F, 0x10, 0x04 + 8 * (NUM), 0x24); 

#define MOV_RSP_XMM7 \
  PUT_5BYTE(0xF2, 0x0F, 0x11, 0x3C, 0x24);
  
#define ADD_RSP \
  PUT_QWORD(0x48, 0x83, 0xC4, 0x08)
   
#define SUB_RSP  \
  PUT_QWORD(0x48, 0x83, 0xEC, 0x08)

#define JUMP_IF(NUM)                   \
  MOV_XMM_RSP(6)                       \
  ADD_RSP                              \
  MOV_XMM_RSP(7)                       \
  ADD_RSP                              \
                                       \
  /* comisd xmm6, xmm7*/               \
  PUT_QWORD(0x66, 0x0F, 0x2F, 0xF7);   \
                                       \
  /* jmp 0x0040... */                  \
  PUT_BYTE(0x0F)                       \
  PUT_BYTE(NUM)                        \
                                       \
  fixups.push_back({                   \
                                       \
    /*x86_location*/                   \
    x86_c - 2,                         \
                                       \
    /*ams_location from code*/         \                     
    *(int*)(input + ams_c),            \
                                       \
    /*spot for inject jump**/          \
    output + x86_c,                    \
                                       \
    /*Size of jump*/                   \
    6                                  \
  });                                  \
                                       \
                                       \
  ams_c += 4;                          \
  x86_c += 4;        


#define MATH(BYTE)                           \
  MOV_XMM_RSP(6)                             \
  ADD_RSP                                    \
  MOV_XMM_RSP(7)                             \
  ADD_RSP                                    \
                                             \
  /* Operatioin */                           \
  PUT_5BYTE(0xF2, 0x0F, (BYTE), 0xFE, 0x48); \
                                             \
  SUB_RSP                                    \
  MOV_RSP_XMM7


#define JMP_OVER_QWORD    \
  PUT_BYTE(0xEB);         \
  PUT_BYTE(0x08);         

//! SYMBOL ( <Name of the command>, 
//!          <Number of command>, 
//!          <Code for CPU> 
//!        )
//!

SYMBOL(PUSH, 1, { 
  
  JMP_OVER_QWORD

  size_t start_num = x86_c;           //
  for (int i = 0; i < 8; ++i)         // push number in the code
    output[x86_c++] = input[ams_c++]; //
    
  // movsd xmm7, [start_num]
  PUT_5BYTE(0xF2, 0x0F, 0x10, 0x3C, 0x25);

  *(size_t*)(output + x86_c) = start_num + offset;
  x86_c += 4;
  
  SUB_RSP
  MOV_RSP_XMM7
})

SYMBOL(PUSH, 51, {    
  int number_var = 10 * (input[ams_c + 1] - '0') + (input[ams_c + 2] - '0');

  // movsd xmm7, [rbp + number_var]
  PUT_5BYTE(0xF2, 0x0F, 0x10, 0x7d, (number_var * 8));
  
  SUB_RSP
  MOV_RSP_XMM7
 
  ams_c += LEN_REGISTER_NAME;
})


SYMBOL(PUSH, 101, { 
  SUB_RSP

  // movsd [rsp], xmm
  PUT_5BYTE(0xF2, 0x0F, 0x11, 0x04 + 8 * (input[ams_c] - FIRST_AMS_VAR), 0x24);
  ams_c += LEN_REGISTER_NAME - 1;
})



SYMBOL(POP, 2, {
  ADD_RSP
})

SYMBOL(POP, 102, {
   
  // movsd [rsp], xmm
  PUT_5BYTE(0xF2, 0x0F, 0x10, 0x04 + 8 * (input[ams_c] - FIRST_AMS_VAR), 0x24); 
  ams_c += LEN_REGISTER_NAME - 1;
  
  ADD_RSP
})


SYMBOL(POP, 52, { 
   int number_var = 10 * (input[ams_c + 1] - '0') + (input[ams_c + 2] - '0');
      
   MOV_XMM_RSP(7)
     
   // movsd [rsp], xmm
   PUT_5BYTE(0xF2, 0x0F, 0x11, 0x7d, number_var * 8);
   ams_c += LEN_REGISTER_NAME;
 
   ADD_RSP
})


SYMBOL(ADD, 3, { 
  MATH(0x58);
})


SYMBOL(DIV, 5, {
  MATH(0x5E);
})

SYMBOL(MULT, 6, { 
  MATH(0x59);
})

SYMBOL(SUB, 7, {  
  MATH(0x5C);
})

SYMBOL(PWR, 18, {
  MOV_XMM_RSP(6)
  ADD_RSP
  MOV_XMM_RSP(7)
  ADD_RSP

  CALL //root
  *(int*)(output + x86_c) = -stdlib_size - x86_c + ROOT_function;
  x86_c += 4; 
 
  SUB_RSP
  MOV_RSP_XMM7

})

SYMBOL(JMP, 9, {
  
  // jmp
  PUT_BYTE(0xE9);
  FIXUP

  ams_c += 4;
  x86_c += 4;

})

SYMBOL(JA, 10, {
   JUMP_IF(0x87);
})

SYMBOL(JB, 11, {
   JUMP_IF(0x82);
})

SYMBOL(JAE, 12, {
   JUMP_IF(0x83);
})

SYMBOL(JBE, 13, {
   JUMP_IF(0x86);
})

SYMBOL(JE, 14, {
   JUMP_IF(0x84);
})

SYMBOL(JNE, 15, {
  JUMP_IF(0x85);
})


SYMBOL(CALL, 16, {
 
  CALL
  FIXUP

  ams_c += 4;
  x86_c += 4;

})

SYMBOL(RETURN, 17, {
  PUT_BYTE(0xC3);
})

SYMBOL(PUT, 19, {
  CALL
  
  *(int*)(output + x86_c) = -stdlib_size - x86_c + GET_function;
  x86_c += 4; 
})

SYMBOL(PRINT, 4, {
  CALL
  
  *(int*)(output + x86_c) = -stdlib_size - x86_c + PUT_function;
  x86_c += 4; 
})

SYMBOL(END, 20, {
  // mov eax, 0x3c
  PUT_5BYTE(0xB8, 0x3C, 0x00, 0x00, 0x00); 
  // mov edi, 0
  PUT_5BYTE(0xBF, 0x00, 0x00, 0x00, 0x00); 
  
  // syscall
  PUT_BYTE(0x0F);
  PUT_BYTE(0x05);
})
