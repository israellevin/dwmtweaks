void
tv(const Arg *arg) {
    int x, y, w, h, nw, nh, ow, oh;

    x = 1980;
    y = 25;
    w = nw = 925;
    h = nh = 710;

    if(sel->mina > 0 && sel->maxa > 0) {
        if(sel->maxa < (float)w / h)
            nw = h * sel->maxa;
        else if(sel->mina < (float)h / w)
            nh = w * sel->mina;
    }

    ow = w - nw;
    oh = h - nh;

    if(ow > 0) x += ow / 2;
    if(oh > 0) y += oh / 2;

    sel->isfloating = True;
    sel->bw = 0;
    sel->tags = TAGMASK;
    resize(sel, x, y, nw, nh, False);
    arrange();
}
