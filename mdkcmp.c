/*
 * mdkcmp.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "mdk.h"

int capt_register(mdk_context_t *ctx)
{
	int i;

	for (i = 0; i < MDK_REGISTER_MAX; i++) {
		if (ctx->reg.val[i] == MDK_REGISTER_FREE) {
			ctx->reg.val[i] = MDK_REGISTER_USING;
			return i;
		}
	}

	return -1;
}

int free_register(mdk_context_t *ctx, int regnum)
{
	ctx->reg.val[regnum] = MDK_REGISTER_FREE;

	return 0;
}

int rpn_token(mdk_context_t *ctx, char *rpn)
{
	char *p;
	int i;

	if (ctx == NULL || rpn == NULL)
		return EINVAL;

	for (i = 0, p = rpn; *p; p++) {
		if (*p && *p != ' ' && *p != '\t') {
			ctx->tkn[i].token = p;
			for (; *p && *p != ' ' && *p != '\t'; p++)
				;
			*p = '\0';

			/* predicate a data type */
			switch (*ctx->tkn[i].token) {
			case '+': ctx->tkn[i].type = MDK_TOKEN_ADD;   break;
			case '-': ctx->tkn[i].type = MDK_TOKEN_SUB;   break;
			case '*': ctx->tkn[i].type = MDK_TOKEN_MUL;   break;
			case '/': ctx->tkn[i].type = MDK_TOKEN_DIV;   break;
			default:  ctx->tkn[i].type = MDK_TOKEN_VALUE; break;
			}

			i++;
		}
	}

	return 0;
}

int generate_code(mdk_context_t *ctx)
{
	int i, r1, r2;
	char *inst;

	if (ctx == NULL)
		return EINVAL;

	for (i = 0; ctx->tkn[i].token; i++) {
		if (ctx->tkn[i].type != MDK_TOKEN_VALUE) {
			switch (ctx->tkn[i].type) {
			case MDK_TOKEN_ADD: inst = "ADD"; break;
			case MDK_TOKEN_SUB: inst = "SUB"; break;
			case MDK_TOKEN_MUL: inst = "MUL"; break;
			case MDK_TOKEN_DIV: inst = "DIV"; break;
			default:
				/* error */
				break;
			}

			r1 = capt_register(ctx);
			r2 = capt_register(ctx);

			printf("POP	QB%d\n", r1);
			printf("POP	QB%d\n", r2);
			printf("%s	QB%d,QB%d\n", inst, r1, r2);
			printf("PUSH	QB%d\n", r1);

			free_register(ctx, r1);
			free_register(ctx, r2);
		} else
			printf("PUSH	%s\n", ctx->tkn[i].token);

		printf(";\n");
	}
	r1 = capt_register(ctx);
	printf("POP	QB%d\n", r1);
	free_register(ctx, r1);

	return 0;
}

int rpn(char *rpn)
{
	mdk_context_t ctx;
	int r;

	if (rpn == NULL)
		return EINVAL;

	memset(&ctx, 0, sizeof(ctx));
	r = rpn_token(&ctx, rpn);
	if (r) {
		_printf("fail: rpn_token()\n");
	}

	r = generate_code(&ctx);
	if (r) {
		_printf("fail: generate_code()\n");
	}

	return 0;
}

int main(int argc, char *argv[])
{
	char buf[BUFSIZ], *p;

	_printf("Ctrl-D to quit. > ");	
	while (fgets(buf, BUFSIZ, stdin)) {
		p = strchr(buf, '\n');
		if (p)
			*p = '\0';
		rpn(buf);
		_printf("Ctrl-D to quit. > ");	
	}
	_printf("\n");

	exit(EXIT_SUCCESS);
}
