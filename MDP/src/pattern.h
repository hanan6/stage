#ifndef PATTERN_H
#define PATTERN_H

#include <string>  // string
#include <vector>  // vector
#include <utility> // pair
#include <algorithm>
#include <iostream>
//!
//! \brief The Pattern class
//!
class Pattern {
public:
    typedef size_t                    size_type     ; /*!< The size type.*/
    typedef std::string               sequence_type ; /*!< The type of sequence.*/
    typedef sequence_type::value_type value_type    ; /*!< The of characters of the pattern.*/
private:
    typedef std::pair<size_type, size_type> p_size_type ; /*! pair of size_type */
    typedef std::vector<p_size_type>        v_size_type ; /*!< Vecotr of (size_type, size_type)*/

    sequence_type m_solids   ; /*!< The sequence of solid caracters */
    v_size_type   m_sections ; /*!< The indices of the charatcters that are preceded by a gap, and the size of the gaps */
    size_type     m_slack    ; /*!< The slack of the pattern */
    size_type     m_size     ; /*!< The size of the pattern */
    sequence_type m_tmp      ; /*!< A */


    //!
    //! \brief The iterator_impl class
    //!
    class iterator_impl {
    public:
        typedef std::pair<Pattern::value_type, size_type> value_type ; /*!< */
    private:
        Pattern    const *p   ; /*!< */
        value_type        v   ; /*!< The value of the character and the gap with the last move */
        size_type         i_s ; /*!< The indice of the current solid characters */
        size_type         i_g ; /*!< The indice of the current gap section */
    public:
        //!
        //! \brief iterator_impl
        //! \param p
        //!
        iterator_impl(Pattern const* p_)
            : p(p_), v(p_->m_solids[0], 0), i_s(0), i_g(0)
        {}

        //!
        //! \brief iterator_impl
        //! \param p
        //!
        iterator_impl(iterator_impl const &src)
            : p(src.p), v(src.v), i_s(src.i_s), i_g(src.i_g)
        {}

        //!
        //! \brief iterator_impl
        //! \param p
        //!
        iterator_impl(Pattern const* p_, bool end)
            : p(p_), v(Pattern::value_type(), -1), i_s(p_->m_solids.size()), i_g(p_->m_sections.size())
        {}

        //!
        //! \brief iterator_impl
        //! \param p
        //!
        iterator_impl operator=(iterator_impl const &rhs) {
            if (this != &rhs) {
                p   = rhs.p   ;
                v   = rhs.v   ;
                i_s = rhs.i_s ;
                i_g = rhs.i_g ;
            }
            return *this;
        }


        //!
        //! \brief operator *
        //! \return
        //!
        value_type operator*(void) {
            return v;
        }

        value_type* operator->(void) {
            return &v;
        }

        //!
        //! \brief operator ++
        //! \return
        //!
        iterator_impl operator++(void) {
            iterator_impl ret{*this};
            increment();
            return ret;
        }

        //!
        //! \brief operator ++
        //! \return
        //!
        iterator_impl operator++(int) {
            increment();
            return *this;
        }

        bool operator==(iterator_impl const &rhs) {
            return p   == rhs.p
                && v   == rhs.v
                && i_s == rhs.i_s
                && i_g == rhs.i_g;
        }

        bool operator!=(iterator_impl const &rhs) {
            return !(p   == rhs.p
                  && v   == rhs.v
                  && i_s == rhs.i_s
                  && i_g == rhs.i_g);
        }

        //!
        //! \brief increment the iterator first, then advance the iterator second
        //!        such that second advance as much as first ?????
        //! \param obj
        //! \param dep
        //! \return true if there is a corresponding solid char between first and second
        //!
        bool incrementIteratorThen(iterator_impl &rhs) {
            ++(*this);
            if(*this == iterator_impl(p, false))
                return true;
            ++rhs;
            size_t off_this = (*this)->second + 1;
            size_t off_rhs  = rhs->second + 1;
            while(off_this > off_rhs) {
               ++rhs;
               off_rhs  += rhs->second + 1;
             }


            return off_rhs == off_this;
        }

    private:
        void increment(void) {
            ++i_s;
            if(i_s < p->m_solids.size()) {
                Pattern::value_type nv  = p->m_solids[i_s];
                size_type           gap = (i_g < p->m_sections.size() && i_s == p->m_sections[i_g].first) ?
                                          p->m_sections[i_g++].second : 0;
                v = std::make_pair(nv, gap);
            } else { // end of the pattern
                *this = iterator_impl(p, false);
            }
        }
    };


public:
    typedef iterator_impl iterator;
    typedef iterator_impl const_iterator;

    //!
    //! \brief Pattern
    //!
    Pattern();
    //!
    //! \brief Pattern
    //! \param src
    //!
    Pattern(Pattern const& src);
    //!
    //! \brief Pattern
    //! \param src
    //!
    Pattern(std::string const& src);

    //!
    //! \brief Pattern
    //! \param begin
    //! \param end
    //!
    template<class ForwardIterator>
    Pattern(ForwardIterator begin, ForwardIterator end)
    : m_solids(), m_sections(), m_slack(), m_size(), m_tmp()
    {
        __build(begin, end);
    }

    //!
    //! \brief Pattern
    //! \param src
    //!
    Pattern(Pattern&& src);

    ~Pattern();

    //!
    //! \brief operator =
    //! \param p
    //! \return
    //!
    Pattern& operator=(Pattern const& p);

    //!
    //! \brief operator =
    //! \param p
    //! \return
    //!
    Pattern& operator=(Pattern&& p);

    //!
    //! \brief search
    //! \param src
    //! \return
    //!
    bool search(Pattern const& src) const;

    //!
    //! \brief search
    //! \param src
    //! \return
    //!
    bool search(std::string const &src) const;


    //!
    //! \brief search
    //! \param src
    //! \return
    //!
    size_t search(std::string const &src, size_t start) const;

    //!
    //! \brief operator ==
    //! \param rhs
    //! \return
    //!
    bool operator==(const Pattern& rhs) const;

    //!
    //! \brief slack
    //! \return
    //!
    size_type slack(void) const;

    //!
    //! \brief size
    //! \return
    //!
    size_type size(void) const;

    //!
    //! \brief solids
    //! \return
    //!
    size_type solids(void) const;

    //!
    //! \brief The grSlack struct
    //!
    struct grSlackPartial {
        bool operator()(const Pattern& lhs, const Pattern& rhs) const {
            return lhs.m_slack > rhs.m_slack;
        }
    };

    //!
    //! \brief The grSlack struct
    //!
    struct grSlackTotal {
        bool operator()(const Pattern& lhs, const Pattern& rhs) const {
            return
            (lhs.m_slack > rhs.m_slack)
         || (lhs.m_slack == rhs.m_slack && lhs.m_size > rhs.m_size)
         || (lhs.m_slack == rhs.m_slack && lhs.m_size == rhs.m_size && lhs.solids() > rhs.solids())
         || (lhs.m_slack == rhs.m_slack && lhs.m_size == rhs.m_size && lhs.solids() == rhs.solids() && lhs.m_tmp < rhs.m_tmp);
        }
    };

    //!
    //! \brief The grSizeRelax struct partial ordering with the size
    //!
    struct grSizePartial {
        bool operator()(const Pattern& lhs, const Pattern& rhs) const {
            return lhs.m_size > rhs.m_size;
       }
    };
    //!
    //! \brief The grSize struct total oredering with the size
    //!
    struct grSizeTotal {
        bool operator()(const Pattern& lhs, const Pattern& rhs) const {
            return
                   (lhs.m_size > rhs.m_size)
                || (lhs.m_size == rhs.m_size && lhs.m_slack > rhs.m_slack)
                || (lhs.m_slack == rhs.m_slack && lhs.m_size == rhs.m_size && lhs.solids() > rhs.solids())
                || (lhs.m_slack == rhs.m_slack && lhs.m_size == rhs.m_size && lhs.solids() == rhs.solids() && lhs.m_tmp < rhs.m_tmp);
        }
    };



    //!
    //! \brief split
    //! \param slack
    //! \return
    //!
    std::vector<Pattern> split_by_slack(size_t slack) const;

    //!
    //! \brief split
    //! \param slack
    //! \param src
    //! \return
    //!
    template<class Iterator>
    size_type split_by_slack(size_t slack, Iterator src) const;

    //!
    //! \brief toString
    //! \return
    //!
    std::string toString(void) const;

    //!
    //! \brief begin
    //! \return
    //!
    iterator begin(void) const;

    //!
    //! \brief end
    //! \return
    //!
    iterator end(void) const;

    //!
    //! \brief cbegin
    //! \return
    //!
    const_iterator cbegin(void) const;

    //!
    //! \brief cend
    //! \return
    //!
    const_iterator cend(void) const;

    //!
    //! \brief operator <<
    //! \param os
    //! \param obj
    //! \return
    //!
    friend std::ostream& operator<<(std::ostream& os, Pattern const& obj);

    //!
    //! \brief swap
    //! \param lhs
    //! \param rhs
    //!
    friend void swap(Pattern& lhs, Pattern &rhs);
   /* friend std::vector<Pattern> computeCommonPatternsSlackMemory
        ( sequence_type const &s
        , Pattern       const &p
        , int maxslack = 0
        , int sizeMin = 2);
    friend Pattern subPattern
    ( std::string const & s, Pattern const & p
    , size_t begin_s
    , iterator begin_p, iterator end_p
    );*/
private:
    template<class ForwardIterator>
    void __build(ForwardIterator begin, ForwardIterator end)  {
        m_slack = 0;
        m_size  = 0;
        size_type curSlack = 0;
        size_type i = 0;
        while(begin != end) {
            m_tmp += *begin;
            if(*begin != '.') {
                m_solids += *begin;
                ++i;
                // end of slack section
                if(curSlack != 0) {
                    m_slack    = std::max(m_slack, curSlack);
                    m_sections.emplace_back(i-1, curSlack);
                    curSlack = 0;
                }
            } else {
                ++curSlack;
            }
            ++begin;++m_size;
        }
        // incomplete patterns
        if(curSlack != 0) {
            // TO-DO
        }
    }


};

#endif // PATTERN_H
