//
// Created by Mind Seeker on 3/7/2026.
//

#ifndef SEARCH_ENGINE_INVERTEDINDEX_H
#define SEARCH_ENGINE_INVERTEDINDEX_H

#include <string>
#include <vector>
#include <map>

struct Entry
{
    size_t doc_id, count;
    bool operator==(const Entry& other)const
    {
        return(doc_id==other.doc_id&&
        count==other.count);
    }
};

class InvertedIndex
{
public:
    InvertedIndex();

    void UpdateDocumentBase(const std::vector<std::string>& input_docs);

    std::vector<Entry> GetWordCount (const std::string& word);

private:
    std::vector<std::string> docs;

    std::map<std::string, std::vector<Entry>> freqDictionary;
};

#endif //SEARCH_ENGINE_INVERTEDINDEX_H