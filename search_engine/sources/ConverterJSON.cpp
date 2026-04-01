//
// Created by Mind Seeker on 2/18/2026.
//

#include "ConverterJSON.h"
#include "nlohmann/json.hpp"

#include <iostream>
#include <fstream>

nlohmann::json getConfigFile()
{
    nlohmann::json config;
    if (std::ifstream configFile("config.json"); configFile.is_open())
    {
        configFile >> config;
        configFile.close();
    }
    return config;
}

nlohmann::json getRequestsFile()
{
    nlohmann::json requests;
    if (std::ifstream requestsFile("requests.json"); requestsFile.is_open())
    {
        requestsFile >> requests;
        requestsFile.close();
    }
    return requests;
}

ConverterJSON::ConverterJSON(){}

std::vector<std::string> ConverterJSON::GetTextDocuments()
{
    if (nlohmann::json config = getConfigFile(); config != nullptr)
    {
        std::vector<std::string> strFromFiles;
        std::string strFromFile;
        std::string paragraphFromFile;
        for (auto& file : config["files"])
        {
            if (std::ifstream textFile(file.get<std::string>()); textFile.is_open())
            {
                while(!textFile.eof())
                {
                    std::getline(textFile, paragraphFromFile);
                    strFromFile += paragraphFromFile += "\n";
                }
                strFromFiles.push_back(strFromFile);
                strFromFile = "";
                paragraphFromFile = "";
            }
        }
        return strFromFiles;
    }
}

int ConverterJSON::GetResponsesLimit()
{
    if (nlohmann::json config = getConfigFile(); config != nullptr)
    {
        return config["config"]["max_responses"].get<int>();
    }
}

std::vector<std::string> ConverterJSON::GetRequests()
{
    if (nlohmann::json config = getRequestsFile(); config != nullptr)
    {
        std::vector<std::string> requests;
        for (auto& request : config["requests"])
        {
            requests.emplace_back(request);
        }
        return requests;
    }
}

void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>> answers)
{
    std::ofstream answersFile("answers.json");
    nlohmann::json answersJson;

    for (int i = 0; i < answers.size(); i++)
    {
        answersJson["answers"]["request " + std::to_string(i)]["result"];
        if (!answers[i].empty())
        {
            answersJson["answers"]["request " + std::to_string(i)]["result"] = "true";
            for (int j = 0; j < answers[i].size(); j++)
            {
                answersJson["answers"]["request " + std::to_string(i)]["relevance"] += {{"doc_id",answers[i][j].doc_id},{"rank",answers[i][j].rank}};

            }
        }
        else
        {
            answersJson["answers"]["request " + std::to_string(i)]["result"] = "false";
        }


    }

    std::cout << "answers:" << std::endl;
    for (int i = 0; i < answers.size(); i++)
    {
        std::cout << "request " + std::to_string(i) << std::endl;
        if (!answers[i].empty())
        {
            std::cout << "true" << std::endl;
            for (int j = 0; j < answers[i].size(); j++)
            {

                    std::cout << "doc_id: " << answers[i][j].doc_id;
                    std::cout << ", " << "rank: " << answers[i][j].rank << std::endl;

            }
        }
        else
        {
            std::cout << "false" << std::endl;
        }



    }

    answersFile << std::setw(4) << answersJson;
}