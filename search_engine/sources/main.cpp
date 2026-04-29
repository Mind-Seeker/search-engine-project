#include <iostream>
#include <ostream>

#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main()
{
    InvertedIndex indexing;

    auto docsTexts = ConverterJSON::GetTextDocuments();

    std::cout << ConverterJSON::GetName() << std::endl;

    indexing.UpdateDocumentBase(docsTexts);

    SearchServer searchServer(indexing);

    auto requests = ConverterJSON::GetRequests();

    auto searchResults = searchServer.Search(requests);

    ConverterJSON::PutAnswers(searchResults);
}

