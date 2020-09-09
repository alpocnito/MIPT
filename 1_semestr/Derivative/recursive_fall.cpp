const char* main_string = "";

Node* GetG(const char* str);
Node* GetE();
Node* GetT();
Node* GetPow();
Node* GetP();

Node* GetN();
Node* GetF();
Node* GetId();

Node* GetG(const char* str) {
  main_string = str;
  
  Node* node = GetE();
  assert(*main_string == '\0');

  return node;
}


Node* GetE() {
  Node* node = GetT();

  while (*main_string == '+' || *main_string == '-') {
    char op = *main_string;
    main_string++;
    Node* node2 = GetT();

    if (op == '+') node = TreeSetNode(node, node2, OPERATOR_PLUS,  TYPE_OPERATOR);
    else           node = TreeSetNode(node, node2, OPERATOR_MINUS, TYPE_OPERATOR);
  }

  return node;
}

Node* GetT() {
  Node* node = GetPow();

  if (*main_string == '*' || *main_string == '/') {
    char op = *main_string;
    main_string++;
    Node* node2 = GetPow();

    if (op == '*') node = TreeSetNode(node, node2, OPERATOR_MULT, TYPE_OPERATOR);
    else           node = TreeSetNode(node, node2, OPERATOR_DIV,  TYPE_OPERATOR);

  }

  return node;
}

Node* GetPow() {
 Node* node = GetP();
 
 if (*main_string == '^') {
    main_string++;
    Node* node2 = GetP();

    node = TreeSetNode(node, node2, OPERATOR_POW, TYPE_OPERATOR);

  }

  return node;
}

Node* GetP() {
  Node* node = NULL;
  
  if (*main_string == '(') {
    main_string++;
    
    node = GetE();
    assert(*main_string == ')');
    main_string++;
    goto end;
  }
  
  if (isdigit(*main_string)) {
    node = GetN();
    goto end;
  }

  if (isalpha(*main_string)) {
    node = GetF();
    
    if (node != NULL) goto end;
  }

  if (isalpha(*main_string)) { 
    node = GetId();    
    
    if (node != NULL) goto end;
  }

end:
  assert(node != NULL);
  return node;
}

Node* GetN() {
  Node* node = CreateNode(0, TYPE_NUMBER);

  while (isdigit(*main_string)) {
    node->data.value = node->data.value * 10 + int(*main_string - '0');
    main_string++;
  }
  
  return node;
}

Node* GetId() {
  Node* node = CreateNode(0, TYPE_VARIABLE);

  if (isalpha(*main_string)) {
    node->data.type = *main_string;
    main_string++;
  }
  else {
    TreeDelete(&node);
    node = NULL;
  }
  
  return node;
}

Node* GetF() {
  Node* node = CreateNode(0, TYPE_FUNCTION);

  char symbols[MAX_LEN_FUNCTION] = {};
  int read_symbols = 0;
  sscanf(main_string, "%[a-z]%n", symbols, &read_symbols);
  read_symbols++;

  for (int i = 0; i < NUMBER_FUNCTIONS; ++i) {
    if (strcmp(symbols, functions[i].name) == 0) {
      
      (node)->data.type = i;
      main_string += read_symbols - 1;
        
      node->right = GetP();
      return node;
    }
  }

  TreeDelete(&node);

  return NULL;
}

void ClearEquation(char* input_equation, int size_equation) {
  
  int new_i = 0;
  for (int i = 0; i < size_equation; ++i)
    if (input_equation[i] != ' ') {
      input_equation[new_i] = input_equation[i];
      new_i++;
    }
}


Node* TreeLoad(const char* file_name) {

  FILE* input_file = fopen(file_name, "r");
  int size_input = FileSizeInBytes(input_file);

  char* input_equation = (char*)Calloc(size_input + 1, sizeof(char), input_equation);
  char* first_symbol_input_equation = input_equation;

  fscanf(input_file, "%[^\n]", input_equation);
  ClearEquation(input_equation, size_input);

  Node* tree = GetG(input_equation);

  //NodePrint(tree, "tree.doxy");
  
  Free(first_symbol_input_equation);
  return tree;
}
