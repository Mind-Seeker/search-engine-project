#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

#include <thread>

void LowercasingString(std::string& text)
{
    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] >= 'A' && text[i] <= 'Z')
        {
            text[i] += 32;
        }
    }
}


int main()
{
    InvertedIndex indexing;

    auto docsTexts = ConverterJSON::GetTextDocuments();

    std::vector<std::thread> textThreads;

    for (auto & docText : docsTexts)
    {
        textThreads.emplace_back(LowercasingString,std::ref(docText));
    }

    for (auto & textThread : textThreads) {
        textThread.join();
    }

    indexing.UpdateDocumentBase(docsTexts);

    SearchServer searchServer(indexing);

    auto requests = ConverterJSON::GetRequests();

    auto searchResults = searchServer.Search(requests);

    ConverterJSON::PutAnswers(searchResults);
}

