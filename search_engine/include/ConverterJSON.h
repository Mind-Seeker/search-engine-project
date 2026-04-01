//
// Created by Mind Seeker on 2/18/2026.
//

#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H

#include "SearchServer.h"

#include <string>
#include <vector>

/**
*Класс для  работы с JSON-файлами
*/
class ConverterJSON
{
public:
    ConverterJSON();
    /**
    * Метод получения содержимого файлов
    * @return Возвращает список с содержимым  файлов перечисленных
    * в config.json
    */
    static std::vector<std::string> GetTextDocuments();
    /**
    * Метод считывает поле max_responses для определения предельного
    * количества ответов на один запрос
    * @return
    */
    static int GetResponsesLimit();
    /**
    * Метод получения запросов из файла requests.json
    * @return возвращает список запросов из файла requests.json
    */
    static std::vector<std::string> GetRequests();
    /**
    * Положить в файл answers.json результаты поисковых запросов
    */
    static void putAnswers(std::vector<std::vector<RelativeIndex>>answers);
};

#endif //SEARCH_ENGINE_CONVERTERJSON_H
