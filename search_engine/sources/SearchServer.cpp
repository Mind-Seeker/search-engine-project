//
// Created by Mind Seeker on 3/17/2026.
//

#include "SearchServer.h"
#include "FunctionsForUniqueWords.h"

#include <map>
#include <algorithm>

SearchServer::SearchServer (InvertedIndex& idx) : _index(idx) {}

std::vector<std::vector<RelativeIndex>> SearchServer::Search (const std::vector<std::string>& queries_input)
{

    std::vector<std::vector<RelativeIndex>> result;
    std::vector<RelativeIndex> tmpRelativeIndexVec;

    std::vector<std::vector<std::string>> uniqueWordsFromQueries;
    std::vector<std::string> wordsFromOneQuery;
    for (const auto& query : queries_input)
    {
        uniqueWords(wordsFromOneQuery, query);
        uniqueWordsFromQueries.emplace_back(wordsFromOneQuery);
        wordsFromOneQuery.clear();
    }

    std::map<std::vector<size_t>,std::string> sortedFoundWords;
    std::vector<size_t> counts;
    for (const auto& query : uniqueWordsFromQueries)
    {
        bool found = false;
        for (const auto& word : query)
        {
            if (!_index.GetWordCount(word).empty())
            {
                found = true;
                for (const auto& entry : _index.GetWordCount(word))
                {
                    counts.push_back(entry.count);
                }
            }
            sortedFoundWords.insert(std::make_pair(counts, word));
            counts.clear();
        }
        if (found)
        {
            std::vector<std::pair<float, size_t>> sortedRelevanceVec;
            float maxAbsoluteRelevance = 0;
            float absoluteRelevance = 0;
            const auto tmpIt = sortedFoundWords.begin();
            const size_t docsId = tmpIt->first.size();
            for (int i = 0; i < docsId; i++)
            {
                for (auto it = sortedFoundWords.begin(); it != sortedFoundWords.end(); it++)
                {
                    absoluteRelevance += it->first[i];
                }

                if (absoluteRelevance > maxAbsoluteRelevance)
                {
                    maxAbsoluteRelevance = absoluteRelevance;
                }
                sortedRelevanceVec.emplace_back(absoluteRelevance,i);
                absoluteRelevance = 0;
            }

            std::sort (sortedRelevanceVec.begin(), sortedRelevanceVec.end(), std::greater<>());

            for (auto it = sortedRelevanceVec.begin(); it != sortedRelevanceVec.end(); it++)
            {
                RelativeIndex tmpRelativeIndex(0,0);
                tmpRelativeIndex.doc_id = it->second;
                tmpRelativeIndex.rank = it->first / maxAbsoluteRelevance;
                tmpRelativeIndexVec.emplace_back(tmpRelativeIndex);
            }

        }
        sortedFoundWords.clear();
        result.emplace_back(tmpRelativeIndexVec);
        tmpRelativeIndexVec.clear();
    }
    return result;
}

