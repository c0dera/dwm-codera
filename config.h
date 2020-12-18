/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 45;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 45;       /* vert inner gap between windows */
static const unsigned int gappoh    = 35;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 35;       /* vert outer gap between windows and screen edge */
static const unsigned int swallowfloating = 1;
static const int startontag         = 0;        /* 0 means no tag active on start */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const Bool viewontag         = True;     /* Switch view on tag switch */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 35;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const int vertpad            = 0;       /* vertical padding of bar */
static const int sidepad            = 0;       /* horizontal padding of bar */
/*  Display modes of the tab bar: never shown, always shown, shown only in  */
/*  monocle mode in the presence of several windows.                        */
/*  Modes after showtab_nmodes are disabled.                                */
enum showtab_modes { showtab_never, showtab_auto, showtab_nmodes, showtab_always};
static const int showtab			= showtab_auto;        /* Default tab bar show mode */
static const int toptab				= True;               /* False means bottom tab bar */

static const char *fonts[]          = { "Iosevka:size=12",
                                        "FontAwesome:size=16",
                                        "Noto Emoji:size=14",};
static const char red[]       = "#cc241d";
static const char green[]     = "#98971a";
static const char yellow[]    = "#d79921";
static const char blue[]      = "#458588";
static const char purple[]    = "#b16286";
static const char aqua[]      = "#689d6a";
static const char gray0[]      = "#a89984";
static const char gray1[]      = "#928374";
static const char bg0[]        = "#282828";
static const char bg1[]        = "#3c3836";
static const char bg2[]       = "#504945";
static const char bg3[]       = "#665c54";
static const char bg4[]       = "#7c6f64";
static const char fg0[]        = "#fbf1c7";
static const char fg1[]        = "#ebdbb2";
static const char fg2[]       = "#d5c4a1";
static const char fg3[]       = "#bdae93";
static const char fg4[]       = "#a89984";
static const char orange[]    = "#d65d0e";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { fg2, bg0, bg3 },
	[SchemeSel]  = { fg0, orange, orange  },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };
static const char *tagsalt[] = { "❶", "❷", "❸", "❹", "❺", "❻", "❼", "❽", "❾" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title        tags mask      isfloating     isterminal      noswallow      monitor */
	{ "Gimp",       NULL,   NULL,       0,              1,            0,               0,           -1 },
	{ "TelegramDesktop", NULL, NULL,    1 << 2,         0,            0,               0,           -1 },
	{ "discord",    NULL,   NULL,       1 << 2,         0,            0,               0,           -1 },
	{ "st-256color", NULL,  NULL,       1 << 3,         0,            1,               0,           -1 },
	{ "Virtualbox", NULL,   NULL,       1 << 5,         1,            0,               0,           -1 },
	{ "nnn",     NULL,   NULL,       1 << 1,         0,            1,               0,           -1 },
	{ "Pcmanfm",     NULL,   NULL,      1 << 1,         0,            1,               0,           -1 },
	{ "trayer",     NULL,   NULL,       ~0,             0,            0,               0,           -1 },
	{ "slstatus",   NULL,   NULL,       ~0,             0,            0,               0,           -1 },
	{ "VirtualBox Manager", NULL, NULL, 1 << 5,         0,            0,               0,           -1 },
	{ "Steam",      NULL,   NULL,       1 << 6,         0,            0,               0,           -1 },
	{ "Spotify",    NULL,   NULL,       1 << 8,         0,            0,               0,           -1 },
	{ "Google-chrome", NULL, NULL,      1 << 0,         0,            1,               0,           -1 },
	{ "qutebrowser",  NULL, NULL,       1 << 0,         0,            1,               0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
#include "push.c"
#include "focusurgent.c"
#include "selfrestart.c"
#include "shiftview.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile }, /* first entry is default */
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "[M]",      monocle },
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
static const char *dmenupower[] = { "dmenu-power", NULL };
static const char *voldown[] = { "pactl", "set-sink-volume", "alsa_output.pci-0000_0b_00.3.analog-stereo", "-1%", NULL };
static const char *volup[] = { "pactl", "set-sink-volume", "alsa_output.pci-0000_0b_00.3.analog-stereo", "+1%",
 NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *layoutmenu_cmd = "layoutmenu.sh";
static const char *flamegui[] = { "flameshot", "gui", NULL};
static const char *flamefull[] = { "flameshot", "full", "-c", NULL };
static const char *browser[] = { "google-chrome-stable", NULL };
static const char *files[] = { "st", "nnn", "-C", NULL };
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
	{ MODKEY|ControlMask,           XK_j,      pushdown,       {0} },
	{ MODKEY|ControlMask,           XK_k,      pushup,         {0} },
	{ MODKEY,                       XK_w,      tabmode,        {-1} },
  { MODKEY|ShiftMask,             XK_s,      spawn,          {.v = flamegui } },
  { MODKEY|ShiftMask,             XK_z,      spawn,          SHCMD("killall trayer || trayer --expand true --widthtype request --transparent true --alpha 255 --edge top --align right") },
  { MODKEY,                       XK_p,      spawn,          {.v = flamefull } },
  { MODKEY,                       XK_x,      spawn,          {.v = files } },
  { MODKEY,                       XK_e,      spawn,          {.v = browser} },
  { MODKEY|ControlMask,           XK_w,      spawn,          {.v = killteams } },
  { MODKEY|ShiftMask,             XK_d,      spawn,          SHCMD("killall screenkey || screenkey --key-mode raw -p bottom --opacity 0.2 --bg-color '#ebdbb2' -s small -t 3") },
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
	{ MODKEY,                          XK_u,      focusurgent,    {0} },
	{ MODKEY|ControlMask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY|ControlMask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|ControlMask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },
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
	{ MODKEY,			XK_comma,  shiftview,	   { .i = -1 } },
	{ MODKEY,			XK_period, shiftview,	   { .i = 1 } },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY,                       XK_s,      togglesticky,   {0} },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
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
	{ MODKEY|ShiftMask,             XK_e,      spawn,           {.v = dmenupower } },
        { MODKEY|ShiftMask,             XK_c,      self_restart,   {0} },
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
};
