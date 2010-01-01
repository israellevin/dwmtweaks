/* See LICENSE file for copyright and license details. */

/* appearance */
static const char font[]            = "-*-terminus-*-*-*-*-24-*-*-*-*-*-*-*";
static const char normbordercolor[] = "#000000";
static const char normbgcolor[]     = "#000000";
static const char normfgcolor[]     = "#bbbbbb";
static const char selbordercolor[]  = "#00ff00";
static const char selbgcolor[]      = "#000000";
static const char selfgcolor[]      = "#00ff00";
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 64;       /* snap pixel */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */

// Horizontally tiled layout
static void htile(Monitor *m);

// TV hack
static Client *tvc = NULL;
static void totv(const Arg *arg);
static void fromtv(const Arg *arg);

/* tagging */
static const char *tags[] = { "1", "2", "3" };

static const Rule rules[] = {
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "MPlayer",   NULL,       NULL,       0,       False,       1 },
};

/* layout(s) */
static const float mfact      = 0.85; /* factor of master area size [0.05..0.95] */
static const Bool resizehints = True; /* False means respect size hints in tiled resizals */

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
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
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
static const char *dmenucmd[] = { "dmenu_run", "-b", "-fn", "-*-terminus-*-*-*-*-32-*-*-*-*-*-*-*", "-nb", "#ff0000", "-nf", "#000000", "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[] = { "urxvtcd", "-fade", "30", "-fg", "grey", "-bg", "black", "-cr", "green", "-fn", "-*-terminus-*-*-*-*-32-*-*-*-*-*-*-*", "-vb", "+sb", "-b", "0", "-w", "0", "--color12", "white", NULL };
static const char *termcmd2[]  = { "konsole", "--background-mode", NULL };
static const char *krunnercmd[]  = { "krunner", NULL };
static const char *volumeup[]  = { "bash", "/root/scripts/vol.sh", "1%+", NULL };
static const char *volumedown[]  = { "bash", "/root/scripts/vol.sh", "1%-", NULL };
static const char *volumemute[]  = { "bash", "/root/scripts/vol.sh", "toggle", NULL };
static const char *toggleplay[]  = { "bash", "/root/scripts/anyremote.sh", "pause", NULL };
static const char *vidplay[]  = { "bash", "/root/scripts/vidplay.sh", NULL };
static const char *mpdplay[]  = { "bash", "/root/scripts/mpdplay.sh", NULL };
static const char *uzblcmd[] = { "uzbl-browser", NULL };
static const char *gmalcmd[] = { "uzbl-browser", "-u", "http://mail.google.com", NULL };
static const char *gcalcmd[] = { "uzbl-browser", "-u", "http://www.google.com/calendar/render", NULL };
 
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ControlMask,           XK_Return, spawn,          {.v = termcmd2 } },
	{ ControlMask,                  XK_space,  spawn,          {.v = krunnercmd } },
	{ MODKEY,                       XK_u,      spawn,          {.v = uzblcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_r,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_r,      zoom,           {0} },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
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
    { MODKEY,                       XK_Right,  totv,           {.i = 0} },
    { MODKEY,                       XK_Left,   fromtv,         {.i = 0} },
    { MODKEY,                       XK_Down,   fromtv,         {.i = 1} },
	{ AnyKey,	XF86XK_AudioRaiseVolume,	spawn,	{.v = volumeup } },
	{ AnyKey,	XF86XK_AudioLowerVolume,	spawn,	{.v = volumedown } },
	{ AnyKey,	XF86XK_AudioMute,	spawn,	{.v = volumemute } },
	{ AnyKey,	XF86XK_AudioPlay,	spawn,	{.v = toggleplay } },
	{ AnyKey,	XF86XK_Video,	spawn,	{.v = vidplay } },
	{ AnyKey,	XF86XK_Music,	spawn,	{.v = mpdplay } },
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
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = uzblcmd } },
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

void fromtv(const Arg *arg) {
    if(tvc && ISVISIBLE(tvc)) {
        if(arg->i == 1) {
            XWindowAttributes wa;
            if((XGetWindowAttributes(dpy, tvc->win, &wa)) && (wa.map_state == IsViewable)){
                tvc->isfloating = True;
                resize(tvc, 0, 0, tvc->w, tvc->h, False);
                sendmon(tvc, mons);
                focus(tvc);
                tvc = NULL;
            }
        }
        else {
            tvc->isfloating = False;
            sendmon(tvc, mons);
            tvc = NULL;
        }
    }
}

void totv(const Arg *arg) {
    Client *oldsel = mons->sel;
    if(oldsel && ISVISIBLE(oldsel)) {
        Arg arg = {0};
        arg.i = 0;
        fromtv(&arg);
        sendmon(oldsel, mons->next);
        tvc = oldsel;
    }
}
