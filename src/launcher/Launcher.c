// Emacs style mode select	 -*- C++ -*-
//-----------------------------------------------------------------------------
//
// $Id: Launcher.c 968 2011-09-29 04:35:25Z svkaiser $
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// $Author: svkaiser $
// $Revision: 968 $
// $Date: 2011-09-29 04:35:25 +0000 (Thu, 29 Sep 2011) $
//
// DESCRIPTION:
//
//-----------------------------------------------------------------------------
#ifdef RCSID
static const char rcsid[] = "$Id: Launcher.c 968 2011-09-29 04:35:25Z svkaiser $";
#endif

#include "Launcher.h"
#include "tk_lib.h"

HINSTANCE   hAppInst    = NULL;
HWND        hwndMain    = NULL;
HWND        hwndRes     = NULL;
HWND        hwndSkill   = NULL;
HWND        hwndWarp    = NULL;
HWND        hwndNetType = NULL;
HWND        hwndPlrName = NULL;
HWND        hwndIpAddr  = NULL;

char CommandLine[1024]="";

POINT Resolutions[]=
{
    {   320,    240     },
    {   640,    480     },
    {   768,    576     },
    {   800,    600     },
    {   1024,   768     },
    {   1280,   960     },
    {   1400,   1050    },
    {   1600,   1200    },

    {   854,    480     },
    {   1280,   720     },
    {   1366,   768     },
    {   1920,   1080    },

    {   320,    200     },
    {   1280,   800     },
    {   1440,   900     },
    {   1680,   1050    },
    {   1920,   1200    },
    {   0,      0       }
};

char* Skills[4] =
{
    "BE GENTLE!",
    "BRING IT ON!",
    "I OWN DOOM!",
    "WATCH ME DIE!"
};

char* NetTypes[2] =
{
    "Listen Server",
    "Client"
};

static bool bDevMode        = false;
static bool bFastMonsters   = false;
static bool bNoMosnters     = false;
static bool bNoMusic        = false;
static bool bNoSound        = false;
static bool bRespawnThings  = false;
static bool	bRespawnItems   = false;

static int iWarp            = 0;
static int iSkill           = 0;
static int iResolution      = 0;

static bool bWindowed       = false;

static char *sPlayerName    = NULL;
static char *sIPAddress     = NULL;
static char *sPort          = NULL;
static bool bNetgame        = false;
static bool bDeathMatch     = false;
static int	iNetType        = 0;


tDefTypes_t LauncherDefaults[] =
{
    {"devmode",         'i', false, &bDevMode,      NULL,           NULL},
    {"fast_monsters",   'i', false, &bFastMonsters, NULL,           NULL},
    {"no_mosnters",     'i', false, &bNoMosnters,   NULL,           NULL},
    {"no_music",        'i', false, &bNoMusic,      NULL,           NULL},
    {"no_sound",        'i', false, &bNoSound,      NULL,           NULL},
    {"respawn_things",  'i', false, &bRespawnThings,NULL,           NULL},
    {"respawn_items",   'i', false, &bRespawnItems, NULL,           NULL},
    
    {"level_warp",      'i', 0,     &iWarp,         NULL,           NULL},
    {"start_skill",		'i', 2,     &iSkill,        NULL,           NULL},
    {"video_resolution",'i', 3,     &iResolution,   NULL,           NULL},
    
    {"windowed",        'i', true,  &bWindowed,     NULL,           NULL},
    
    {"playername",      's', 0,     NULL,           "Player",       &sPlayerName},
    {"ip_address",      's', 0,     NULL,           "127.0.0.1",    &sIPAddress},
    {"port",            's', 0,     NULL,           "2342",         &sPort},
    {"netgame",         'i', false,	&bNetgame,      NULL,           NULL},
    {"deathmatch",      'i', false, &bDeathMatch,   NULL,           NULL},
    {"nettype",         'i', 0,     &iNetType,      NULL,           NULL},
    
    {NULL,              '\0',0,     NULL,           NULL,           NULL}
};

//**************************************************************
//**************************************************************
//	L_InitConfig
//	Associates the variables used for LauncherDefaults from the
//	config file
//**************************************************************
//**************************************************************

void L_InitConfig(tDefTypes_t *config, HWND hWnd) 
{
    FILE*			f;
    char			c[32];
    tDefTypes_t*	def;
    int				size;
    
    f = fopen(DefaultConfigFile, "rb");
    tk_ResetConfig(config);
    
    if(f)
    {
        fseek(f, 0, SEEK_SET);
        fseek(f, 0, SEEK_END);
        size = ftell(f);
        fseek(f, 0, SEEK_SET);
        
        if(!size)
        {
            L_Complain("tk_ConfigInit: Error reading cfg file! Filesize = %i\n", size);
            return;
        }
        
        parse = (byte*)malloc(sizeof(byte)*size);
        lastByte = size;
        
        fread(parse, size, 1, f);
        
        fclose(f);
        
        tkLine = 1;
        while(tkPos != lastByte) 
        {
            tk_getToken(); tk_toLwrToken();
            
            for(def = config; def->field; def++) 
            {
                strncpy(c, def->field, 32);
                strlwr(c);
                if(!strncmp(stringToken, c, 32)) 
                {
                    tk_ProcessDefs(def);
                    break;
                }
            }
        }
        
        tk_FreeParse();
    }
    
    CheckDlgButton(hWnd, IDC_DEVMODE,       bDevMode);
    CheckDlgButton(hWnd, IDC_FASTMONSTERS,  bFastMonsters);
    CheckDlgButton(hWnd, IDC_NOMONSTERS,    bNoMosnters);
    CheckDlgButton(hWnd, IDC_NOMUSIC,       bNoMusic);
    CheckDlgButton(hWnd, IDC_NOSOUND,       bNoSound);
    CheckDlgButton(hWnd, IDC_RESPAWN,       bRespawnThings);
    CheckDlgButton(hWnd, IDC_RESPAWNITEM,   bRespawnItems);
    
    CheckDlgButton(hWnd, IDC_NETGAME,       bNetgame);
    CheckDlgButton(hWnd, IDC_NETDM,         bDeathMatch);
    
    CheckDlgButton(hWnd, IDC_WINDOWED,      bWindowed);
}

//**************************************************************
//**************************************************************
//	L_InitResolution
//**************************************************************
//**************************************************************

void L_InitResolution(HWND hWnd)
{
    char	buff[20];
    POINT	*p;
    int		i = 0;
    
    hwndRes = GetDlgItem(hWnd, IDC_CORES);
    for (i = 0, p = Resolutions; p->x; p++, i++)
    {
        if(i <= 7)
            sprintf(buff, "%dx%d (4:3)", p->x, p->y);
        else if(i <= 11)
            sprintf(buff, "%dx%d (16:9)", p->x, p->y);
        else
            sprintf(buff, "%dx%d (16:10)", p->x, p->y);

        SendMessage(hwndRes, CB_ADDSTRING, 0, (LPARAM)buff);
        SendMessage(hwndRes, CB_SETITEMDATA, i, (LPARAM)i);
    }
    SendMessage(hwndRes, CB_SETCURSEL, iResolution, 0);
}

//**************************************************************
//**************************************************************
//	L_InitSkills
//**************************************************************
//**************************************************************

void L_InitSkills(HWND hWnd)
{
    int		i;
    
    hwndSkill = GetDlgItem(hWnd, IDC_COSKILL);
    for (i = 0; i < 4; i++)
    {
        SendMessage(hwndSkill, CB_ADDSTRING, 0, (LPARAM)Skills[i]);
        SendMessage(hwndSkill, CB_SETITEMDATA, i, (LPARAM)i);
    }
    SendMessage(hwndSkill, CB_SETCURSEL, iSkill, 0);
}

//**************************************************************
//**************************************************************
//	L_InitNetType
//**************************************************************
//**************************************************************

void L_InitNetType(HWND hWnd)
{
    int		i;
    
    hwndNetType = GetDlgItem(hWnd, IDC_CONET);
    for (i = 0; i < 2; i++)
    {
        SendMessage(hwndNetType, CB_ADDSTRING, 0, (LPARAM)NetTypes[i]);
        SendMessage(hwndNetType, CB_SETITEMDATA, i, (LPARAM)i);
    }
    SendMessage(hwndNetType, CB_SETCURSEL, iNetType, 0);
}

//**************************************************************
//**************************************************************
//	L_InitWarp
//**************************************************************
//**************************************************************

void L_InitWarp(HWND hWnd)
{
    int		i = 0;
    char	buff[2];
    
    hwndWarp = GetDlgItem(hWnd, IDC_COWARP);
    SendMessage(hwndWarp, CB_ADDSTRING, 0, (LPARAM)"None");
    SendMessage(hwndWarp, CB_SETITEMDATA, 0, (LPARAM)i);
    for (i = 1; i < 33; i++)
    {
        sprintf(buff, "%02d", i);
        SendMessage(hwndWarp, CB_ADDSTRING, 0, (LPARAM)buff);
        SendMessage(hwndWarp, CB_SETITEMDATA, i, (LPARAM)i);
    }
    SendMessage(hwndWarp, CB_SETCURSEL, iWarp, 0);
}

//**************************************************************
//**************************************************************
//	L_InitName
//**************************************************************
//**************************************************************

void L_InitName(HWND hWnd)
{
    SetDlgItemText(hWnd, IDC_PLYRNAME, sPlayerName);
}

//**************************************************************
//**************************************************************
//	L_CreateDlgSettings
//	Sets all booleans and variables for the config file
//**************************************************************
//**************************************************************

void L_CreateDlgSettings(HWND hWnd)
{
    bDevMode        =   IsDlgButtonChecked(hWnd, IDC_DEVMODE);
    bFastMonsters   =   IsDlgButtonChecked(hWnd, IDC_FASTMONSTERS);
    bNoMosnters     =   IsDlgButtonChecked(hWnd, IDC_NOMONSTERS);
    bNoMusic        =   IsDlgButtonChecked(hWnd, IDC_NOMUSIC);
    bNoSound        =   IsDlgButtonChecked(hWnd, IDC_NOSOUND);
    bRespawnThings  =   IsDlgButtonChecked(hWnd, IDC_RESPAWN);
    bRespawnItems   =   IsDlgButtonChecked(hWnd, IDC_RESPAWNITEM);
    
    bWindowed       =   IsDlgButtonChecked(hWnd, IDC_WINDOWED);
    
    iWarp           =   SendMessage(hwndWarp, CB_GETCURSEL, 0, 0);
    iSkill          =   SendMessage(hwndSkill, CB_GETCURSEL, 0, 0);
    iResolution     =   SendMessage(hwndRes, CB_GETCURSEL, 0, 0);
    
    bNetgame        =   IsDlgButtonChecked(hWnd, IDC_NETGAME);
    bDeathMatch     =   IsDlgButtonChecked(hWnd, IDC_NETDM);
    iNetType        =   SendMessage(hwndNetType, CB_GETCURSEL, 0, 0);
    
    GetDlgItemText(hWnd, IDC_PLYRNAME, sPlayerName, 1024);
    if(sPlayerName == "")
        sPlayerName = "Player";
}

//**************************************************************
//**************************************************************
//	L_SaveConfig
//**************************************************************
//**************************************************************

void L_SaveConfig(HWND hWnd)
{
    L_CreateDlgSettings(hWnd);
    tk_Open();
    tk_SaveConfig(LauncherDefaults);
    tk_Close();
}

//**************************************************************
//**************************************************************
//	L_CreateExecutableParam
//	Assembles the path executable and arguments to CommandLine
//**************************************************************
//**************************************************************

bool L_CreateExecutableParam(HWND hWnd)
{
    PROCESS_INFORMATION	pi;
    STARTUPINFO			si;
    char				buff[1024];
    int					data;
    POINT*				res;
    
    strcpy(CommandLine, "Doom64.exe ");
    
    if(IsDlgButtonChecked(hWnd, IDC_DEVMODE)==BST_CHECKED)      strcat(CommandLine,	" -devparm");
    if(IsDlgButtonChecked(hWnd, IDC_FASTMONSTERS)==BST_CHECKED) strcat(CommandLine, " -fast");
    if(IsDlgButtonChecked(hWnd, IDC_NOMONSTERS)==BST_CHECKED)   strcat(CommandLine,	" -nomonsters");
    if(IsDlgButtonChecked(hWnd, IDC_NOMUSIC)==BST_CHECKED)      strcat(CommandLine,	" -nomusic");
    if(IsDlgButtonChecked(hWnd, IDC_NOSOUND)==BST_CHECKED)      strcat(CommandLine,	" -nosound");
    if(IsDlgButtonChecked(hWnd, IDC_RESPAWN)==BST_CHECKED)      strcat(CommandLine,	" -respawn");
    if(IsDlgButtonChecked(hWnd, IDC_RESPAWNITEM)==BST_CHECKED)  strcat(CommandLine,	" -respawnitem");
    
    if(IsDlgButtonChecked(hWnd, IDC_NETGAME) == BST_CHECKED)
    {
        data = SendMessage(hwndNetType, CB_GETCURSEL, 0, 0);
        switch(data)
        {
        case 0:
            strcat(CommandLine,	" -server");
            GetDlgItemText(hWnd, IDC_IPADDR, buff, 1024);

            strcat(CommandLine,	" ");
            strcat(CommandLine,	buff);
            
            if((!strcmp(buff, "")) || (!strcmp(buff, " ")))
                break;

            strcat(CommandLine,	" -port");
            strcat(CommandLine,	" ");
            strcat(CommandLine,	buff);

            break;
        case 1:
            strcat(CommandLine,	" -connect");
            GetDlgItemText(hWnd, IDC_IPADDR, buff, 1024);
            
            if(!strstr(buff, ".") || strlen(buff) < 7)
                L_Complain("L_CreateExecutableParam: %s is not a valid IP Address", buff);
            
            strcat(CommandLine,	" ");
            strcat(CommandLine,	buff);
            break;
        }
    }
    
    strcat(CommandLine,	" -playername");
    strcat(CommandLine,	" ");
    GetDlgItemText(hWnd, IDC_PLYRNAME, buff, 1024);
    strcat(CommandLine,	buff);
    
    if(IsDlgButtonChecked(hWnd, IDC_NETDM) == BST_CHECKED)
        strcat(CommandLine,	" -deathmatch");
    
    if(IsDlgButtonChecked(hWnd, IDC_WINDOWED) == BST_CHECKED) 
        strcat(CommandLine,	" -window");
    else strcat(CommandLine," -fullscreen");
    
    data = SendMessage(hwndWarp, CB_GETCURSEL, 0, 0);
    if(data)
    {
        sprintf(buff, " -warp %i", SendMessage(hwndWarp, CB_GETITEMDATA, data, 0));
        strcat(CommandLine,	buff);
        
        data = SendMessage(hwndSkill, CB_GETCURSEL, 0, 0) + 1;
        sprintf(buff, " -skill %i", data);
        strcat(CommandLine,	buff);
    }
    
    data = SendMessage(hwndRes, CB_GETCURSEL, 0, 0);
    res = &Resolutions[data];
    sprintf(buff, " -width %i", res->x);
    strcat(CommandLine,	buff);
    sprintf(buff, " -height %i", res->y);
    strcat(CommandLine,	buff);
    
    GetDlgItemText(hWnd, IDC_PARAM, buff, 1024);
    strcat(CommandLine,	" ");
    strcat(CommandLine,	buff);
    
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb=sizeof(STARTUPINFO);
    if(CreateProcess(NULL, CommandLine, NULL, NULL, FALSE, 
        CREATE_DEFAULT_ERROR_MODE|DETACHED_PROCESS, NULL, NULL, &si, &pi))
    {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else
    {
        MessageBox(NULL, "Unable to launch the game. Make sure launcher is in the same directory as game executable", "Error", MB_OK);
        return false;
    }
    return true;
}

//**************************************************************
//**************************************************************
//	L_Complain
//**************************************************************
//**************************************************************

void L_Complain(char *fmt, ...)
{
    va_list	va;
    char	buff[1024];
    
    va_start(va, fmt);
    vsprintf(buff, fmt, va);
    va_end(va);
    MessageBox(NULL, buff, "Error", MB_OK);
    exit(1);
}


//**************************************************************
//**************************************************************
//	L_CheckParamOptions
//**************************************************************
//**************************************************************

void L_CheckParamOptions(HWND hWnd)
{
    char* port = "Port (default: 2342)";
    char* ipaddress = "IP Address";

    if(SendMessage(hwndNetType, CB_GETCURSEL, 0, 0) == 1)
    {
        EnableWindow(GetDlgItem(hWnd, IDC_NETDM), 0);
        EnableWindow(GetDlgItem(hWnd, IDC_DEVMODE), 0);
        EnableWindow(GetDlgItem(hWnd, IDC_FASTMONSTERS), 0);
        EnableWindow(GetDlgItem(hWnd, IDC_NOMONSTERS), 0);
        EnableWindow(GetDlgItem(hWnd, IDC_RESPAWN), 0);
        EnableWindow(GetDlgItem(hWnd, IDC_RESPAWNITEM), 0);
        EnableWindow(GetDlgItem(hWnd, IDC_COWARP), 0);
        EnableWindow(GetDlgItem(hWnd, IDC_COSKILL), 0);

        SendMessage(GetDlgItem(hWnd, IDC_STATIC_IP), WM_SETTEXT, 0, (LPARAM)ipaddress);

        SetDlgItemText(hWnd, IDC_IPADDR, sIPAddress);

        GetDlgItemText(hWnd, IDC_IPADDR, sIPAddress, 1024);
        if(sIPAddress == "")
            sIPAddress = "127.0.0.1";
    }
    else
    {
        EnableWindow(GetDlgItem(hWnd, IDC_NETDM), 1);
        EnableWindow(GetDlgItem(hWnd, IDC_DEVMODE), 1);
        EnableWindow(GetDlgItem(hWnd, IDC_FASTMONSTERS), 1);
        EnableWindow(GetDlgItem(hWnd, IDC_NOMONSTERS), 1);
        EnableWindow(GetDlgItem(hWnd, IDC_RESPAWN), 1);
        EnableWindow(GetDlgItem(hWnd, IDC_RESPAWNITEM), 1);
        EnableWindow(GetDlgItem(hWnd, IDC_COWARP), 1);
        EnableWindow(GetDlgItem(hWnd, IDC_COSKILL), 1);

        SendMessage(GetDlgItem(hWnd, IDC_STATIC_IP), WM_SETTEXT, 0, (LPARAM)port);

        SetDlgItemText(hWnd, IDC_IPADDR, sPort);

        GetDlgItemText(hWnd, IDC_IPADDR, sPort, 1024);
        if(sPort == "")
            sPort = "2342";
    }
}

//**************************************************************
//**************************************************************
//	L_CheckNetgameOptions
//**************************************************************
//**************************************************************

void L_CheckNetgameOptions(HWND hWnd)
{
    if(IsDlgButtonChecked(hWnd, IDC_NETGAME) == BST_CHECKED)
    {
        EnableWindow(GetDlgItem(hWnd, IDC_CONET), 1);
        EnableWindow(GetDlgItem(hWnd, IDC_PLYRNAME), 1);
        EnableWindow(GetDlgItem(hWnd, IDC_NETDM), 1);
        EnableWindow(GetDlgItem(hWnd, IDC_IPADDR), 1);

        L_CheckParamOptions(hWnd);
    }
    else
    {
        EnableWindow(GetDlgItem(hWnd, IDC_CONET), 0);
        EnableWindow(GetDlgItem(hWnd, IDC_PLYRNAME), 0);
        EnableWindow(GetDlgItem(hWnd, IDC_IPADDR), 0);
        EnableWindow(GetDlgItem(hWnd, IDC_NETDM), 0);

        EnableWindow(GetDlgItem(hWnd, IDC_DEVMODE), 1);
        EnableWindow(GetDlgItem(hWnd, IDC_FASTMONSTERS), 1);
        EnableWindow(GetDlgItem(hWnd, IDC_NOMONSTERS), 1);
        EnableWindow(GetDlgItem(hWnd, IDC_RESPAWN), 1);
        EnableWindow(GetDlgItem(hWnd, IDC_RESPAWNITEM), 1);
        EnableWindow(GetDlgItem(hWnd, IDC_COWARP), 1);
        EnableWindow(GetDlgItem(hWnd, IDC_COSKILL), 1);
    }
}

//**************************************************************
//**************************************************************
//	MainDlgProc
//**************************************************************
//**************************************************************

bool __stdcall MainDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
        L_InitConfig(LauncherDefaults, hWnd);
        L_InitResolution(hWnd);
        L_InitSkills(hWnd);
        L_InitWarp(hWnd);
        L_InitNetType(hWnd);
        L_InitName(hWnd);

        L_CheckParamOptions(hWnd);
        L_CheckNetgameOptions(hWnd);

        return TRUE;

    case WM_DESTROY:
        L_SaveConfig(hWnd);
        return TRUE;

    case WM_CLOSE:
        EndDialog(hWnd, FALSE);
        break;

    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case IDC_NETGAME:
            L_CheckNetgameOptions(hWnd);
            break;
            
        case IDC_CONET:
            L_CheckParamOptions(hWnd);
            break;
            
        case ID_BTNLAUNCH:
            if(L_CreateExecutableParam(hWnd))
                SendMessage(hWnd, WM_ENABLE, 0, 0);
            break;
        }
        break;
    }
    return FALSE;
}

//**************************************************************
//**************************************************************
//	WinMain
//**************************************************************
//**************************************************************

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    hAppInst = hInstance;
    InitCommonControls();
    
    DialogBox(hAppInst, MAKEINTRESOURCE(IDD_DLGMAIN), NULL, (DLGPROC)MainDlgProc);
    
    return 0;
}

