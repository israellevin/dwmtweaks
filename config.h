/* See LICENSE file for copyright and license details. */

/* appearance */
static const char font[]            = "-*-terminus-*-*-*-*-24-*-*-*-*-*-*-*";
static const char normbordercolor[] = "#000000";
static const char normbgcolor[]     = "#000000";
static const char normfgcolor[]     = "#bbbbbb";
static const char selbordercolor[]  = "#00ff00";
static const char selbgcolor[]      = "#000000";
static const char selfgcolor[]      = "#00ff00";
static unsigned int borderpx        = 1;        /* border pixel of windows */
static unsigned int snap            = 64;       /* snap pixel */
static Bool showbar                 = False;    /* False means no bar */
static Bool topbar                  = True;     /* False means bottom bar */

// Horizontally tiled layout
static void htile(void);

// TV hack
static Bool freemouse = False;
static int screensizex = 1920;
static int screensizey = 1200;
static Client *tvc = NULL;
static void togglefreemouse(const Arg *arg);
static void tv(const Arg *arg);

/* tagging */
static const char tags[][MAXTAGLEN] = { "1", "2", "3" };
static unsigned int tagset[] = {1, 1}; /* after start, first tag is selected */

static Rule rules[] = {
	/* class      instance    title       tags mask     isfloating */
    { "stam",  NULL,       NULL,       1 << 8,       True },
};

/* layout(s) */
static float mfact      = 0.85; /* factor of master area size [0.05..0.95] */
static Bool resizehints = False; /* False means respect size hints in tiled resizals */

static Layout layouts[] = {
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
static const char *termcmd[] = { "rxvt", "-fade", "30", "-fg", "grey", "-bg", "black", "-cr", "green", "-fn", "-*-terminus-*-*-*-*-32-*-*-*-*-*-*-*", "-vb", "+sb", "-b", "0", "-w", "0", "--color12", "white", NULL };
static const char *termcmd2[]  = { "konsole", "--background-mode", NULL };
static const char *krunnercmd[]  = { "krunner", NULL };
static const char *volumeup[]  = { "bash", "/root/scripts/vol.sh", "1%+", NULL };
static const char *volumedown[]  = { "bash", "/root/scripts/vol.sh", "1%-", NULL };
static const char *volumemute[]  = { "bash", "/root/scripts/vol.sh", "toggle", NULL };
static const char *vidplay[]  = { "bash", "/root/scripts/vidplay.sh", NULL };
static const char *mpdplay[]  = { "bash", "/root/scripts/mpdplay.sh", NULL };
 
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ControlMask,	        XK_Return, spawn,          {.v = termcmd2 } },
	{ ControlMask,	                XK_space,  spawn,          {.v = krunnercmd } },
	{ MODKEY,                       XK_a,      togglefreemouse,{0} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,	                    XK_c,      killclient,     {0} },
	{ MODKEY,	                    XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_d,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
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
	{ MODKEY,                       XK_Right,  tv,             {.i = 0} },
	{ MODKEY,                       XK_Right,  focusstack,     {.i = 1} },
	{ MODKEY,                       XK_Left,   tv,             {.i = 1} },
	{ MODKEY,                       XK_Down,   tv,             {.i = 2} },
	{ AnyKey,	XF86XK_AudioRaiseVolume,	spawn,	{.v = volumeup } },
	{ AnyKey,	XF86XK_AudioLowerVolume,	spawn,	{.v = volumedown } },
	{ AnyKey,	XF86XK_AudioMute,	spawn,	{.v = volumemute } },
	{ AnyKey,	XF86XK_Video,	spawn,	{.v = vidplay } },
	{ AnyKey,	XF86XK_Music,	spawn,	{.v = mpdplay } },
};

/* button definitions */
/* click can be a tag number (starting at 0),
 * ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
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

static void htile(void) {
       int x, y, h, w, mh;
       unsigned int i, n;
       Client *c;

       for(n = 0, c = nexttiled(clients); c; c = nexttiled(c->next), n++);
       if(n == 0)
               return;

       /* master */
       c = nexttiled(clients);
       mh = mfact * wh;
       resize(c, wx, wy, ww - 2 * c->bw, (n == 1 ? wh : mh) - 2 * c->bw);

       if(--n == 0)
               return;

       /* tile stack */
       x = wx;
       y = (wy + mh > c->y + c->h) ? c->y + c->h + 2 * c->bw : wy + mh;
       w = ww / n;
       h = (wy + mh > c->y + c->h) ? wy + wh - y : wh - mh;
       if(h < bh)
               h = wh;

       for(i = 0, c = nexttiled(c->next); c; c = nexttiled(c->next), i++) {
               resize(c, x, y, ((i + 1 == n) ? wx + ww - x : w) - 2 * c->bw, h - 2 * c->bw);
               if(w != ww)
                       x = c->x + WIDTH(c);
       }
}

void togglefreemouse(const Arg *arg) {
    freemouse = !freemouse;
    drawbar();
}

// Handles TV tiling
// 0 means send to TV
// 1 means retrieve from TV
// 2 means retrieve from TV but keep afloat
void tv(const Arg *arg) {
    Bool oldmouse = freemouse;
    Client *oldsel = sel;
	XWindowAttributes wa;

    freemouse = True;

    if(tvc && ISVISIBLE(tvc)) {
        if((XGetWindowAttributes(dpy, tvc->win, &wa)) && (wa.map_state == IsViewable)) {
            if(arg->i == 2) {
                resize(tvc, 0, 0, tvc->w, tvc->h);
            } else {
                tvc->isfloating = False;
//                tvc->bw = borderpx;
                tvc->tags = tagset[seltags];
            }
            focus(tvc);
        }
        tvc = NULL;
    }
    if(arg->i == 0 && oldsel && ISVISIBLE(oldsel)) {
        int x, y, w, h, nw, nh, ow, oh;
        x = 1980 + 10;
        y = 25;
        w = nw = 925;
        h = nh = 710;

        if(oldsel->mina > 0 && oldsel->maxa > 0) {
            if(oldsel->maxa < (float)w / h)
                nw = h * oldsel->maxa;
            else if(oldsel->mina < (float)h / w)
                nh = w * oldsel->mina;
        }

        ow = w - nw;
        oh = h - nh;

        if(ow > 0) x += ow / 2;
        if(oh > 0) y += oh / 2;

        oldsel->isfloating = True;
//        oldsel->bw = 0;
        oldsel->tags = TAGMASK;
        resize(oldsel, x, y, nw, nh);
        tvc = oldsel;
    }
    arrange();
    freemouse = oldmouse;
}

