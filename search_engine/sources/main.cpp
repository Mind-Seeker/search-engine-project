#include "ConverterJSON.h"
#include "InvertedIndex.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "SearchServer.h"

int main()
{
    int index = 0;
    // for (auto& text: ConverterJSON::GetTextDocuments())
    // {
    //     std::cout << index;
    //     std::cout << "//" << "\n" << text << "\n" << "//" << std::endl;
    //     index++;
    // }
    //
    // std::cout << "//" << ConverterJSON::GetResponsesLimit() << "//" << std::endl;
    //
    // for (auto& request: ConverterJSON::GetRequests())
    // {
    //     std::cout << "//" << request << "//" << std::endl;
    // }
    //
    // index = 0;

    ///////////////////


    // InvertedIndex indexing;
    //
    // indexing.UpdateDocumentBase(ConverterJSON::GetTextDocuments());
    //
    // for (auto& text: indexing.docs)
    // {
    //     std::cout << "Text from doc " << index;
    //     std::cout << "//" << "\n" << text << "\n" << "//" << std::endl;
    //     index++;
    // }
    //
    // index = 0;
    // for (std::map<std::string, std::vector<Entry>>::iterator it = indexing.freqDictionary.begin();
    //     it != indexing.freqDictionary.end(); ++it)
    // {
    //     std::cout << " word number " << index << " ";
    //     std::cout << "***" << "\n" << it->first << std::endl;
    //     for (auto& entry: it->second)
    //     {
    //         std::cout << "doc id: " << entry.doc_id << std::endl;
    //         std::cout << "word count: " << entry.count << std::endl;
    //     }
    //     std::cout << "\n" << "***";
    //     index++;
    // }

    InvertedIndex indexing;

    indexing.UpdateDocumentBase(ConverterJSON::GetTextDocuments());

    SearchServer searchServer(indexing);

    ConverterJSON::putAnswers(searchServer.Search(ConverterJSON::GetRequests()));

}

