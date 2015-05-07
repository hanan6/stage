#include "setcoveringoptions.h"

using namespace Gecode;


namespace mdp {

SetCoveringOptions::SetCoveringOptions(matrix<bool> *m, int excl)
    : Options("MDP")
    , _supportMatrix(m)
    , _sequences(m->size1())
    , _patterns(m->size2())
    , _excl(excl)
{}

#if 0
SetCoveringOptions::SetCoveringOptions(matrix<bool> *m, int w, int h)
    : Options("MDP")
    , exclusionMatrice(m)
    , width(w)
    , height(h)
    , excl(0)
    , cardMax(-1)
{}

SetCoveringOptions::SetCoveringOptions(matrix<bool> *m, int w, int h, int e)
    : Options("MDP")
    , exclusionMatrice(m)
    , width(w)
    , height(h)
    , excl(e)
    , cardMax(-1)
{}

SetCoveringOptions::SetCoveringOptions(matrix<bool> *m, int w, int h, int e, int c)
    : Options("MDP")
    , exclusionMatrice(m)
    , width(w)
    , height(h)
    , excl(e)
    , cardMax(c)
{}
#endif


int SetCoveringOptions::sequences(void) const {
    return _sequences;
} // end method h

int SetCoveringOptions::patterns(void) const {
    return _patterns;
} // end method w

int SetCoveringOptions::e() const {
    return _excl;
} // end method e

#if 0
int SetCoveringOptions::c(void) const {
    return cardMax;
} // end method c

#endif
bool SetCoveringOptions::value(int s, int p) const {
    return (*_supportMatrix)(s,p);
} // end method value

} // end namespace mdp
