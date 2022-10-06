
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

/* To avoid security error on Visual Studio */
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

/*====================================================================*/
/*          ****** DO NOT MODIFY ANYTHING FROM THIS LINE ******       */
#define MAX_NR_TOKENS    32    /* Maximum length of tokens in a command */
#define MAX_TOKEN_LEN    64    /*k mum length of single token */
#define MAX_ASSEMBLY    256 /* Maximum length of assembly string */

typedef unsigned char bool;
#define true    1
#define false    0
/*          ****** DO NOT MODIFY ANYTHING UP TO THIS LINE ******      */
/*====================================================================*/


/***********************************************************************
 * translate()
 *
 * DESCRIPTION
 *   Translate assembly represented in @tokens[] into a MIPS instruction.
 *   This translate should support following 13 assembly commands
 *
 *    - add
 *    - addi
 *    - sub
 *    - and
 *    - andi
 *    - or
 *    - ori
 *    - nor
 *    - lw
 *    - sw
 *    - sll
 *    - srl
 *    - sra
 *    - beq
 *    - bne
 *
 * RETURN VALUE
 *   Return a 32-bit MIPS instruction
 *
 */
static int register_translate(char* var)
{
    if(var[0] == 'a')
    {
        
        if(var[1] == 't')
        {
            return 1;
        }
        else{
            return atoi(&var[1]) + 4;
        }
    }
    else if(var[0] == 'v')
    {
        return atoi(&var[1])+2;
    }
    else if(var[0] == 'z')
    {
        return 0;
    }
    else if(var[0] == 't')
    {
        int a = atoi(&var[1]);
        if(a<8)
        {
            return a+8;
        }
        else{
            return a+16;
        }
    }
    else if(var[0] == 's')
    {
        if(var[1] == 'p')
        {
            return 29;
        }
        return atoi(&var[1])+16;
    }
    else if(var[0] == 'k')
    {
        return atoi(&var[1])+25;
    }
    else if(var[0] == 'g')
    {
        return 28;
    }
    else if(var[0] == 'f')
    {
        return 30;
    }
    else if(var[0] == 'r')
    { /*ra*/
        return 31;
    }
    else if(var[0] == '-')
    {
        char *var_2 = strtok(var, "-");
        if(var[1] == '0'){
            
            return -strtol(var_2, NULL,16);
        }
        else if(var[1])
        {
            return -atoi(var_2);
        }
    }
    else if(var[0] == '0'){
        return strtol(var, NULL,16);
    }
    else
    {
        return atoi(var);
    }
    return 1;
}

static char* tenConvertTwo(int a, int len, int minus)
{
    char*two =(char*)malloc(len);
    if(minus == -1) /* 음수인 경우는 양수로 바꿔서 2진법으로 변환 뒤 +1 함*/
    {
        a *= -1;
    }
    for(int i =0 ;i < len; i++)
    {
        two[i] = '0';
    }
    int div = a;
    int digit = len;
    while(1) /* 2진법 변환 */
    {
        two[digit-1] = div%2 + '0';
        div = (div/2) ;
        digit-=1;
        if(div ==0)
        {
            break;
        }
    }
    if(minus == -1) /* 음수인 경우 */
    {
        for(int i = 0;i<len;i++) /*보수 처리*/
        {
            if(two[i] == '1')
            {
                two[i] = '0';
            }
            else{
                two[i] = '1';
            }
        }
        int digit = len;
        while(1) /* +1 처리 */
        {
            if(two[digit-1] == '0')
            {
                two[digit-1] = '1';
                
                break;
            }
            else{
                
                two[digit-1] = '0';
                
                digit-=1;
            }
        }
    }
    return two;
    
}
static char* R_type(int opcode, int rs, int rt, int rd, int shmt, int funct)
{
    char*two = (char*)malloc(32);
    char* a = opcode >= 0 ? tenConvertTwo(opcode,6,1) : tenConvertTwo(opcode,6,-1);
    char* b = rs >= 0 ? tenConvertTwo(rs,5, 1) : tenConvertTwo(rs,5, -1);
    char* c = rt >= 0 ? tenConvertTwo(rt,5, 1) : tenConvertTwo(rt,5, -1);
    char* d = rd >= 0 ? tenConvertTwo(rd,5, 1) : tenConvertTwo(rd,5, -1);
    char* e = shmt >= 0 ? tenConvertTwo(shmt,5, 1) : tenConvertTwo(shmt,5, -1);
    char* f = funct >= 0 ? tenConvertTwo(funct,6,1) : tenConvertTwo(funct,6,-1);
    
    strcat(two,a);
    strcat(two,b);
    strcat(two,c);
    strcat(two,d);
    strcat(two,e);
    strcat(two,f);
    
    return two;
}
static char* I_type(int opcode, int rs, int rt, int imm)
{
    char*two = (char*)malloc(32);
    char* a = opcode >= 0 ? tenConvertTwo(opcode,6,1) : tenConvertTwo(opcode,6,-1);
    char* b = rs >= 0 ? tenConvertTwo(rs,5,1) : tenConvertTwo(rs,5,-1);
    char* c = rt >= 0 ? tenConvertTwo(rt,5,1) : tenConvertTwo(rt,5,-1);
    char* d = imm >= 0 ? tenConvertTwo(imm,16,1) : tenConvertTwo(imm,16,-1);
    
    strcat(two,a);
    strcat(two,b);
    strcat(two,c);
    strcat(two,d);
    return two;
}
static unsigned int translate(int nr_tokens, char *tokens[])
{
	/* TODO:
	 * This is an example MIPS instruction. You should change it accordingly.
	 */
    
    char* a = "";
    int opcode;
    int rs;
    int rt;
    int rd;
    int shmt;
    int funct;
    if(strcmp(tokens[0],"add") == 0)
    {
        a  = R_type(0,register_translate(tokens[2]),register_translate(tokens[3]),register_translate(tokens[1]),0,(strtol("0x20", NULL,16)));
        return strtol(a, NULL, 2);
    }
    else if(strcmp(tokens[0],"sub") ==0)
    {
        a = R_type(0,register_translate(tokens[2]),register_translate(tokens[3]),register_translate(tokens[1]),0,(strtol("0x22", NULL,16)));
        return strtol(a, NULL, 2);
    }
    else if(strcmp(tokens[0],"and") ==0)
    {
        a = R_type(0,register_translate(tokens[2]),register_translate(tokens[3]),register_translate(tokens[1]),0,(strtol("0x24", NULL,16)));
        return strtol(a, NULL, 2);
    }
    else if(strcmp(tokens[0],"or") ==0)
    {
        a = R_type(0,register_translate(tokens[2]),register_translate(tokens[3]),register_translate(tokens[1]),0,(strtol("0x25", NULL,16)));
        return strtol(a, NULL, 2);
    }
    else if(strcmp(tokens[0],"nor") ==0)
    {
        a = R_type(0,register_translate(tokens[2]),register_translate(tokens[3]),register_translate(tokens[1]),0,(strtol("0x27", NULL,16)));
        return strtol(a, NULL, 2);
    }
    else if(strcmp(tokens[0],"ori") ==0 )
    {
        a = I_type(strtol("0x0d", NULL,16),register_translate(tokens[2]),register_translate(tokens[1]),register_translate(tokens[3]));
        return strtol(a, NULL, 2);
    }
    else if(strcmp(tokens[0],"addi") ==0 )
    {
        a = I_type(strtol("0x08", NULL,16),register_translate(tokens[2]),register_translate(tokens[1]),register_translate(tokens[3]));
        return strtol(a, NULL, 2);
    }
    else if(strcmp(tokens[0],"sll") ==0)
    {
        a = R_type(0,0,register_translate(tokens[2]),register_translate(tokens[1]),register_translate(tokens[3]),(strtol("0x00", NULL,16)));
        return strtol(a, NULL, 2);
    }
    else if(strcmp(tokens[0],"srl") ==0)
    {
        a = R_type(0,0,register_translate(tokens[2]),register_translate(tokens[1]),register_translate(tokens[3]),(strtol("0x02", NULL,16)));
        return strtol(a, NULL, 2);
    }
    else if(strcmp(tokens[0],"sra") ==0)
    {
        a = R_type(0,0,register_translate(tokens[2]),register_translate(tokens[1]),register_translate(tokens[3]),(strtol("0x03", NULL,16)));
        return strtol(a, NULL, 2);
    }
    else if(strcmp(tokens[0],"lw") ==0 )
    {
        a = I_type(strtol("0x23", NULL,16),register_translate(tokens[2]),register_translate(tokens[1]),register_translate(tokens[3]));
        return strtol(a, NULL, 2);
    }
    else if(strcmp(tokens[0],"sw") ==0 )
    {
        a = I_type(strtol("0x2b", NULL,16),register_translate(tokens[2]),register_translate(tokens[1]),register_translate(tokens[3]));
        return strtol(a, NULL, 2);
    }
    else if(strcmp(tokens[0],"beq") ==0 )
    {
        a = I_type(strtol("0x04", NULL,16),register_translate(tokens[2]),register_translate(tokens[1]),register_translate(tokens[3]));
        return strtol(a, NULL, 2);
    }
    else if(strcmp(tokens[0],"bne") ==0 )
    {
        a = I_type(strtol("0x05", NULL,16),register_translate(tokens[2]),register_translate(tokens[1]),register_translate(tokens[3]));
        return strtol(a, NULL, 2);
    }
    else if(strcmp(tokens[0],"andi") ==0 )
    {
        a = I_type(strtol("0x0c", NULL,16),register_translate(tokens[2]),register_translate(tokens[1]),register_translate(tokens[3]));
        return strtol(a, NULL, 2);
    }
    else{
        return 1;
    }
   
}



/***********************************************************************
 * parse_command()
 *
 * DESCRIPTION
 *   Parse @assembly, and put each assembly token into @tokens[] and the number
 *   of tokes into @nr_tokens. You may use this implemention or your own
 *   from PA0.
 *
 *   A assembly token is defined as a string without any whitespace (i.e., space
 *   and tab in this programming assignment). For exmaple,
 *     command = "  add t1   t2 s0 "
 *
 *   then, nr_tokens = 4, and tokens is
 *     tokens[0] = "add"
 *     tokens[1] = "t0"
 *     tokens[2] = "t1"
 *     tokens[3] = "s0"
 *
 *   You can assume that the characters in the input string are all lowercase
 *   for testing.
 *
 *
 * RETURN VALUE
 *   Return 0 after filling in @nr_tokens and @tokens[] properly
 *
 */
static int parse_command(char *assembly, int *nr_tokens, char *tokens[])
{
	char *curr = assembly;
	int token_started = false;
	*nr_tokens = 0;

	while (*curr != '\0') {  
		if (isspace(*curr)) {  
			*curr = '\0';
			token_started = false;
		} else {
			if (!token_started) {
				tokens[*nr_tokens] = curr;
				*nr_tokens += 1;
				token_started = true;
			}
		}
		curr++;
	}

	return 0;
}


/*====================================================================*/
/*          ****** DO NOT MODIFY ANYTHING BELOW THIS LINE ******      */

/***********************************************************************
 * The main function of this program.
 */
int main(int argc, char * const argv[])
{
	char assembly[MAX_ASSEMBLY] = { '\0' };
	FILE *input = stdin;

	if (argc > 1) {
		input = fopen(argv[1], "r");
		if (!input) {
			fprintf(stderr, "No input file %s\n", argv[0]);
			return EXIT_FAILURE;
		}
	}

	if (input == stdin) {
		printf("*********************************************************\n");
		printf("*          >> SCE212 MIPS translator  v0.10 <<          *\n");
		printf("*                                                       *\n");
		printf("*                                       .---.           *\n");
		printf("*                           .--------.  |___|           *\n");
		printf("*                           |.------.|  |=. |           *\n");
		printf("*                           || >>_  ||  |-- |           *\n");
		printf("*                           |'------'|  |   |           *\n");
		printf("*                           ')______('~~|___|           *\n");
		printf("*                                                       *\n");
		printf("*                                 Spring 2022           *\n");
		printf("*********************************************************\n\n");
		printf(">> ");
	}

	while (fgets(assembly, sizeof(assembly), input)) {
		char *tokens[MAX_NR_TOKENS] = { NULL };
		int nr_tokens = 0;
		unsigned int machine_code;

		for (size_t i = 0; i < strlen(assembly); i++) {
			assembly[i] = tolower(assembly[i]);
		}

		if (parse_command(assembly, &nr_tokens, tokens) < 0)
			continue;

		machine_code = translate(nr_tokens, tokens);

		fprintf(stderr, "0x%08x\n", machine_code);
        fprintf(stderr, "%s\n", tenConvertTwo(machine_code,32,0));

		if (input == stdin) printf(">> ");
	}

	if (input != stdin) fclose(input);

	return EXIT_SUCCESS;
}
