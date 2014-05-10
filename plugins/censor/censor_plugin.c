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
PCL void OnMessageSent(char* message, int slot, qboolean *show)
{
    char *isvulgarism = "false";
	G_SayCensor(message,slot,&isvulgarism);
	if(strcmp( isvulgarism, "true") == 0)
	{
        SendDataToAdmins(message,slot);
		*show = qfalse;
	}
}

PCL void OnClientUserinfoChanged(client_t* client)
{
	char *isvulgarism = "false";
	int slotNum = client - clientbase;
	char *name = client->name;
	G_NickCensor(name,slotNum,&isvulgarism);
	if(strcmp( isvulgarism, "true") == 0)
	{
		Q_strncpyz(client->shortname, name, sizeof(client->shortname));
		Info_SetValueForKey( client->userinfo, "name", client->shortname);
	}
}

PCL void OnInfoRequest(pluginInfo_t *info){	// Function used to obtain information about the plugin
    // Memory pointed by info is allocated by the server binary, just fill in the fields
    
    // =====  MANDATORY FIELDS  =====
    info->handlerVersion.major = PLUGIN_HANDLER_VERSION_MAJOR;
    info->handlerVersion.minor = PLUGIN_HANDLER_VERSION_MINOR;	// Requested handler version
    // Requested handler version
    
    // =====  OPTIONAL  FIELDS  =====
    info->pluginVersion.major = 2;
    info->pluginVersion.minor = 200;	// Plugin version
    strncpy(info->fullName,"IceOps message censoring plugin by TheKelm",sizeof(info->fullName));
}