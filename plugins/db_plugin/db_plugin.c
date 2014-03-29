//#include "../declarations.h"
//#include "../functions.c"
#include <mysql/mysql.h>
//#include <my_global.h>
#include <sys/types.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../pinc.h"
#include "db_plugin.h"
PCL int OnInit(){	// Funciton called on server initiation
	//Plugin_ScrAddMethod("testsend",send);
	Plugin_ScrAddMethod("senddata",Scr_Send);
	connect();
	return 0;
}
void Scr_Send(scr_entref_t arg){
	int entityNum;
	 if(HIWORD(arg))
		Plugin_Scr_ObjectError("Not an entity");   
	entityNum = LOWORD(arg);
	char *type = Plugin_Scr_GetString(0);
    char *ident = Plugin_Scr_GetString(1);
    char *data = Plugin_Scr_GetString(2);
	if(!data)
		data = "";
	char *receive = "this is bad value";
	SendData( type, ident, data, Plugin_GetPlayerGUID(entityNum), &receive, entityNum);
	Plugin_Scr_AddString(receive);
}
void SendData(char *type,char *ident,char *data,const char *guid,char **receive,int slot){
	if(strcmp( guid, "BOT-Client") == 0)
		return;
	regex_t regex;
	regex_t regex2;
	char *pattern = "^([a-f0-9]{32,32})$";
	int reti;
	char *pattern_ident = "^([0-9]+)$";
	int retu;
    reti = regcomp(&regex, pattern, REG_EXTENDED);	
	reti = regexec(&regex, guid, 0, NULL, 0);
	retu = regcomp(&regex2, pattern_ident, REG_EXTENDED);	
	retu = regexec(&regex2, ident, 0, NULL, 0);
	if( reti == REG_NOMATCH )
	{
		Plugin_PrintError("Client have spoofed guid %s\n",guid);
		regfree(&regex);
		return;
	}
	if( retu == REG_NOMATCH )
	{
		Plugin_PrintError("Client have spoofed ident %s\n",ident);
		regfree(&regex2);
		return;
	}
	regfree(&regex);
	regfree(&regex2);
	char select[256];
    snprintf(select,sizeof(select),"SELECT id FROM clients WHERE guid='%s'",guid);
    int net_port = Plugin_Cvar_GetInteger("net_port");
    if(net_port == 33333)
    {
        char * xpdb = "secure data";
        char * perkdb = "secure data";
    }
    else
    {
        char * xpdb = "secure data";
        char * perkdb = "secure data";
    }
    char *cl_id;
	comm_Db(select,"select_id",&cl_id);
    char rankxp[256],getperks[256],updatexp[256],insertperk[256],insertxp[256], newip[256],updateData[256];
    Plugin_Free(select);
    
    if(strcmp( type, "get_perks") == 0)
    {
        snprintf(getperks,sizeof(getperks),"secure data");
        char *perks;
		comm_Db(getperks,"select",&perks);
        *receive = perks;
        return;
    }else if(strcmp( type, "get_xp") == 0)
    {
        const char *ip = Plugin_GetPlayerIp(slot);
        StripLast(ip,newip,6);
        char * getxp; 
        char * dataUpdate;
        snprintf(rankxp,sizeof(rankxp),"secure data");
        snprintf(insertxp,sizeof(insertxp),"secure data");
        snprintf(updateData,sizeof(updateData),"secure data");
        comm_Db(rankxp,"select_id",&getxp);    
		if(strcmp(getxp,"null") == 0)
        {
            char *addxp;
			comm_Db(insertxp,"insert_xp",&addxp);
            comm_Db(updateData,"update",&dataUpdate);
            *receive = addxp;
            
            return;
        }
        comm_Db(updateData,"update",&dataUpdate);
		*receive = getxp;
        return;
    }else if(strcmp( type, "update_xp") == 0)
    {
        snprintf(updatexp,sizeof(updatexp),"secure data");
        char * update_xp; 
		comm_Db(updatexp,"update",&update_xp);
		*receive = update_xp;
        return;
    }else if(strcmp( type, "insert_perk") == 0)
    {
        snprintf(insertperk,sizeof(insertperk),"secure data");
        char * insert_perk; 
		comm_Db(insertperk,"insert",&insert_perk);
		*receive = insert_perk;
        return;
    }else if(strcmp( type, "id") == 0)
    {
        *receive = cl_id;
        return;
    }else
	{
		*receive = "error";
		return;
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
    info->pluginVersion.minor = 0;	// Plugin version
    strncpy(info->fullName,"4GF Database Plugin",sizeof(info->fullName)); //Full plugin name
}
