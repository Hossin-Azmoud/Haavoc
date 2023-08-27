#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#define SZ       256
#define PATH_MAX 4096

typedef enum TokenType {
	STR_LIT,
	INT,
	ID,
	SYM
} TokenType;

typedef struct token {
	TokenType Type;
	char      *value;
} token;

typedef struct lexer {
	FILE    *file_pointer;
	char    *file_path;
} lexer;

char *get_type_name(TokenType t)
{
	switch(t)
	{
		case STR_LIT: { return "STR_LIT"; } break;
		case INT:     { return "INT"; } break;
		case ID:      { return "ID"; } break;
		case SYM:     { return "SYM"; } break;
		default:
			return ("NONE");
	}

	return ("NONE");
}

char *strdup(char *s)
{
	size_t it = 0;
	char *ptr;

	if (s == NULL)
		return (NULL);

	ptr = malloc(strlen(s) + 1);

	while (*(s + it))
	{
		*(ptr + it) = *(s + it);
		it++;
	}

	*(ptr + it)	 = 0;
	return (ptr);
}

lexer *new_lexer(char *path)
{
	lexer *lex = malloc(sizeof(lexer));

	if (path == NULL)
	{
		free(lex);
		fprintf(stderr, "Could not open the file! because u provided null path..\n");
		return (NULL);
	}

	if (lex == NULL)
		return (NULL);
	
	lex->file_path    = strdup(path);
	lex->file_pointer = fopen(lex->file_path, "r");

	if (lex->file_pointer == NULL)
	{
		fprintf(stderr, "Could not read the file..\n");
		free(lex);
		return (NULL);
	}

	return (lex);
}


token *next(lexer *lex)
{
	char c;
	short it = 0;
	token *tok   = malloc(sizeof(token));
	char  *value = malloc(SZ); *value = '\0';

	while ( ( c = fgetc(lex->file_pointer)) != EOF )
	{
		if(!isspace(c)) {
			if (isalpha(c))
			{
				value[it++] = c;
				tok->Type = ID;
				while ((c = fgetc(lex->file_pointer)) != EOF && isalnum(c))
				{
					value[it++] = c;
				}

				break;
			}

			if (isdigit(c))
			{
				value[it++] = c;
				tok->Type = INT;
				while ((c = fgetc(lex->file_pointer)) != EOF && isdigit(c))
				{
					value[it++] = c;
				}

				break;
			}
			
			if (ispunct(c))
			{
				if (c == '\"')
				{
					tok->Type = STR_LIT;
					while ((c = fgetc(lex->file_pointer)) != EOF && c != '\"')
					{
						value[it++] = c;
					}
				} else {
					tok->Type   = SYM;
					value[it++] = c;
				}

				break;
			}

			continue;
		}

		if(!(*value))
			continue;

		break;
	}

	if (c == EOF)
	{
		free(value);
		free(tok);
		return (NULL);
	}

	value[it]    = '\0';
	tok->value = value;

	return tok;
}

void free_lexer(lexer *lex)
{
	if (lex == NULL)
		return;

	fclose(lex->file_pointer);
	free(lex->file_path);
	free(lex);
}

int main()
{
	lexer *lx = new_lexer("./src.txt");
	token *tok = NULL;

	while ((tok = next(lx)))
	{
		printf("Token: %s\n", tok->value);
		printf("Type: %s\n", get_type_name(tok->Type));
		printf("\n");
		free(tok->value);
		free(tok);
	}

	free_lexer(lx);
	return (0);
}
