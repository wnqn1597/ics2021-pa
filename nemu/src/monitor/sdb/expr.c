#include <isa.h>
#include <memory/host.h>
#include <memory/paddr.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ, TK_NEQ, TK_AND, TK_REGNAME,
  TK_DEC, TK_HEX, TK_LPA, TK_RPA, TK_NEG, TK_DEREF, TK_REG

  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"-", '-'},		// minus
  {"\\*", '*'},		// multiply
  {"/", '/'},		// divide
  {"==", TK_EQ},        // equal
  {"!=", TK_NEQ},	// not equal
  {"&&", TK_AND},	// logic and
  {"([a-z]{1,2}[0-9]{0,2})|(\\$0)", TK_REGNAME},	// register name
  {"0x[0-9a-f]+", TK_HEX},	// hexadecimal
  {"[0-9]+", TK_DEC},	// decimal
  {"\\(", TK_LPA},	// left_parenthese
  {"\\)", TK_RPA},	// right_parenthese
  {"_", TK_NEG},	// negative
  {"~", TK_DEREF},	// dereference
  {"\\$", TK_REG},	// visit register
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static void refresh(){
  for(int i = 0; i < 32; i++){
    tokens[i].type = 0;
    memset(tokens[i].str, 0, 32);
  }
  nr_token = 0;
}

static inline bool judge(char *c){
  return *c == '+' || *c == '-' || *c == '*' || *c == '/' || *c == '(' || *c == '_';
}

static bool make_token(char *s) {
  char e[strlen(s)+1];
  strcpy(e, s);
  char *_e = e;
  if(*_e == '-') *_e = '_';
  if(*_e == '*') *_e = '~';
  //_e++;
  while(*_e != '\0'){
    if(judge(_e)){
      if(*(_e+1) == '-') *(_e+1) = '_';
      else if(*(_e+1) == '*') *(_e+1) = '~';
    }
    _e++;
  }

  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
		//TODO: len > 32
        switch (rules[i].token_type) {
	  	  case TK_NOTYPE: break;
		  case TK_REGNAME:
	  	  case TK_DEC:
	  	  case TK_HEX: strncpy(tokens[nr_token].str, substr_start, substr_len);
          default: tokens[nr_token++].type = rules[i].token_type;
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }
  return true;
}

bool check_parentheses(int p, int q){
  if(tokens[p].type != TK_LPA || tokens[q].type != TK_RPA)return false;
  int count = 0;
  bool ret = true;
  for(int i = p; i <= q; i++){
    if(tokens[i].type == TK_LPA) count++;
    else if(tokens[i].type == TK_RPA) count--;
    if(count < 0) Log("Bad expression.");
    else if(count == 0 && i != q) ret = false;
  }
  return ret;
}

int get_main_token(int p, int q){
  int count = 0;
  bool chosen = false;
  int ret = -1;
  for(int i = q; i >= p; i--){
    if(tokens[i].type == TK_RPA) count++;
    else if(tokens[i].type == TK_LPA) count--;
    if(count > 0) continue;
    if(!chosen && (tokens[i].type == '*' || tokens[i].type == '/')){
      ret = i;
      chosen = true;
    }else if(tokens[i].type == '+' || tokens[i].type == '-') return i;
  }
  return ret;
}

word_t eval(int p, int q){
  if(p > q){
    Log("Bad expression.");
    assert(0);
  }else if(p == q){
    if(tokens[p].type == TK_HEX) return (word_t)strtol(tokens[p].str, (char**)NULL, 16);
    else if(tokens[p].type == TK_DEC) return (word_t)atoi(tokens[p].str);
    else if(tokens[p].type == TK_REGNAME) return regname_to_index(tokens[p].str); //TODO
    else assert(0);
  }else if(check_parentheses(p, q) == true){
    return eval(p+1, q-1);
  }else{
    int index = get_main_token(p, q);
    if(index == -1){
      if(tokens[p].type == TK_NEG) return -1 * eval(p+1, q);
      else if(tokens[p].type == TK_DEREF){
        return host_read(guest_to_host(eval(p+1, q)), 4);     
      }else if(tokens[p].type == TK_REG){
	bool success;
        word_t index = eval(p+1, q);
	if(index >= 32||index < 0)assert(0);
        word_t ret = isa_reg_str2val(regs[index], &success);
	if(success)return ret;
	else assert(0);
      }
    }
    word_t val1 = eval(p, index-1);
    word_t val2 = eval(index+1, q);

    switch(tokens[index].type){
      case '+': return val1 + val2;
      case '-': return val1 - val2;
      case '*': return val1 * val2;
      case '/': return val1 / val2;
      default: assert(0);
    }
  }

}


word_t expr(char *e, bool *success) {
  init_regex();
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  word_t ret = eval(0, nr_token-1);
  refresh();
  *success = true;
  return ret;
}

