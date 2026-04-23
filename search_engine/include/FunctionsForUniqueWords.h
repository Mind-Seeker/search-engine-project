//
// Created by Mind Seeker on 3/18/2026.
//

#ifndef SEARCH_ENGINE_FUNCTIONSFORUNIQUEWORDS_H
#define SEARCH_ENGINE_FUNCTIONSFORUNIQUEWORDS_H

#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <iostream>

inline std::mutex keyWordsSameWordsCheck;
inline std::mutex keyWordsCounting;
inline std::mutex keyWordsUpdate;

inline void GetUniqueWordsFromVector(std::vector<std::string>& uniqueWords,
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

inline void GetUniqueWordsFromString(std::vector<std::string>& keyWords, const std::string& input_doc,
                                    int index, const bool isARequest)
{
    ++index;

    size_t wordsCount = 0;

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
        else if (!foundWord.empty() && i+1 != input_doc.length() - 1
        && input_doc[i] == '\''
        && (input_doc[i+1] == 'd' || input_doc[i+1] == 'D'
        || input_doc[i+1] == 's' || input_doc[i+1] == 'S'
        || input_doc[i+1] == 't' || input_doc[i+1] == 'T'))
        {
            keyWordsCounting.lock();
            foundWord += input_doc[i];
            foundWord += input_doc[i+1];
            ++i;
            keyWordsCounting.unlock();
            keyWordsUpdate.lock();
            if (std::find(keyWords.begin(), keyWords.end(), foundWord.c_str()) == keyWords.end())
            {
                keyWords.emplace_back(foundWord);
            }
            ++wordsCount;
            foundWord = {};
            keyWordsUpdate.unlock();
        }
        else if (!foundWord.empty() && i+2 != input_doc.length() - 1
        && input_doc[i] == '\''
        && (input_doc[i+1] == 'l' && (input_doc[i+2] == 'l' || input_doc[i+2] == 'L')
        || (input_doc[i+1] == 'L' && (input_doc[i+2] == 'l' || input_doc[i+2] == 'L'))))
        {
            keyWordsCounting.lock();
            foundWord += input_doc[i];
            foundWord += input_doc[i+1];
            foundWord += input_doc[i+2];
            i += 2;
            keyWordsCounting.unlock();
            keyWordsUpdate.lock();
            if (std::find(keyWords.begin(), keyWords.end(), foundWord.c_str()) == keyWords.end())
            {
                keyWords.emplace_back(foundWord);
            }
            ++wordsCount;
            foundWord = {};
            keyWordsUpdate.unlock();
        }

        if ((((input_doc[i] < 'A' || input_doc[i] > 'Z')
            && (input_doc[i] < 'a' || input_doc[i] > 'z'))
            || i == input_doc.length() - 1) && !foundWord.empty())
        {
            keyWordsUpdate.lock();
            if (std::find(keyWords.begin(), keyWords.end(), foundWord.c_str()) == keyWords.end())
            {
                keyWords.emplace_back(foundWord);
            }
            ++wordsCount;
            foundWord = {};
            keyWordsUpdate.unlock();
        }
        if (isARequest && foundWord.size() > 100)
        {
            throw std::runtime_error("The request number " + std::to_string(index) + " contains a word that is too large (more than 100 symbols)");
        }
        if (!isARequest && foundWord.size() > 100)
        {
            throw std::runtime_error("The file number " + std::to_string(index) + " contains a word that is too large (more than 100 symbols)");
        }
        if (isARequest && wordsCount > 10)
        {
            throw std::runtime_error("Too many words in request number " + std::to_string(index) + " (more than 10)");
        }
        if (!isARequest && wordsCount > 1000)
        {
            throw std::runtime_error("Too many words in file number " + std::to_string(index) + " (more than 1000)");
        }
    }
    if (isARequest && wordsCount == 0)
    {
        std::cerr << "Request number " + std::to_string(index) + " doesn't contain words";
    }
    if (!isARequest && wordsCount == 0)
    {
        std::cerr << "File number " + std::to_string(index) + " doesn't contain words";
    }
}

#endif //SEARCH_ENGINE_FUNCTIONSFORUNIQUEWORDS_H