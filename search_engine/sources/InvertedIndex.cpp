//
// Created by Mind Seeker on 3/7/2026.
//

#include "InvertedIndex.h"
#include "ConverterJSON.h"
#include "FunctionsForUniqueWords.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

std::mutex entryUpdate;

std::vector<Entry> indexingOneWord(const std::string& keyWord,
                const std::vector<std::string>& input_docs)
{
    std::vector<Entry> oneWordEntries;
    Entry entry(0,0);
    size_t position = 0;
    for (int i = 0; i < input_docs.size(); i++)
    {
        entryUpdate.lock();
        entry.doc_id = i;
        while (position < input_docs[i].length())
        {
            position = input_docs[i].find(keyWord, position);
            if(position != std::string::npos)
            {
                if (((input_docs[i][position-1] < 'A' || input_docs[i][position-1] > 'Z')
                    && (input_docs[i][position-1] < 'a' || input_docs[i][position-1] > 'z'))
                    && ((input_docs[i][position+keyWord.length()] < 'A' || input_docs[i][position+keyWord.length()] > 'Z')
                    && (input_docs[i][position+keyWord.length()] < 'a' || input_docs[i][position+keyWord.length()] > 'z')))
                {
                    entry.count++;
                }
                position += keyWord.length();
            }
        }
        oneWordEntries.emplace_back(entry);
        entry.count = 0;
        entryUpdate.unlock();
        position = 0;
        //
    }
    return oneWordEntries;
}


InvertedIndex::InvertedIndex()
{
}

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& input_docs)
{
    docs = input_docs;
    std::vector<std::thread> threadsKeyWordsFromOneDoc;
    std::vector<std::thread> threadsAllKeyWords;

    std::vector<std::string> foundKeyWords;
    std::vector<std::string> keyWordsFromOneDoc;
    for (const auto & input_doc : input_docs)
    {
        threadsKeyWordsFromOneDoc.emplace_back(uniqueWords,std::ref(keyWordsFromOneDoc),input_doc);
    }

    for (int i = 0; i < input_docs.size(); i++)
    {
        threadsKeyWordsFromOneDoc[i].join();
        threadsAllKeyWords.emplace_back(getUniqueWords,std::ref(foundKeyWords), std::ref(keyWordsFromOneDoc));
    }

    for (auto& thread : threadsAllKeyWords)
    {
        thread.join();
    }

    for (const auto& KeyWord : foundKeyWords)
    {
        freqDictionary.insert(std::make_pair(KeyWord, indexingOneWord(KeyWord, docs)));
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word)
{
    std::vector<Entry> oneWordEntries;
    auto wordEntries = freqDictionary.find(word);
    if (wordEntries != freqDictionary.end())
    {
        for (const auto& entry : wordEntries->second)
        {
            oneWordEntries.emplace_back(entry);
        }
    }
    return oneWordEntries;
}

