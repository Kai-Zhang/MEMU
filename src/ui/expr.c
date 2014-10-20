#include "common.h"
#include "memory.h"
#include "cpu/reg.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, NUM, HEX, REG, LE, GE, EQ, NEQ, AND, OR, NOT, SHL, SHR,
	BAND, BOR, XOR, BNOT, DEREF, NEG, POS
	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",		NOTYPE},			// white space
	{"\\+",		'+'},				// plus
	{"-",		'-'},				// minus
	{"\\*",		'*'},				// multiply
	{"\\/",		'/'},				// divide
	{"\\%",		'%'},				// mod
	{"<[^<]",	'<'},				// less than
	{">[^>]",	'>'},				// greater than
	{"<=",		LE},				// less or equal than
	{">=",		GE},				// greater or equal than
	{"==",		EQ},				// equal
	{"!=",		NEQ},				// not equal
	{"&&",		AND},				// logical and
	{"\\|\\|",	OR},				// logical or
	{"![^=]",	NOT},				// logical not
	{"<<",		SHL},				// shift left
	{">>",		SHR},				// shift right
	{"&[^&]",	BAND},				// bit and
	{"\\|[^\\|]",		BOR},		// bit or
	{"\\^",		XOR},				// exclusive or
	{"~",		BNOT},				// bit not
	{"\\(",		'('},				// left parenthese
	{"\\)",		')'},				// right parenthese
	{"0x[0-9A-Fa-f]+",	HEX},		// hexadecimal number
	{"[[:digit:]]+",	NUM},		// decimal number
	{"\\$[[:alpha:]]+",	REG}		// register
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			test(0, "regex compilation failed: %s\n%s\n", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match regex[%d] at position %d with len %d: %.*s", i, position, substr_len, substr_len, substr_start);

				position += substr_len;


				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */

				if(rules[i].token_type == NOTYPE) break;

				int trv = 0;
				switch(rules[i].token_type) {
					case NUM:	while(substr_start[trv] >= '0' && substr_start[trv] <= '9' && trv < 32) {
									tokens[nr_token].str[trv] = substr_start[trv]; ++trv;
								}
								tokens[nr_token].str[trv] = 0;		break;
					case HEX:	for(trv = 2; ((substr_start[trv] >= '0' && substr_start[trv] <= '9') || 
										(substr_start[trv] >= 'a' && substr_start[trv] <= 'f') ||
										(substr_start[trv] >= 'A' && substr_start[trv] <= 'F')) && trv < 32; ++trv) {
									tokens[nr_token].str[trv-2] = substr_start[trv];
								}
								tokens[nr_token].str[trv-2] = 0;	break;
					case REG:	for(trv = 1; ((substr_start[trv] >= 'a' && substr_start[trv] <= 'z') ||
										(substr_start[trv] >= 'A' && substr_start[trv] <= 'Z')) && trv < 32; ++trv) {
									tokens[nr_token].str[trv-1] = substr_start[trv];
								}
								tokens[nr_token].str[trv-1] = 0;	break;
					//default: assert(0);
				}
				tokens[nr_token].type = rules[i].token_type;
				++ nr_token;

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

bool check_parenthese(int start, int end) {
	if(tokens[start].type != '(' || tokens[end].type != ')') {
		return false;
	}
	
	int pair_check = 1;
	int checker = start + 1;
	for (; checker < end; ++checker) {
		if(tokens[checker].type == '(') {
			++ pair_check;
		} else if (tokens[checker].type == ')') {
			-- pair_check;
		}

		if(pair_check <= 0) {
			return false;
		}
	}
	return pair_check == 1;
}

int dominant_operator(int start, int end) {
	int op_pos = -1, weigh = -1, weigh_temp = -1;
	int pair_check = 0;
	int trv = start;
	for( ; trv <= end; ++trv) {
		switch(tokens[trv].type) {
			case '+':	weigh_temp = (pair_check > 0) ? -1 : 3;	break;
			case '-':	weigh_temp = (pair_check > 0) ? -1 : 3;	break;
			case '*':	weigh_temp = (pair_check > 0) ? -1 : 2;	break;
			case '/':	weigh_temp = (pair_check > 0) ? -1 : 2;	break;
			case '%':	weigh_temp = (pair_check > 0) ? -1 : 2;	break;
			case '<':	weigh_temp = (pair_check > 0) ? -1 : 5;	break;
			case '>':	weigh_temp = (pair_check > 0) ? -1 : 5;	break;
			case LE:	weigh_temp = (pair_check > 0) ? -1 : 5;	break;
			case GE:	weigh_temp = (pair_check > 0) ? -1 : 5;	break;
			case EQ:	weigh_temp = (pair_check > 0) ? -1 : 6;	break;
			case NEQ:	weigh_temp = (pair_check > 0) ? -1 : 6;	break;
			case AND:	weigh_temp = (pair_check > 0) ? -1 : 10;break;
			case OR:	weigh_temp = (pair_check > 0) ? -1 : 11;break;
			case NOT:	weigh_temp = (pair_check > 0) ? -1 : 1;	break;
			case SHL:	weigh_temp = (pair_check > 0) ? -1 : 4;	break;
			case SHR:	weigh_temp = (pair_check > 0) ? -1 : 4;	break;
			case BAND:	weigh_temp = (pair_check > 0) ? -1 : 7;	break;
			case BOR:	weigh_temp = (pair_check > 0) ? -1 : 9;	break;
			case XOR:	weigh_temp = (pair_check > 0) ? -1 : 8;	break;
			case BNOT:	weigh_temp = (pair_check > 0) ? -1 : 1;	break;
			case DEREF:	weigh_temp = (pair_check > 0) ? -1 : 1;	break;
			case POS:	weigh_temp = (pair_check > 0) ? -1 : 1;	break;
			case NEG:	weigh_temp = (pair_check > 0) ? -1 : 1;	break;
			case '(':	weigh_temp = -1;	++ pair_check;		break;
			case ')':	weigh_temp = -1;	-- pair_check;		break;
			default:	weigh_temp = -1;
		}

		if(pair_check < 0)	return -1;
		if(weigh_temp > weigh) {
			weigh = weigh_temp;
			op_pos = trv;
		}
	}
	
	return op_pos;
}

uint32_t eval(int start, int end, bool *success) {
	if(start > end) {
		*success = false;
		return 0;
	} else if(start == end) {
		*success = true;
		int i = 0;
		switch(tokens[start].type) {
			case NUM:	return (uint32_t)strtol(tokens[start].str, NULL, 10);
			case HEX:	return (uint32_t)strtol(tokens[start].str, NULL, 16);
			case REG:	for(i = R_EAX; i <= R_EDI; ++i)
							if(strcasecmp(regsl[i], tokens[start].str) == 0)
								return reg_l(i);
						if(strcasecmp("eip", tokens[start].str) == 0)
							return cpu.eip;
						if(strcasecmp("eflags", tokens[start].str) == 0)
							return cpu.eflags.value;
						*success = false;	return 0;
			default:	*success = false;	return 0;
		}
	} else if(check_parenthese(start, end)) {
		return eval(start + 1, end - 1, success);
	} else {
		int op_pos = dominant_operator(start, end);
		if(op_pos == -1) {
			*success = false;
			return 0;
		}
		uint32_t rhs = eval(op_pos + 1, end, success);
		if(!(*success)) return 0;
		switch(tokens[op_pos].type) {
			case POS:	return rhs;
			case NEG:	return -rhs;
			case NOT:	return !rhs;
			case BNOT:	return ~rhs;
			case DEREF:	return swaddr_read(rhs, 4);
		}
		uint32_t lhs = eval(start, op_pos - 1, success);
		if(!(*success)) return 0;

		switch(tokens[op_pos].type) {
			case '+':	return lhs + rhs;
			case '-':	return lhs - rhs;
			case '*':	return lhs * rhs;
			case '/':	return lhs / rhs;
			case '%':	return lhs % rhs;
			case '<':	return lhs < rhs;
			case '>':	return lhs > rhs;
			case LE:	return lhs <= rhs;
			case GE:	return lhs >= rhs;
			case EQ:	return lhs == rhs;
			case NEQ:	return lhs != rhs;
			case AND:	return lhs && rhs;
			case OR:	return lhs || rhs;
			case SHL:	return lhs << rhs;
			case SHR:	return lhs >> rhs;
			case BAND:	return lhs & rhs;
			case BOR:	return lhs | rhs;
			case XOR:	return lhs ^ rhs;
			default:	assert(0);
		}
	}
}

uint32_t expr_calc(char *e, bool *success) {
	if(!success) {
		bool temp = true;
		success = &temp;
	}
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Implement code to evaluate the expression. */

	int i = 0;
	for (; i < nr_token-1; ++i) {
		if(i == 0 || (tokens[i-1].type != ')' && tokens[i-1].type != NUM && tokens[i-1].type != HEX && tokens[i-1].type != REG)) {
			switch(tokens[i].type) {
				case '*':	tokens[i].type = DEREF;		break;
				case '+':	tokens[i].type = POS;		break;
				case '-':	tokens[i].type = NEG;		break;
			}
		}
	}
	return eval(0, nr_token-1, success);
}

