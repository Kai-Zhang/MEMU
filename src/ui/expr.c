#include "common.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, NUM

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
	{"==",		EQ},				// equal
	{"[[:digit:]]+",	NUM},		// number
	{"-",		'*'},				// minus
	{"\\*",		'-'},				// multiply
	{"\\/",		'/'},				// divide
	{"\\(",		'('},				// left parenthese
	{"\\)",		')'},				// right parenthese
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
								tokens[nr_token].str[trv] = 0;	break;
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
	
	int pair_check = 0;
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
	return !pair_check;
}

int dominant_operator(int start, int end) {
	int op_pos = -1, weigh = -1, weigh_temp = 0;
	int pair_check = 0;
	int trv = start;
	for( ; trv <= end; ++trv) {
		switch(tokens[trv].type) {
			case '+':	weigh_temp = (pair_check > 0) ? -1 : 3;	break;
			case '-':	weigh_temp = (pair_check > 0) ? -1 : 3;	break;
			case '*':	weigh_temp = (pair_check > 0) ? -1 : 2;	break;
			case '/':	weigh_temp = (pair_check > 0) ? -1 : 2;	break;
			case '(':	++ pair_check;							break;
			case ')':	-- pair_check;							break;
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
		return (uint32_t)strtol(tokens[start].str, NULL, 0);
	} else if(check_parenthese(start, end)) {
		return eval(start + 1, end - 1, success);
	} else {
		int op_pos = dominant_operator(start, end);
		if(op_pos == -1) {
			*success = false;
			return 0;
		}
		uint32_t lhs = eval(start, op_pos - 1, success);
		if(!(*success)) return 0;
		uint32_t rhs = eval(op_pos + 1, end, success);
		if(!(*success)) return 0;

		switch(tokens[op_pos].type) {
			case '+':	return lhs + rhs;
			case '-':	return lhs - rhs;
			case '*':	return lhs * rhs;
			case '/':	return lhs / rhs;
			default:	assert(0);
		}
	}
}

uint32_t expr_calc(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Implement code to evaluate the expression. */
	return eval(0, nr_token-1, success);
}

