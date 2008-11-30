void
tv(const Arg *arg) {
    static XWindowAttributes wa;
    Window w;
    int x, y;
    unsigned int width_return;
    unsigned int height_return;
    unsigned int bw;
    unsigned int dp;

    unsigned int offh;

    sel->isfloating = True;
    sel->bw = 0;
    sel->tags = TAGMASK;
    resize(sel, 0, 0, 1920, 1200, False);

//    XGetWindowAttributes(dpy, sel->win, &wa);
//    offh = 1200 - wa.height;

    XGetGeometry(dpy, sel->win, &w, &x, &y, &width_return, &height_return, &bw, &dp); 
    offh = 1201 - height_return;

    if(offh > 0) {
//        resize(sel, 0, offh / 2, 1920, 1200, True);
        resize(sel, 0, 2, 492, 420, True);
    }
    arrange();
}
//	resize(sel, 1980, 25, 925, 710, True);
