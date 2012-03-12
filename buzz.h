/* Shared header between user library & kernel library */
void init_bzz(bzz_t *, int, useconds_t);
void bzz_color(int, bzz_t *);
void bzz_lock(bzz_t *);
void bzz_release(bzz_t *);
void bzz_kill(bzz_t *);
