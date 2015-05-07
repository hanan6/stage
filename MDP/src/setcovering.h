#ifndef SETCOVERING_H
#define SETCOVERING_H

#include <gecode/int.hh>        // IntVar, BoolVarArray
#include <gecode/driver.hh>     // MaximizeScript
#include <ostream>              // ostream
#include "setcoveringoptions.h" // SetCoveringOptions


namespace mdp {
//!
//! \brief The SetCovering class
//!
class SetCovering : public Gecode::MaximizeScript {
protected:
    Gecode::BoolVarArray blocs ; /*!< */
    Gecode::BoolVarArray seqs  ; /*!< */
    Gecode::IntVarArray seqsTmp  ; /*!< */
    Gecode::IntVarArray  supByBl;
    Gecode::IntVar       obj   ; /*!< */
    Gecode::IntVar       card  ; /*!< */
    Gecode::IntVar       excl  ; /*!< */

public:
    //!
    //! \brief SetCovering
    //! \param opt
    //!
    SetCovering(const SetCoveringOptions& opt);
    //!
    //! \brief cost
    //! \return
    //!
    virtual Gecode::IntVar cost(void) const;
    //!
    //! \brief SetCovering
    //! \param share
    //! \param s
    //!
    SetCovering(bool share, SetCovering& s);
    //!
    //! \brief copy
    //! \param share
    //! \return
    //!
    virtual Space* copy(bool share);

    //!
    //! \brief getSelectedBlocks
    //! \param it
    //!
    template<class Iterator>
    void getSelectedBlocks(Iterator it) const {
        for(int i = 0; i < blocs.size(); ++i)
            *it++ = blocs[i].val();
    }
    //!
    //! \brief getSelectedBlocks
    //! \return
    //!
    template<class Iterator>
    std::vector<bool> getSelectedBlocks(void) const;
    //!
    //! \brief getExcludedSequences
    //! \param it
    //!
    template<class Iterator>
    void getExcludedSequences(Iterator it) const {
        for(int i = 0; i < seqs.size(); ++i)
            *it++ = seqs[i].val();
    }
    //!
    //! \brief getExcludedSequences
    //! \return
    //!
    std::vector<bool> getSelectedPatterns(void) const;
    //!
    //! \brief getCard
    //! \return
    //!
    int getCard(void) const;
    //!
    //! \brief getExclusion
    //! \return
    //!
    int getExclusion(void) const;
    //!
    //! \brief print
    //! \param os
    //!
    virtual void print(std::ostream &os) const;
};
}
#endif
