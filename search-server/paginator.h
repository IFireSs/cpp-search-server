#pragma once
#include <ostream>
#include <vector>

template<typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end) : begin_(begin), end_(end) {
    }
    Iterator begin() const {
        return begin_;
    }
    Iterator end() const {
        return end_;
    }
    size_t size() const {
        return end_ - begin_;
    }

private:
    Iterator begin_;
    Iterator end_;
};


template <typename Iterator>
class Paginator {
public:
    Paginator(Iterator begin, Iterator end, size_t page_size) {
        while(begin!= end){
            pages_.push_back({begin, next(begin, std::min(page_size, static_cast<size_t>(end - begin)))});
            begin = pages_.back().end();
        }
    }
    auto begin() const {
        return pages_.begin();
    }
    auto end() const {
        return pages_.end();
    }

private:
    std::vector<IteratorRange<Iterator>> pages_;
};

template <typename Iterator>
std::ostream& operator<<(std::ostream& output, const IteratorRange<Iterator> range) {
    for (auto it = range.begin(); it != range.end(); ++it) {
        output << *it;
    }
    return output;
}

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}