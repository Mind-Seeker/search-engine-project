//
// Created by Mind Seeker on 3/17/2026.
//

#include "SearchServer.h"
#include "FunctionsForUniqueWords.h"

#include <map>
#include <algorithm>

#include "ConverterJSON.h"
#include "nlohmann/detail/string_utils.hpp"

SearchServer::SearchServer (InvertedIndex& idx) : _index(idx) {}

std::vector<std::vector<RelativeIndex>> SearchServer::Search ( std::vector<std::string>& queries_input)
{

    std::vector<std::vector<RelativeIndex>> result;

    std::vector<std::vector<std::string>> uniqueWordsForEachQuery;
    std::vector<std::string> wordsFromOneQuery;
    for (int i = 0; i < queries_input.size(); i++)
    {
        GetUniqueWordsFromString(wordsFromOneQuery, queries_input[i],i,true);
        uniqueWordsForEachQuery.emplace_back(wordsFromOneQuery);
        wordsFromOneQuery.clear();
    }

    std::multimap<std::vector<size_t>,std::string> sortedFoundWords;
    std::vector<size_t> counts;

    for (const auto& query : uniqueWordsForEachQuery)
    {
        for (const auto& word : query)
        {
            std::vector<Entry> entryVec = _index.GetWordCount(word);
            if (!entryVec.empty())
            {
                for (const auto& entry : entryVec)
                {
                    counts.emplace_back(entry.count);
                }
            }
            sortedFoundWords.insert(std::make_pair(counts, word));
            counts.clear();
        }

        size_t maxAbsoluteRelevance = 0;
        std::map<size_t,float> docsIDAndAbsoluteRelevance;
        for (const auto& word : sortedFoundWords)
        {
            for (const auto& entry : _index.GetWordCount(word.second))
            {
                if (docsIDAndAbsoluteRelevance.contains(entry.doc_id))
                {
                    docsIDAndAbsoluteRelevance.find(entry.doc_id)->second += entry.count;
                    if (docsIDAndAbsoluteRelevance.find(entry.doc_id)->second > maxAbsoluteRelevance)
                    {
                        maxAbsoluteRelevance = docsIDAndAbsoluteRelevance.find(entry.doc_id)->second;
                    }
                }
                else
                {
                    docsIDAndAbsoluteRelevance.insert(std::make_pair(entry.doc_id, entry.count));
                    if (docsIDAndAbsoluteRelevance.find(entry.doc_id)->second > maxAbsoluteRelevance)
                    {
                        maxAbsoluteRelevance = docsIDAndAbsoluteRelevance.find(entry.doc_id)->second;
                    }
                }
            }
        }

        std::vector<std::pair<float, size_t>> sortedRelevanceVec;

        for (auto it = docsIDAndAbsoluteRelevance.begin(); it != docsIDAndAbsoluteRelevance.end(); it++)
        {
            sortedRelevanceVec.emplace_back(it->second/maxAbsoluteRelevance, it->first);
        }

        std::sort (sortedRelevanceVec.begin(), sortedRelevanceVec.end(), std::greater<>());


        for (int i = 0; i < sortedRelevanceVec.size(); i++)
        {
            bool check = true;
            for (int j = 0; j < sortedRelevanceVec.size() - (i + 1); j++)
            {
                if (sortedRelevanceVec[j].first == sortedRelevanceVec[j+1].first
                    && sortedRelevanceVec[j].second > sortedRelevanceVec[j+1].second)
                {
                    check = false;
                    std::swap(sortedRelevanceVec[j], sortedRelevanceVec[j+1]);
                }
            }
            if (check == true)
            {
                break;
            }
        }


        RelativeIndex tmpRelativeIndex(0,0);
        std::vector<RelativeIndex> tmpRelativeIndexVec;

        auto responseLimit = ConverterJSON::GetResponsesLimit();

        for (const auto& rankAndDocID : sortedRelevanceVec)
        {
            tmpRelativeIndex.doc_id = rankAndDocID.second;
            tmpRelativeIndex.rank = rankAndDocID.first;
            tmpRelativeIndexVec.emplace_back(tmpRelativeIndex);
            responseLimit--;
            if (responseLimit == 0)
            {
                break;
            }
        }

        docsIDAndAbsoluteRelevance.clear();
        sortedFoundWords.clear();
        result.emplace_back(tmpRelativeIndexVec);
        tmpRelativeIndexVec.clear();
    }

    return result;
}

