//#include "../declarations.h"
//#include "../functions.c"
#include "../pinc.h"
cvar_t* sv_privateAdmins;
cvar_t* sv_adminPassword;
cvar_t* sv_privateClients;
cvar_t* sv_privatePassword;
PCL int OnInit(){	// Funciton called on server initiation
	//	Load function pointers
	//	Function pointers loaded, add your plugin's content here
	Plugin_PrintWarning("test\n");
    return 0;
}
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

PCL void OnMessageSent(char *message, int slot,qboolean *show){
	char text[150];
	Q_strncpyz( text, message, sizeof( text ) );
	char* textptr = text;
	char response[256];
	if(textptr[0] == 0x15) textptr++;
	
	if(textptr[0] == '!' || textptr[0] == '@')
	{
		textptr++;
		*show = qfalse;
		snprintf(response,sizeof(response)," ^4B3 Cmd Send ^3%s^7",message);
		Plugin_ChatPrintf(slot,response);
	}
}

PCL void OnClientConnecting(client_t **newcl,client_t **cl,char *password,qboolean *active)
{
    int a,b,c,count,admins,vip;
    count = Plugin_Cvar_VariableIntegerValue("sv_privateClients") + Plugin_Cvar_VariableIntegerValue("sv_privateAdmins");
	admins = 0;
	vip	   = 0;
	for (a = 0 ; a < Plugin_Cvar_VariableIntegerValue("sv_maxclients") ; a++ )
	{
		if ( clientbase[a].state >= CS_CONNECTED ) {
			if (clientbase[a].netchan.remoteAddress.type != NA_BOT) {
				if(a < Plugin_Cvar_VariableIntegerValue("sv_privateAdmins"))
					admins++;
				else if(a >= Plugin_Cvar_VariableIntegerValue("sv_privateAdmins") && a < count)
					vip++;
			}
		}
	}
    if ( !strcmp( password, Plugin_Cvar_VariableString("sv_adminPassword") ) ) {
        for ( b = 0; b < Plugin_Cvar_VariableIntegerValue("sv_privateAdmins") ; b++) {
            *cl = &clientbase[b];
            if (clientbase[b].state == CS_FREE) {
                *active = qtrue;
                *newcl = *cl;
                break;
            }
        }
    }
    else if ( !strcmp( password, Plugin_Cvar_VariableString("sv_privatePassword") ) || (!strcmp( password, Plugin_Cvar_VariableString("sv_adminPassword") ) && admins == Plugin_Cvar_VariableIntegerValue("sv_privateAdmins")) ) {
        for ( c = Plugin_Cvar_VariableIntegerValue("sv_privateAdmins"); c < count; c++) {
            *cl = &clientbase[c];
            if (clientbase[c].state == CS_FREE) {
                *active = qtrue;
                *newcl = *cl;
                break;
            }
        }
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
    strncpy(info->fullName,"4GF B3 Admin system",sizeof(info->fullName));
}