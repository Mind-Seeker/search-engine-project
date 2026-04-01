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
    // Данный оператор необходим для проведения тестовых сценариев
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
    // Обновить или заполнить базу документов, по которой
    // будем совершать поиск
    // @param texts_input содержимое документов

    std::vector<Entry> GetWordCount (const std::string& word);
    // Метод определяет количество вхождений слова word в
    // загруженной базе документов
    // @param word слово, частоту вхождений которого необходимо определить
    // @return возвращает подготовленный список с частотой слов

private:
    std::vector<std::string> docs;
    // коллекция для хранения текстов документов,
    // в которой номер элемента в векторе определяет doc_id для формирования
    // результата запроса

    std::map<std::string, std::vector<Entry>> freqDictionary;
    // или частотный словарь —
    // это коллекция для хранения частоты слов, встречаемых в тексте. Entry
    // представляет собой структуру
};


#endif //SEARCH_ENGINE_INVERTEDINDEX_H