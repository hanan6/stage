#ifndef ZIP_ITERATOR
#define ZIP_ITERATOR

template<class Iterator1, class Iterator2>
class zip_iterator {
public:
    Iterator1 first;
    Iterator2 second;
    zip_iterator(Iterator1 i1, Iterator2 i2)
    : first(i1), second(i2)
    {}
    zip_iterator & operator++() {
        ++first; ++second; return *this;
    }
    zip_iterator & operator *() {
        return *this;
    }
    bool operator == (const zip_iterator &other) {
            return !(*this != other);
    }
    bool operator != (const zip_iterator &other) {
            return first != other.first && second != other.second;
    }
};


template<class Iterator1, class Iterator2>
zip_iterator<Iterator1, Iterator2> make_zip_iterator(Iterator1 it1, Iterator2 it2) {
    return zip_iterator<Iterator1, Iterator2>(it1, it2);
}

#endif // ZIP_ITERATOR
