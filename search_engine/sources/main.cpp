#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

#include <thread>




int main()
{
    InvertedIndex indexing;

    auto docsTexts = ConverterJSON::GetTextDocuments();



    indexing.UpdateDocumentBase(docsTexts);

    SearchServer searchServer(indexing);

    auto requests = ConverterJSON::GetRequests();

    auto searchResults = searchServer.Search(requests);

    ConverterJSON::PutAnswers(searchResults);
}

