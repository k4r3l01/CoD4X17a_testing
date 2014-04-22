#include <time.h>
#include "../pinc.h"
#include <stdio.h>
#include <stdlib.h>
PCL int OnInit(){	// Funciton called on server initiation
	//	Load function pointers
	//	Function pointers loaded, add your plugin's content here
	return 0;
}

PCL void OnMessageSent(char *message, int slot,qboolean **show){
	FILE *write;
	char path[512]; 
	snprintf(path,sizeof(path),"%s/%i.txt",Plugin_Cvar_VariableString("fs_homepath"),Plugin_Cvar_VariableIntegerValue("net_port"));
	write = fopen( path, "at" );
	fprintf(write,"time\\%d\\name\\%s\\guid\\%s\\message\\%s\n",(int)time(NULL),Plugin_GetPlayerName(slot),Plugin_GetPlayerGUID(slot),message);
	fclose(write);
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
    strncpy(info->fullName,"4GF Chatlog plugin",sizeof(info->fullName));
}