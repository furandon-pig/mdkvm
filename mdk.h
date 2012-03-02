/*
 * mdk.h
 */

#ifndef _MDK_H_
#define _MDK_H_

#define _printf(args...)			\
	do {					\
		if (isatty(fileno(stdin)))	\
			printf(args);		\
	} while (0)

#define MDK_STACK_MAX		1024
#define MDK_TOEKN_MAX		1024
#define MDK_REGISTER_MAX	9

typedef enum {
	MDK_REGISTER_FREE = 0,
	MDK_REGISTER_USING
} mdk_register_state_t;

typedef enum {
	MDK_TOKEN_VALUE = 1,
	MDK_TOKEN_ADD,
	MDK_TOKEN_SUB,
	MDK_TOKEN_MUL,
	MDK_TOKEN_DIV,
} mdk_token_type;

typedef struct {
	int val[MDK_STACK_MAX];
	int idx;
} mdk_stack_t;

typedef struct {
	int val[MDK_REGISTER_MAX];
} mdk_register_t;

typedef struct {
	char *token;
	mdk_token_type type;
} mdk_token_t;

typedef struct {
	mdk_stack_t stk;
	mdk_register_t reg;
	mdk_token_t tkn[MDK_TOEKN_MAX];
} mdk_context_t;

#endif	/* _MDK_H_ */
