#include <stdlib.h>
#include <string.h>
#include "../pinc.h"
#include "censor.h"
#include <sys/types.h>
#include <regex.h>
#include <stdio.h>

typedef struct badwordsList_s {
    struct	badwordsList_s *next;
    char	word[1024];
}badwordsList_t;

badwordsList_t *badwords;


/*
=============
Q_strncpyz
 
Safe strncpy that ensures a trailing zero
=============
*/
void Q_strncpyz( char *dest, const char *src, int destsize ) {

	if (!dest ) {
	    Plugin_Error( P_ERROR_DISABLE, "Q_strncpyz: NULL dest" );
	}
	if ( !src ) {
		Plugin_Error( P_ERROR_DISABLE, "Q_strncpyz: NULL src" );
	}
	if ( destsize < 1 ) {
		Plugin_Error(P_ERROR_DISABLE,"Q_strncpyz: destsize < 1" ); 
	}

	strncpy( dest, src, destsize-1 );
  dest[destsize-1] = 0;
}


int Q_stricmpn (const char *s1, const char *s2, int n) {
	int		c1, c2;

        if ( s1 == NULL ) {
           if ( s2 == NULL )
             return 0;
           else
             return -1;
        }
        else if ( s2==NULL )
          return 1;


	
	do {
		c1 = *s1++;
		c2 = *s2++;

		if (!n--) {
			return 0;		// strings are equal until end point
		}
		
		if (c1 != c2) {
			if (c1 >= 'a' && c1 <= 'z') {
				c1 -= ('a' - 'A');
			}
			if (c2 >= 'a' && c2 <= 'z') {
				c2 -= ('a' - 'A');
			}
			if (c1 != c2) {
				return c1 < c2 ? -1 : 1;
			}
		}
	} while (c1);
	
	return 0;		// strings are equal
}

int Q_stricmp (const char *s1, const char *s2) {
	return (s1 && s2) ? Q_stricmpn (s1, s2, 99999) : -1;
}





char* removeColors(char *output,char *string,size_t size)
{
	register int j=0;
	register int i;
	if(!output || !string) return NULL;
	if(size < strlen(string)){
	    Plugin_Error(P_ERROR_TERMINATE,"Censor plugin: removeColors: buffer overflow");
	    return NULL;
	}
	for(i=0;string[i]!=0;i++){
		if(string[i]=='^' && (string[i+1] >= '0' && string[i+1] <= '9')){
			i++;
		}
		else{
			output[j]=string[i];
			j++;
		}			
	}
	output[j]=0;
	return output;
}





char* CharConv(char *output, char *string,int size)
{
	char *ret = output;
	if(!output || !string) return NULL;
	if(size < strlen(string)){
	    Plugin_Error(P_ERROR_TERMINATE,"Censor plugin: CharConv: buffer overflow");
	    return NULL;
	}
	for( ; *string != 0; string++){
		switch(*string){
			case 'A':
			case '4':
			case '@':
				*output='a';
				break;
			case 'B':
				*output='b';
				break;
			case 'C':
			case '(':
				*output='c';
				break;
			case 'D':
			case ')':
				*output='d';
				break;
			case 'E':
			case '3':
				*output='e';
				break;
			case 'F':
				*output='f';
				break;
			case 'G':
				*output='g';
				break;
			case 'H':
				*output='h';
				break;
			case 'I':
			case '1':
			case '|':
			case '!':
				*output='i';
				break;
			case 'J':
				*output='j';
				break;
			case 'K':
				*output='k';
				break;
			case 'L':
				*output='l';
				break;
			case 'M':
				*output='m';
				break;
			case 'N':
				*output='n';
				break;
			case 'O':
			case '0':
				*output='o';
				break;
			case 'P':
				*output='p';
				break;
			case 'Q':
				*output='q';
				break;
			case 'R':
				*output='r';
				break;
			case 'S':
			case '5':
			case '$':
				*output='s';
				break;
			case 'T':
				*output='t';
				break;
			case 'U':
				*output='u';
				break;
			case 'V':
				*output='v';
				break;
			case 'W':
				*output='w';
				break;
			case 'X':
				*output='x';
				break;
			case 'Y':
				*output='y';
				break;
			case 'Z':
				*output='z';
				break;
			case '.':
			case ',':
			case '\\':
			case '/':
			case '-':
			case '_':
				continue;
			default:
				*output=*string;
				break;
		}
		output++;
	}
	*output=0;
	return ret;
}




void G_SayCensor_Init()
{
	fileHandle_t file;
	int read;
	badwordsList_t *this;
	char buff[1024];
	char line[1024];
	char* linept;
	register int i=0;

        Plugin_FS_SV_FOpenFileRead("badwords.txt",&file);
        if(!file){
            Plugin_Printf("Censor_Plugin: Can not open badwords.txt for reading\n");
            return;
        }
        for(i = 0; ;i++){
            read = Plugin_FS_ReadLine(buff,sizeof(buff),file);

            if(read == 0){
                Plugin_Printf("%i lines parsed from badwords.txt\n",i);
                Plugin_FS_FCloseFile(file);
                return;
            }
            if(read == -1){
                Plugin_Printf("Can not read from badwords.txt\n");
                Plugin_FS_FCloseFile(file);
                return;
            }

            Q_strncpyz(line,buff,sizeof(line));
            linept = line;


            this = Plugin_Malloc(sizeof(badwordsList_t));
            if(this){
                this->next = badwords;
                Q_strncpyz(this->word,linept,strlen(linept));
                //Plugin_Printf("Regex num %i loaded: %s\n",i,this->word);
            	badwords = this;
	    }
        }
	Plugin_Printf("Censor: init complete.\n");
}

char* censor_ignoreMultiple(char *output, char *string, size_t size)
{
	if(!output || !string) return NULL;
	if(size < strlen(string)){
	    Plugin_Error(P_ERROR_TERMINATE,"Censor plugin: censor_ignoreMultiple: buffer overflow");
	    return NULL;
	}

	char *ret = output;

	for(; *string; output++){
		*output = *string;
		do{
		    string++;
		}while(*output == *string && *string);
	}
	*output=0;
	return ret;
}


char* G_SayCensor(char *msg,int slot,char **isvulgarism)
{
	badwordsList_t *this;
	char* ret = msg;
	int vulg = 0;
	char serverIp[64] = "88.86.107.13";
	char *string;
	if(msg[0] == 0x15) msg++;
	for(this = badwords ; this ; this = this->next){
			regex_t regex;
			int reti;
			char msgbuf[100];
			const char * pattern = this->word;
	/* Compile regular expression */
			reti = regcomp(&regex, pattern, REG_EXTENDED);
			if( reti ){
				Plugin_PrintWarning("Could not compile regex %s\n", pattern); 
				regfree(&regex);
				continue;
			}

	/* Execute regular expression */
			//Plugin_PrintWarning("pattern %s message %s",pattern,msg);
			reti = regexec(&regex, msg, 0, NULL, 0);
			if( !reti ){
				string = strstr(msg,serverIp);
				if(!string)
					vulg++;
			}
			else if( reti == REG_NOMATCH ){
				  //Plugin_PrintWarning("No regex match for %s\n", pattern);
			}
			else{
				regerror(reti, &regex, msgbuf, sizeof(msgbuf));
				//Plugin_PrintWarning("Regex match failed: %s\n", msgbuf);
			}

		regfree(&regex);
	}
	if(vulg > 0)
	{
		*isvulgarism = "true";
		//Plugin_G_LogPrintf( "say;%s;%d;%s;%s\n", Plugin_GetPlayerGUID(slot), slot, Plugin_GetPlayerName(slot), message );
	}
	return ret;
}
char* G_NickCensor(char *msg,int slot,char **isvulgarism)
{
	char token2[1024];
	char token[1024];
	badwordsList_t *this;
	char* ret = msg;
	int vulg = 0;
	while(1){
		msg = Plugin_ParseGetToken(msg);
		if(msg==NULL)
			break;

		int size = Plugin_ParseTokenLength(msg);
		Q_strncpyz(token,msg,size+1);

		removeColors(token2,token,sizeof(token2));
		CharConv(token,token2,sizeof(token));//	'clear' token
		censor_ignoreMultiple(token2,token,sizeof(token2));

		for(this = badwords ; this ; this = this->next){
                regex_t regex;
                int reti;
                char msgbuf[100];
                const char * pattern = this->word;
        /* Compile regular expression */
                reti = regcomp(&regex, pattern, REG_EXTENDED);
                if( reti ){
                    //Com_PrintWarning("Could not compile regex %s\n", pattern); 
                    regfree(&regex);
                    continue;
                }

        /* Execute regular expression */
                reti = regexec(&regex, token, 0, NULL, 0);
                if( !reti ){
                    //Com_PrintWarning("Regex match, sensoring"); 
                    vulg++;
					memset(msg,'*',size);
                }
                else if( reti == REG_NOMATCH ){
                      //Com_PrintWarning("No regex match for %s\n", pattern);
                }
                else{
                    regerror(reti, &regex, msgbuf, sizeof(msgbuf));
                    //Com_PrintWarning(stderr, "Regex match failed: %s\n", msgbuf);
                }

            regfree(&regex);
		}
	}
	if(vulg > 0)
	{
		*isvulgarism = "true";
	}
	return ret;
}
char *SendDataToAdmins(char *message,int slot)
{
	int i;
	char dvar[128];
	char *token;
	char *token2;
	char msg[256];
	char buffer[26];
	snprintf(msg,sizeof(msg),"^1Player^7 %s say vulgarity: ^3%s^7",Plugin_GetPlayerName(slot),message);
	for(i = 0; i < Plugin_GetSlotCount();i++)
	{
		snprintf(dvar,sizeof(dvar),"acp_info_%i",i);
		if(Plugin_Cvar_VariableString(dvar) == NULL)
			continue;
		if(strcmp(Plugin_GetPlayerState(i),"active") == 0)
		{
			snprintf(buffer,sizeof(buffer),"%s",Plugin_Cvar_VariableString(dvar));
			token = strtok(buffer,":");
			token2 = strtok(NULL,":");
			if(token2 == NULL)
				continue;
			if(atoi(token2) > 40)
				Plugin_ChatPrintf(i,msg);
		}
	}
	return "";
}
