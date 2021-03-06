/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 15;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 20;       /* vert inner gap between windows */
static const unsigned int gappoh    = 15;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static const unsigned int swallowfloating = 1;
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 30;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const int vertpad            = 10;       /* vertical padding of bar */
static const int sidepad            = 10;       /* horizontal padding of bar */
static const char *fonts[]          = { "Iosevka:size=12",
                                        "FontAwesome:size=14",
                                        "Noto Emoji:size=14",};
static const char dmenufont[]       = "Iosevka:size=12";
static const char nord0[]       = "#2e3440";
static const char nord1[]       = "#3b4252";
static const char nord2[]       = "#434c5e";
static const char nord3[]       = "#4c566a";
static const char nord4[]       = "#d8dee9";
static const char nord5[]       = "#e5e9f0";
static const char nord6[]       = "#eceff4";
static const char nord7[]        = "#8fbcbb";
static const char nord8[]        = "#88c0d0";
static const char nord9[]        = "#81a1c1";
static const char nord10[]        = "#5e81ac";
static const char nord11[]        = "#bf616a";
static const char nord12[]        = "#d08770";
static const char nord13[]        = "#ebcb8b";
static const char nord14[]        = "#a3be8c";
static const char nord15[]        = "#b48ead";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { nord4, nord0, nord0 },
	[SchemeSel]  = { nord6, nord10,  nord1  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const char *tagsalt[] = { "", "", "", "", "", "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title        tags mask      isfloating     isterminal      noswallow      monitor */
	{ "Gimp",       NULL,   NULL,       0,              1,            0,               0,           -1 },
  { "TelegramDesktop", NULL, NULL,    1 << 2,         0,            0,               0,           -1 },
  { "discord",    NULL,   NULL,       1 << 2,         0,            0,               0,           -1 },
  { "Alacritty",  NULL,   NULL,       1 << 3,         0,            1,               0,           -1 },
  { "imv",        NULL,   NULL,       1 << 4,         0,            0,               0,           -1 },
  { "Virtualbox", NULL,   NULL,       1 << 5,         1,            0,               0,           -1 },
  { "ranger",     NULL,   NULL,       1 << 1,         0,            1,               0,           -1 },
  { "Pcmanfm",     NULL,   NULL,      1 << 1,         0,            1,               0,           -1 },
  { "trayer",     NULL,   NULL,       ~0,             0,            0,               0,           -1 },
  { "slstatus",   NULL,   NULL,       ~0,             0,            0,               0,           -1 },
  { "VirtualBox Manager", NULL, NULL, 1 << 5,         0,            0,               0,           -1 },
  { "Steam",      NULL,   NULL,       1 << 6,         1,            0,               0,           -1 },
  { "Spotify",    NULL,   NULL,       1 << 8,         1,            0,               0,           -1 },
	{ "Firefox",    NULL,   NULL,       1 << 0,         0,            1,               0,           -1 },
	{ "qutebrowser",  NULL, NULL,       1 << 0,         0,            1,               0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile }, /* first entry is default */
	{ "[\\]",     dwindle },
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *lock[] = { "slock", NULL };
static const char *voldown[] = { "pactl", "set-sink-volume", "alsa_output.pci-0000_0b_00.3.analog-stereo", "-5%", NULL };
static const char *volup[] = { "pactl", "set-sink-volume", "alsa_output.pci-0000_0b_00.3.analog-stereo", "+5%",
 NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *layoutmenu_cmd = "layoutmenu.sh";
static const char *flamegui[] = { "flameshot", "gui", NULL};
static const char *flamefull[] = { "flameshot", "full", "-c", NULL };
static const char *browser[] = { "firefox", NULL };
static const char *files[] = { "alacritty", "-e", "ranger", NULL };
static const char *trayer[] = { "trayer", "--expand", "true", "--widthtype", "request", "--transparent", "true", "--alpha", "255", "--edge", "top", "--align", "right" };
static const char *killteams[] = { "killall", "teams", NULL };

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY|ShiftMask,             XK_j,      rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      rotatestack,    {.i = -1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
  { MODKEY|ShiftMask,             XK_s,      spawn,          {.v = flamegui } },
  { MODKEY|ShiftMask,             XK_z,      spawn,          {.v = trayer } },
  { MODKEY,                       XK_p,      spawn,          {.v = flamefull } },
  { MODKEY,                       XK_x,      spawn,          {.v = files } },
  { MODKEY,                       XK_e,      spawn,          {.v = browser} },
  { MODKEY|ControlMask,           XK_w,      spawn,          {.v = killteams } },
	{ MODKEY|ControlMask,		XK_comma,  cyclelayout,    {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
  { MODKEY,              XK_bracketleft,     spawn,     {.v = voldown } },
  { MODKEY,              XK_bracketright,     spawn,     {.v = volup } },
  { MODKEY|ControlMask,              XK_bracketleft,     incnmaster,     {.i = -1 } },
  { MODKEY|ControlMask,              XK_bracketright,     incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY|ShiftMask,                       XK_Return, zoom,           {0} },
	{ MODKEY|ControlMask,              XK_u,      incrgaps,       {.i = +1 } },
	{ MODKEY|ControlMask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY|ControlMask,              XK_i,      incrigaps,      {.i = +1 } },
	{ MODKEY|ControlMask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	{ MODKEY|ControlMask,              XK_o,      incrogaps,      {.i = +1 } },
	{ MODKEY|ControlMask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	{ MODKEY|ControlMask,              XK_6,      incrihgaps,     {.i = +1 } },
	{ MODKEY|ControlMask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	{ MODKEY|ControlMask,              XK_7,      incrivgaps,     {.i = +1 } },
	{ MODKEY|ControlMask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	{ MODKEY|ControlMask,              XK_8,      incrohgaps,     {.i = +1 } },
	{ MODKEY|ControlMask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	{ MODKEY|ControlMask,              XK_9,      incrovgaps,     {.i = +1 } },
	{ MODKEY|ControlMask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY|ControlMask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|ControlMask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  rotatestack,      {.i = +1} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_Down,   moveresize,     {.v = "0x 25y 0w 0h" } },
	{ MODKEY,                       XK_Up,     moveresize,     {.v = "0x -25y 0w 0h" } },
	{ MODKEY,                       XK_Right,  moveresize,     {.v = "25x 0y 0w 0h" } },
	{ MODKEY,                       XK_Left,   moveresize,     {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = "0x 0y 0w 25h" } },
	{ MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = "0x 0y 0w -25h" } },
	{ MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = "0x 0y 25w 0h" } },
	{ MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = "0x 0y -25w 0h" } },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_n,      togglealttag,   {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_c,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_e,      spawn,           {.v = lock } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        layoutmenu,     {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTabBar,            0,              Button1,        focuswin,       {0} },
};
