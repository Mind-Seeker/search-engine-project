//
// Created by Mind Seeker on 3/18/2026.
//

#ifndef SEARCH_ENGINE_FUNCTIONSFORUNIQUEWORDS_H
#define SEARCH_ENGINE_FUNCTIONSFORUNIQUEWORDS_H

#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <functional>

inline std::mutex keyWordsSameWordsCheck;
inline std::mutex keyWordsCounting;
inline std::mutex keyWordsUpdate;

inline void getUniqueWords(std::vector<std::string>& uniqueWords,
                           const std::vector<std::string>& allWords)
{
    for (const auto & word : allWords)
    {
        keyWordsSameWordsCheck.lock();
        if (std::find(uniqueWords.begin(), uniqueWords.end(), word.c_str()) == uniqueWords.end())
        {

            uniqueWords.emplace_back(word);

        }
        keyWordsSameWordsCheck.unlock();
    }
}

inline void uniqueWords(std::vector<std::string>& keyWords, const std::string& input_doc)
{
    std::string foundWord;

    for (size_t i = 0; i < input_doc.length(); i++)
    {

        if ((input_doc[i] >= 'a' && input_doc[i] <= 'z')
            || (input_doc[i] >= 'A' && input_doc[i] <= 'Z'))
        {
            keyWordsCounting.lock();
            foundWord += input_doc[i];
            keyWordsCounting.unlock();
        }
        if (((input_doc[i] < 'A' || input_doc[i] > 'Z')
            && (input_doc[i] < 'a' || input_doc[i] > 'z'))
            || i == input_doc.length() - 1)
        {
            keyWordsUpdate.lock();
            if (keyWords.empty())
            {
                if (!foundWord.empty())
                {
                    keyWords.emplace_back(foundWord);
                    foundWord = {};
                }
            }
            else if (!foundWord.empty()) {
                if (std::find(keyWords.begin(), keyWords.end(), foundWord.c_str()) == keyWords.end())
                {
                    keyWords.emplace_back(foundWord);
                }
                foundWord = {};
            }
            keyWordsUpdate.unlock();
        }
    }
}

#endif //SEARCH_ENGINE_FUNCTIONSFORUNIQUEWORDS_H