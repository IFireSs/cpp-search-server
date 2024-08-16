#pragma once
#include <deque>
#include "search_server.h"

const int MIN_IN_DAY = 1440;

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server);

    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate);
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string& raw_query);

    int GetNoResultRequests() const;
private:
    struct QueryResult {
        bool empty_result;
    };
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = MIN_IN_DAY;
    const SearchServer& search_server_;
    int time_ = 0;

};

template <typename DocumentPredicate>
std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
    auto result = search_server_.FindTopDocuments(raw_query, document_predicate);
    time_++;
    if (time_ > min_in_day_) {
        requests_.pop_front();
    }
    requests_.push_back(QueryResult(result.empty()));
    return result;
}