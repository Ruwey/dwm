/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 18;       /* snap pixel */
static const unsigned int gappih    = 18;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 18;       /* vert inner gap between windows */
static const unsigned int gappoh    = 18;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 18;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int usealtbar          = 1;        /* 1 means use non-dwm status bar */
static const char *altbarclass      = "Polybar"; /* Alternate bar class name */
static const char *alttrayname      = "tray";    /* Polybar tray instance name */
static const char *altbarcmd        = "$HOME/.config/polybar/bar"; /* Alternate bar launch command */
static const char *fonts[]          = { "Cozette" };
static const char dmenufont[]       = "Cozette";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};
static const unsigned int baralpha = OPAQUE;
static const unsigned int borderalpha = OPAQUE;
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/*  Autostart */
static const char *const autostart[] = {
	"sh", "-c", "$HOME/.config/autostart.sh", NULL, /* Universal Stuff */
	NULL /* terminate */
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"termite", "--name", "sppulse", "-e", "pulsemixer", NULL };
const char *spcmd2[] = {"termite", "--name", "spcalc", "-e", "qalc", NULL };
const char *spcmd3[] = {"termite", "--name", "spterm", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"sppulse",      spcmd1},
	{"spcalc",       spcmd2},
	{"spterm",       spcmd3},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
	{ NULL,       "sppulse",  NULL,       SPTAG(0),     1,           -1 },
	{ NULL,       "spcalc",   NULL,       SPTAG(1),     1,           -1 },
	{ NULL,       "spterm",   NULL,       SPTAG(2),     1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	/* { MOD, XK_Tab,   ACTION##stack, {.i = PREVSEL } }, 
	   { MOD, XK_q,     ACTION##stack, {.i = 0 } }, \
	   { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \
	   { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \
	   { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* Xresources preferences to load at startup (name, type, address) */
ResourcePref resources[] = {
		{ "colorbg",	STRING,  &normbgcolor },
		{ "colorbg",	STRING,  &normbordercolor },
		{ "colorfg",	STRING,  &normfgcolor },
		{ "color6",	STRING,  &selbgcolor },
		{ "color14",	STRING,  &selbordercolor },
		{ "colorfg",	STRING,  &selfgcolor },
		/* { "borderpx",          	INTEGER, &borderpx },
		{ "snap",          	INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	FLOAT,   &mfact }, */
};

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, "-h", "21", NULL };
static const char *termcmd[]  = { "termite", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_e,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_e,      spawn,          SHCMD("pkill -x xinit") },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	{ MODKEY|ShiftMask,             XK_o,      incnmaster,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_o,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_space,  zoom,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_i,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_i,      togglefloating, {0} },
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },
	/* { MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[2]} }, */
	{ MODKEY|ShiftMask,             XK_f,      fullscreen,      {0} },
	{ MODKEY,                       XK_s,      togglesticky,   {0} },
	{ MODKEY,                    XK_backslash, view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,          XK_backslash, tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	TAGKEYS(                        XK_0,                      9)
	/* Scratchpad Windows */
	{ MODKEY|ShiftMask,             XK_m,      togglescratch,  {.ui = 0} },
	{ MODKEY,			XK_c,	   togglescratch,  {.ui = 1} },
	{ MODKEY|ControlMask,           XK_Return, togglescratch,  {.ui = 2} },
	/*  Gaps */
	{ MODKEY,                       XK_p,      togglegaps,     {0} },
	{ MODKEY|ShiftMask,             XK_p,      incrgaps,       {.i = +3 } },
	{ MODKEY|ControlMask,           XK_p,      incrgaps,       {.i = -3 } },
	{ MODKEY|ControlMask|ShiftMask, XK_p,      defaultgaps,    {0} },
	/* Launchers */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_Tab,    spawn,          SHCMD("winFlash") },
	/* Power Keys */
	{ MODKEY,                       XK_BackSpace, spawn,       SHCMD("powerMenu") },
	{ MODKEY|ShiftMask,             XK_BackSpace, spawn,       SHCMD("blurlock") },
	{ MODKEY|ControlMask,           XK_BackSpace, spawn,       SHCMD("blurlock -s") },
	{ MODKEY|ControlMask|ShiftMask, XK_BackSpace, spawn,       SHCMD("blurlock -h") },
	/* Media Keys */
	{ 0,         XF86XK_AudioRaiseVolume,  	spawn,     SHCMD("chVol +5%") },
	{ 0,         XF86XK_AudioLowerVolume,   spawn,     SHCMD("chVol -5%") },
	{ ShiftMask, XF86XK_AudioRaiseVolume,  	spawn,     SHCMD("chVol +2%") },
	{ ShiftMask, XF86XK_AudioLowerVolume,   spawn,     SHCMD("chVol -2%") },
	{ 0,         XF86XK_AudioMute,          spawn,     SHCMD("chVol mute") },
	{ 0,         XF86XK_AudioPrev,          spawn,     SHCMD("playerctl -p $(cat $HOME/.local/share/mediaPlayer) previous") },
	{ 0,         XF86XK_AudioPlay,          spawn,     SHCMD("playerctl -p $(cat $HOME/.local/share/mediaPlayer) play-pause") },
	{ 0,         XF86XK_AudioNext,          spawn,     SHCMD("playerctl -p $(cat $HOME/.local/share/mediaPlayer) next") },
	{ MODKEY,    XK_m,                      spawn,     SHCMD("chMp") },
	{ 0,         XF86XK_MonBrightnessUp,    spawn,     SHCMD("chBl +10") },
	{ 0,         XF86XK_MonBrightnessDown,  spawn,     SHCMD("chBl -10") },
	{ ShiftMask, XF86XK_MonBrightnessUp,    spawn,     SHCMD("chBl +5") },
	{ ShiftMask, XF86XK_MonBrightnessDown,  spawn,     SHCMD("chBl -5") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(  view,                1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggleview,          1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tag,                 1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggletag,           1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tagmon,              1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  focusmon,            1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  focusstack,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  zoom,                1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  incnmaster,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  killclient,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  togglefloating,      1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  setmfact,            1,      {ARG_TYPE_FLOAT}  ),
  IPCCOMMAND(  setlayoutsafe,       1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  quit,                1,      {ARG_TYPE_NONE}   )
};

