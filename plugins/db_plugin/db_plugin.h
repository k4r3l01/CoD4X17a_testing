void Scr_Send();
void connect();
void init();
void SendData(char *type,char *ident,char *data,const char *guid,char **receive,int slot);
void comm_Db(char *mysqldata,char *type,char **response);
typedef unsigned int long DWORD;
typedef unsigned short WORD;
#ifndef LOWORD
#define LOWORD(a) ((WORD)(a))
#endif
#ifndef HIWORD
#define HIWORD(a) ((WORD)(((DWORD)(a) >> 16) & 0xFFFF))
#endif