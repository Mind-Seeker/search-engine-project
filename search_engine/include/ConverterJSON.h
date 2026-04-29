//
// Created by Mind Seeker on 2/18/2026.
//

#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H

#include "SearchServer.h"

#include <string>
#include <vector>

class ConverterJSON
{
public:
    ConverterJSON();

    static std::string GetName();

    static std::vector<std::string> GetTextDocuments();

    static int GetResponsesLimit();

    static std::vector<std::string> GetRequests();

    static void PutAnswers(std::vector<std::vector<RelativeIndex>>&answers);
};

#endif //SEARCH_ENGINE_CONVERTERJSON_H
