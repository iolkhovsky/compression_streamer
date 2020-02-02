#pragma once

#include <stddef.h>
#include <vector>

using std::vector;

template<typename It>
class IteratorRange {
    IteratorRange(It begin, It end)
        : _begin_it(begin), _end_it(end) {
    }
    It begin() {
        return _begin_it;
    }
    It end() {
        return _end_it;
    }
private:
    It _begin_it;
    It _end_it;
};

template<typename It>
class Page {
public:
    Page(IteratorRange rng, size_t sz)
        : _range(rng), _size(sz) {
    }
    It Begin() const {
        return _range.begin();
    }
    It End() const {
        return _range.end();
    }
    size_t Size() const {
        return _size;
    }
private:
    IteratorRange _range;
    size_t _size;
};

template<typename It>
class Paginator {
public:
    using PageIt = typename vector<Page<It>>::iterator;

    Paginator(IteratorRange<It> range, size_t page_sz)
        : _range(range), _page_sz(page_sz) {
    }
    size_t GetPageCount() const {

    }
    size_t GetPage(size_t id) const {

    }
    PageIt Begin() const {

    }
    PageIt End() const {

    }
private:
    IteratorRange<It> _range;
    size_t _page_sz;
    vector<Page> _pages;
};
