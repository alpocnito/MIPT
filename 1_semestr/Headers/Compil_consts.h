#define COMMAND_SIZE 1

#define SIGNATURE 'NORI'
#define VERSION (char)1

const unsigned int MAX_LEN_COMMAND = 20; // for InterpretateLine function

const char LEN_REGISTER_NAME = 3;
const char REGISTER_SHIFT = 100;
const char OPERATIVE_SHIFT = 50;
const unsigned int SIZE_OPERATIVE = 100;

const unsigned int MAX_AMOUNT_LABELS = 1024;
const unsigned int MAX_LEN_LABEL = 20;


typedef int Label_t;
