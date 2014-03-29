void G_SayCensor_Init(void);
char* G_SayCensor(char *msg,int slot,char **isvulgarism);
char* G_NickCensor(char *msg,int slot,char **isvulgarism);
char *SendDataToAdmins(char *message,int slot);
void StripLast(const char *input, char *output,int size);