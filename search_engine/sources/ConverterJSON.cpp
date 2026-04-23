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
        try
        {
            configFile >> config;
            configFile.close();
        }
        catch (...)
        {
            throw std::runtime_error("Config file is either empty or contains mistakes in formatting for JSON");
        }
    }

    if (config["config"]["version"] != "0.7")
    {
        throw std::runtime_error("config.json has incorrect file version");
    }

    return config;
}

nlohmann::json getRequestsFile()
{
    nlohmann::json requests;
    if (std::ifstream requestsFile("requests.json"); requestsFile.is_open())
    {
        try
        {
            requestsFile >> requests;
            requestsFile.close();

        }
        catch (...)
        {
            throw std::runtime_error("Requests file is either empty or contains mistakes in formatting for JSON");
        }
    }

    if (requests["requests"].size() > 1000)
    {
        throw std::runtime_error("Requests file contains more than 1000 requests");
    }

    return requests;
}

ConverterJSON::ConverterJSON(){}

std::vector<std::string> ConverterJSON::GetTextDocuments()
{
    try
    {
        if (nlohmann::json config = getConfigFile(); config != nullptr)
        {
            std::vector<std::string> strFromFiles;
            std::string strFromFile;
            std::string paragraphFromFile;
            for (auto& file : config["files"])
            {
                try {
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
                    else
                    {
                        throw std::runtime_error("couldn't find " + file.get<std::string>());
                    }
                }
                catch (std::exception& e)
                {
                    std::cerr << e.what() << std::endl;
                }
            }
            if (strFromFiles.empty())
            {
                throw std::runtime_error("config file doesn't include files to search through");
            }
            return strFromFiles;
        }
        throw std::runtime_error("Could not find config file");
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::terminate();
    }
}

int ConverterJSON::GetResponsesLimit()
{
    try
    {
        if (nlohmann::json config = getConfigFile(); config != nullptr)
        {
            if (config["config"]["max_responses"] != nullptr)
            {
                return config["config"]["max_responses"].get<int>();
            }

            throw std::runtime_error("Could not find max_responses");

        }
        throw std::runtime_error("Could not find config file");
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::terminate();
    }
}

std::vector<std::string> ConverterJSON::GetRequests()
{
    try
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
        throw std::runtime_error("Could not find requests file");
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::terminate();
    }

}

void ConverterJSON::PutAnswers(std::vector<std::vector<RelativeIndex>>& answers)
{
    std::ofstream answersFile("answers.json");
    nlohmann::json answersJson;

    std::string requestId;

    for (int i = 0; i < answers.size(); i++)
    {
        if (i < 9)
        {
            requestId = "request00" + std::to_string(i+1);
        }
        else if (i > 98)
        {
            requestId = "request" + std::to_string(i+1);
        }
        else
        {
            requestId = "request0" + std::to_string(i+1);
        }
        if (!answers[i].empty())
        {
            answersJson["answers"][requestId]["result"] = "true";
            for (int j = 0; j < answers[i].size(); j++)
            {
                answersJson["answers"][requestId]["relevance"] += {{"doc_id",answers[i][j].doc_id},{"rank",answers[i][j].rank}};

            }
        }
        else
        {
            answersJson["answers"][requestId]["result"] = "false";
        }


    }
    answersFile << std::setw(4) << answersJson;
}