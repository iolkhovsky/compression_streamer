#pragma once

#include <stddef.h>
#include <vector>
#include <stdexcept>

using std::vector;
using std::runtime_error;
using std::distance;
using std::min;

template<typename It>
class IteratorRange {
public:
    IteratorRange() = default;
    IteratorRange(It begin, It end)
        : _begin_it(begin), _end_it(end) {
    }
    It begin() {
        return _begin_it;
    }
    It end() {
        return _end_it;
    }
    size_t get_size() const {
        return distance(_begin_it, _end_it);
    }
private:
    It _begin_it;
    It _end_it;
};

template<typename It>
class Page {
public:
    Page() = default;
    Page(IteratorRange<It> rng, size_t sz)
        : _range(rng), _size(sz) {
    }
    It begin() {
        return _range.begin();
    }
    It end() {
        return _range.end();
    }
    size_t get_size() const {
        return _size;
    }
private:
    IteratorRange<It> _range;
    size_t _size;
};

template<typename It>
class Paginator {
public:
    using PageIt = typename vector<Page<It>>::iterator;

    Paginator(IteratorRange<It> range, size_t page_sz)
        : _range(range), _page_sz(page_sz) {
        if (_page_sz == 0)
            throw runtime_error("Zero page size");

        size_t range_size = _range.get_size();
        size_t pages_cnt = range_size / page_sz;
        if (range_size % page_sz)
            pages_cnt += 1;

        _pages.resize(pages_cnt);

        size_t offset = 0;
        auto start_it = _range.begin();
        auto end_it = start_it;

        for (auto &page: _pages) {
            size_t length = min(_range.get_size() - offset, _page_sz);
            end_it = next(start_it, length);
            page = {{start_it, end_it}, length};
            offset += length;
            start_it = end_it;
        }
    }

    size_t get_page_cnt() const {
        return _pages.size();
    }

    size_t get_page(size_t id) const {
        return _pages.at(id);
    }

    PageIt begin() {
        return _pages.begin();
    }

    PageIt end() {
        return _pages.end();
    }

private:
    IteratorRange<It> _range;
    size_t _page_sz;
    vector<Page<It>> _pages;
};
