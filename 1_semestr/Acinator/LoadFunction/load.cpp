void SkipSpaces(FILE* file) {
  assert(file);
  
  char c = '-';
  while ((c = fgetc(file)) == ' ' || c == '\t' || c == '\n');
  
  ungetc(c, file);
}


int TreeLoad(Tree* tree, const char* file_name, int mode) {
  assert(tree);
  assert(file_name);

  TreeInit(tree);
  tree->number_nodes = 1;
  
  FILE* input = NULL;
  if (OpenFile(file_name, "r", &input));
  
  char first_char = fgetc(input);
  if (first_char != '{') return 1;
    

  switch (mode) {
    
    case PREORDER:
      PreorderLoadNodes(tree, &(tree->root), input);
      break;
    

    case INORDER:
      InorderLoadNodes(tree, &(tree->root), input);
      break;

    case POSTORDER: 
      PostorderLoadNodes(tree, &(tree->root), input);
      break;

    default:
      return 1;
  }

  return 0;
}


int PreorderLoadNodes(Tree* tree, Node** node, FILE* file) {
  assert(tree);
  assert(file);
  assert(node);
  assert(*node);
  
  // saving data
  (*node)->data = (tElem_t) calloc(MAX_LEN_NODE_DATA, sizeof(char));
  SkipSpaces(file);
  fscanf(file, "\"%[^\"]\"", (*node)->data);
  SkipSpaces(file);

  // saving right node
  char next_symbol = fgetc(file);
  if (next_symbol == 'n') {
    next_symbol = fgetc(file);
    next_symbol = fgetc(file);
    (*node)->right = NULL; 
  } 
  else {
    if (TreePush(tree, &((*node)->right), "-"))
      return 1;
    PreorderLoadNodes(tree, &((*node)->right), file);
  }
  
  // saving left node
  next_symbol = fgetc(file);
  if (next_symbol == 'n') {
    next_symbol = fgetc(file);
    next_symbol = fgetc(file);
    (*node)->left = NULL; 
  } 
  else {
    if (TreePush(tree, &((*node)->left), "-"))
      return 1;
    PreorderLoadNodes(tree, &((*node)->left), file);
  }
  
  // reading '}'
  next_symbol = fgetc(file);

  return 0;
}

int InorderLoadNodes(Tree* tree, Node** node, FILE* file) {
  assert(tree);
  assert(file);
  assert(node);
  assert(*node);
  
  // saving left node
  char next_symbol = fgetc(file);
  if (next_symbol == 'n') {
    next_symbol = fgetc(file);
    next_symbol = fgetc(file);
    (*node)->left = NULL; 
  } 
  else {
    if (TreePush(tree, &((*node)->left), "-"))
      return 1;
    InorderLoadNodes(tree, &((*node)->left), file);
  }
   
  // saving data
  (*node)->data = (tElem_t) calloc(MAX_LEN_NODE_DATA, sizeof(char));
  SkipSpaces(file);
  fscanf(file, "\"%[^\"]\"", (*node)->data);
  SkipSpaces(file);

  // saving right node
  next_symbol = fgetc(file);
  if (next_symbol == 'n') {
    next_symbol = fgetc(file); next_symbol = fgetc(file);
    (*node)->right = NULL; 
  } 
  else {
    if (TreePush(tree, &((*node)->right), "-"))
      return 1;
    InorderLoadNodes(tree, &((*node)->right), file);
  }
  
 
  // reading '}'
  next_symbol = fgetc(file);

  return 0;
}

int PostorderLoadNodes(Tree* tree, Node** node, FILE* file) {
  assert(tree);
  assert(file);
  assert(node);
  assert(*node); 
  
  // saving left node
  char next_symbol = fgetc(file);
  if (next_symbol == 'n') {
    next_symbol = fgetc(file);
    next_symbol = fgetc(file);
    (*node)->left = NULL; 
  } 
  else {
    if (TreePush(tree, &((*node)->left), "-"))
      return 1;
    PostorderLoadNodes(tree, &((*node)->left), file);
  }
 
  // saving right node
  next_symbol = fgetc(file);
  if (next_symbol == 'n') {
    next_symbol = fgetc(file);
    next_symbol = fgetc(file);
    (*node)->right = NULL; 
  } 
  else {
    if (TreePush(tree, &((*node)->right), "-"))
      return 1;
    PostorderLoadNodes(tree, &((*node)->right), file);
  } 

  // saving data
  (*node)->data = (tElem_t) calloc(MAX_LEN_NODE_DATA, sizeof(char));
  SkipSpaces(file);
  fscanf(file, "\"%[^\"]\"", (*node)->data);
  SkipSpaces(file);


  // reading '}'

  next_symbol = fgetc(file);

  return 0;
}

