#include "setcovering.h"
#include <iostream>
#include <algorithm>
#include <iterator>

using namespace std;
using namespace Gecode;


namespace mdp {

SetCovering::SetCovering(const SetCoveringOptions& opt)
    : blocs(*this, opt.patterns(), 0, 1)
    , seqs(*this, opt.sequences(), 0, 1)
    , seqsTmp(*this, opt.sequences(), 0, opt.patterns())
    , supByBl(*this, opt.sequences()*opt.patterns(), 0, opt.patterns())
    //, obj(*this, opt.e() * opt.h(), opt.h() * opt.w() + opt.h())
    , obj(*this, 0, opt.patterns())
    , card(*this, 1, opt.patterns())
    , excl(*this, opt.e()+1, opt.sequences())
{
    rel(*this, card == sum(blocs));
    //rel(*this, obj == (opt.h() - card) + excl * opt.h());
    rel(*this, obj == (opt.patterns() - card));
    // To use linear we need to use Array of IntArgs.
    IntArgs excluded(opt.patterns()*opt.sequences());
    Matrix<IntArgs> mat(excluded, opt.sequences(), opt.patterns());
    Matrix<IntVarArray> matSupByBl(supByBl, opt.sequences(), opt.patterns());
    vector<int> zeros;
    for(int i = 0; i < blocs.size(); ++i) {
        int zero = 0;
        for(int j = 0; j < seqs.size(); ++j) {
            if(mat(j,i) == 0)
                zero++;
        }
        zeros.push_back(zero);
    }
    for(int j = 0; j < seqs.size(); ++j) {
        bool disj = false;
        for(int i = 0; i < blocs.size(); ++i) {
            //cout << j << " " << i << endl;
            mat(j, i) = !opt.value(j, i);
            disj |= mat(j,i);
        };
        rel(*this, seqs[j] == disj);
    }
    rel(*this, excl == sum(seqs));


    for(int j = 0; j < seqs.size(); ++j) {
        linear(*this, mat.col(j), blocs, IRT_GR, 0, imp(seqs[j]));
    }

    // Each bloc has to exclude AT LEAST one unique sequence
    for(int j = 0; j < seqs.size(); ++j) {
        linear(*this, mat.col(j), blocs, IRT_EQ, seqsTmp[j]);
    }
    //count(*this, seqsTmp, 1, IRT_GQ, card);

    for(int i = 0; i < blocs.size(); ++i) {
        IntVar ct(*this, 0, opt.sequences());
        for(int j = 0; j < seqs.size(); ++j) {
            rel(*this, matSupByBl(j, i) == (seqsTmp[j] - mat(j,i)));
        }
        count(*this, matSupByBl.row(i), 0, IRT_EQ, ct);
        rel(*this, (ct != zeros[i]) == blocs[i]);
        //cout << matSupByBl.row(i) << endl;
        //IntVar one(*this, 1, 1);
        //member(*this, matSupByBl.row(i), one, imp(blocs[i]));
    }

    for(int i = 0; i < blocs.size(); ++i) {
        for(int ii = 0; ii< blocs.size(); ++ii) {
            if(i != ii) {
                bool inclusion = true;
                bool strict   = false;

                for(int j = 0; j < seqs.size() && inclusion; ++j) {
                    if(mat(j,ii) && !mat(j,i))
                        inclusion = false;
                    if(!mat(j,ii) && mat(j,i))
                        strict = true;
                }
                if(inclusion && strict) {
                    rel(*this, blocs[ii] == false);
                }
            }
        }
    }
    /*
    for(int i = 0; i < blocs.size(); ++i) {
        BoolVar incl(*this, 0, seqs.size());

        for(int j = 0; j < seqs.size() && inclusion; ++j) {
            rel(*this, (mat(j,i) >> seqs[j]))
            inclusion = false;
            if(!mat(j,ii) && mat(j,i))
                        strict = true;
        }
    }*/


    branch(*this, blocs, INT_VAR_AFC_MAX(), INT_VAL_MAX());
}

IntVar SetCovering::cost(void) const {
    return obj;
}

SetCovering::SetCovering(bool share, SetCovering& s) : MaximizeScript(share, s) {
    blocs.update(*this, share, s.blocs);
    seqs.update(*this, share, s.seqs);
    seqsTmp.update(*this, share, s.seqsTmp);
    supByBl.update(*this, share, s.supByBl);
    obj.update(*this, share, s.obj);
    card.update(*this, share, s.card);
    excl.update(*this, share, s.excl);
}

Space* SetCovering::copy(bool share) {
    return new SetCovering(share, *this);
}

int SetCovering::getCard() const {
    return card.val();
}

int SetCovering::getExclusion() const {
    return excl.val();
}

void SetCovering::print(std::ostream &os) const  {
    os << blocs << std::endl;
    os << seqs << std::endl;
    os << "card = " << card << std::endl;
    os << "Excl = " << excl << "/" << seqs.size() << std::endl;
}

vector<bool> SetCovering::getSelectedPatterns(void) const {
    vector<bool> ret;
    transform
        ( blocs.begin(), blocs.end()
        , back_inserter(ret)
        , [](BoolVar v) {
            return v.val();
          }
        );

    return ret;
}

}
