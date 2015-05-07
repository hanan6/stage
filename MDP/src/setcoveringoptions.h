#ifndef SETCOVERINGOPTIONS_H
#define SETCOVERINGOPTIONS_H

#include "matrix.h"
#include <vector>
#include <gecode/driver.hh> // Options


namespace mdp {
class SetCoveringOptions : public Gecode::Options {
    protected:
        matrix<bool>*      _supportMatrix  ; /*!< */
#if 0
        matrix<bool>*      startsExcl       ; /*!< holds if a pattern start a the negative sequences */
        matrix<bool>*      endExcl          ; /*!< holds if a pattern start a the positive sequences */
        std::vector<bool>* starts           ; /*!< holds if a pattern start a the sequence */
        std::vector<bool>* ends             ; /*!< holds if a pattern end a the sequence   */
#endif
        int                _sequences        ; /*!< */
        int                _patterns           ; /*!< */
        int                _excl;
#if 0
        int                cardMax          ; /*!< */
#endif

    public:
        SetCoveringOptions(matrix<bool> *supportMatrice, int excl);
        /*SetCoveringOptions(matrix<bool> *m, int w, int h);
        SetCoveringOptions(matrix<bool> *m, int w, int h, int e);
        SetCoveringOptions(matrix<bool> *m, int w, int h, int e, int c);
        SetCoveringOptions(matrix<bool> const &m, int w, int h, int e, int c);*/
        //!
        //! \brief h
        //! \return
        //!
        int patterns(void) const;
        //!
        //! \brief w
        //! \return
        //!
        int sequences(void) const;
        //!
        //! \brief e
        //! \return
        //!
        int e(void) const;
#if 0
        //!
        //! \brief c
        //! \return
        //!
        int c(void) const;
#endif
        //!
        //! \brief value
        //! \param x
        //! \param y
        //! \return
        //!
        bool value(int x, int y) const;
};
}

#endif // SETCOVERINGOPTIONS_H
