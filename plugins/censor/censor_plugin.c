//#include "../declarations.h"
//#include "../functions.c"
#include "../pinc.h"
#include "censor.h"

PCL int OnInit(){	// Funciton called on server initiation
	//	Load function pointers
	//	Function pointers loaded, add your plugin's content here
	G_SayCensor_Init();
	
	return 0;
}
PCL void OnMessageSent(char *message, int slot,qboolean **show,qboolean **censored){
	const char *ip = Plugin_GetPlayerIp(slot);
    char newip[256];
    StripLast(ip,newip,6);
    char *isvulgarism = "false";
	G_SayCensor(message,slot,&isvulgarism);
	if(strcmp( isvulgarism, "true") == 0)
	{
		Plugin_ChatPrintf(slot,"ip test %s",newip);
        SendDataToAdmins(message,slot);
		*censored = qfalse;
        *show = qfalse;
	}
}

PCL void OnClientInfoChange(int slotNum)
{
	char *isvulgarism = "false";
	char *name;
	name = Plugin_GetPlayerName(slotNum);
	G_NickCensor(name,slotNum,&isvulgarism);
	if(strcmp( isvulgarism, "true") == 0)
		Plugin_SetPlayerName(slotNum, name);
}

PCL void OnInfoRequest(pluginInfo_t *info){	// Function used to obtain information about the plugin
    // Memory pointed by info is allocated by the server binary, just fill in the fields
    
    // =====  MANDATORY FIELDS  =====
    info->handlerVersion.major = PLUGIN_HANDLER_VERSION_MAJOR;
    info->handlerVersion.minor = PLUGIN_HANDLER_VERSION_MINOR;	// Requested handler version
    // Requested handler version
    
    // =====  OPTIONAL  FIELDS  =====
    info->pluginVersion.major = 1;
    info->pluginVersion.minor = 0;	// Plugin version
    strncpy(info->fullName,"IceOps message censoring plugin by TheKelm",sizeof(info->fullName));
}

void StripLast(const char *input, char *output,int size)
{
    int len = strlen(input);
    strcpy(output, input);
    output[len - size] = '\0';
}