/* See LICENSE file for copyright and license details. */

/* appearance */
static const char font[]            = "-*-terminus-*-*-*-*-24-*-*-*-*-*-*-*";
static const char normbordercolor[] = "#000000";
static const char normbgcolor[]     = "#000000";
static const char normfgcolor[]     = "#bbbbbb";
static const char selbordercolor[]  = "#00ff00";
static const char selbgcolor[]      = "#000000";
static const char selfgcolor[]      = "#00ff00";
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 64;       /* snap pixel */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */

// Horizontally tiled layout
static void htile(Monitor *m);

// Attachaside flag
static Bool ataside           = False;
static Bool focaside          = False;
static void toggleaside();

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "MPlayer",   NULL,       NULL,       0,       False,       1 },
};

/* layout(s) */
static const float mfact      = 0.85; /* factor of master area size [0.05..0.95] */
static const Bool resizehints = False; /* False means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[O]",      monocle },
	{ "[]|",      htile },
	{ "><>",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,      {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,       {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ MODKEY|Mod1Mask,              KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|Mod1Mask,              KEY,      view,           {.ui = 1 << TAG} },

// For multimedia keys
#include <X11/XF86keysym.h>

// For keyboard layout handling
#include <X11/XKBlib.h>

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *dmenucmd[] = { "dmenu_run", "-i", "-fn", "-*-terminus-*-*-*-*-50-*-*-*-*-*-*-*", "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, "-xs", "-l", "10", "-c", "-rs", NULL };
static const char *termcmd[] = { "bash", "/root/scripts/term.sh", NULL };
static const char *alltermscmd[] = { "bash", "/root/scripts/allterms.sh", NULL };
static const char *eject[]  = { "bash", "/root/scripts/dmntnir.sh", NULL };
static const char *escflash[]  = { "bash", "/root/scripts/escflash.sh", NULL };
static const char *volumeup[]  = { "bash", "/root/scripts/vol.sh", "1%+", NULL };
static const char *volumedown[]  = { "bash", "/root/scripts/vol.sh", "1%-", NULL };
static const char *volumemute[]  = { "bash", "/root/scripts/vol.sh", "toggle", NULL };
static const char *toggleplay[]  = { "bash", "/root/scripts/anyremote.sh", "pause", NULL };
static const char *vidplay[]  = { "bash", "/root/scripts/svidplay.sh", NULL };
static const char *mpdplay[]  = { "bash", "/root/scripts/mpdplay.sh", NULL };
static const char *comix[]  = { "bash", "/root/scripts/comix.sh", NULL };
static const char *brwscmd[] = { "bash", "/root/scripts/runbrowser.sh", NULL };
static const char *gmalcmd[] = { "bash", "/root/scripts/runbrowser.sh", "https://mail.google.com", NULL };
static const char *gcalcmd[] = { "bash", "/root/scripts/runbrowser.sh", "https://www.google.com/calendar/render", NULL };
 
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_z,      spawn,          {.v = alltermscmd } },
	{ MODKEY,                       XK_a,      toggleaside,    {0} },
	{ MODKEY,                       XK_u,      spawn,          {.v = brwscmd } },
	{ MODKEY,                       XK_g,      spawn,          {.v = gmalcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_r,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_r,      zoom,           {0} },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_grave,  zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_d,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_period, focusmon,       {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY,                       XK_Up,     spawn,          {.v = escflash } },
	{ AnyKey,                       XF86XK_Eject, spawn, {.v = eject } },
	{ AnyKey,                       XF86XK_AudioRaiseVolume, spawn, {.v = volumeup } },
	{ AnyKey,                       XF86XK_AudioLowerVolume, spawn, {.v = volumedown } },
	{ AnyKey,                       XF86XK_AudioMute, spawn, {.v = volumemute } },
	{ AnyKey,                       XF86XK_AudioPlay, spawn, {.v = toggleplay } },
	{ AnyKey,                       XF86XK_Video, spawn, {.v = vidplay } },
	{ AnyKey,                       XF86XK_Music, spawn, {.v = mpdplay } },
	{ AnyKey,                       XF86XK_Pictures, spawn, {.v = comix } },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[1]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkStatusText,        0,              Button1,        spawn,          {.v = gmalcmd } },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = brwscmd } },
	{ ClkStatusText,        0,              Button3,        spawn,          {.v = gcalcmd } },
};

static void
htile(Monitor *m) {
	int x, y, h, w, mh;
	unsigned int i, n;
	Client *c;

    for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
    if(n == 0)
        return;
    /* master */
    c = nexttiled(m->clients);
    mh = m->mfact * m->wh;
    resize(c, m->wx, m->wy, m->ww - 2 * c->bw, (n == 1 ? m->wh : mh) - 2 * c->bw, False);
    if(--n == 0)
        return;
    /* tile stack */
    x = m->wx;
    y = (m->wy + mh > c->y + c->h) ? c->y + c->h + 2 * c->bw : m->wy + mh;
    w = m->ww / n;
    h = (m->wy + mh > c->y + c->h) ? m->wy + m->wh - y : m->wh - mh;
    if(h < bh)
        h = m->wh;
    for(i = 0, c = nexttiled(c->next); c; c = nexttiled(c->next), i++) {
        resize(c, x, y, ((i + 1 == n) ? m->wx + m->ww - x : w) - 2 * c->bw, h - 2 * c->bw, False);
        if(w != m->ww)
            x = c->x + WIDTH(c);
    }
}

static void
toggleaside() {
    ataside = !ataside;
}
