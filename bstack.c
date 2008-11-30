void
bstack(void) {
	int x, y, h, w, mh;
	unsigned int i, n;
	Client *c;

	for(n = 0, c = nexttiled(clients); c; c = nexttiled(c->next), n++);
	if(n == 0)
		return;

	c = nexttiled(clients);
	mh = mfact * wh;
	resize(c, wx, wy, NOBORDER(ww), NOBORDER(n == 1 ? wh : mh), resizehints);

	if(--n == 0)
		return;

	x = wx;
	y = (wy + mh > c->y + c->h) ? c->y + c->h + 2 * c->bw : wy + mh;
	w = ww / n;
	h = (wy + mh > c->y + c->h) ? wy + wh - y : wh - mh;
	if(h < bh)
		h = wh;

	for(i = 0, c = nexttiled(c->next); c; c = nexttiled(c->next), i++) {
		resize(c, x, y, ((i + 1 == n) ? NOBORDER(wx + ww) - x : w),
		       NOBORDER(h), resizehints);
		if(w != ww)
			x = c->x + c->w + 2 * c->bw;	
	}
}
