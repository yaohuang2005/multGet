
#ifndef MULTIGET_FILECHUNKDOWNLOADER_H
#define MULTIGET_FILECHUNKDOWNLOADER_H

#include <curl/curl.h>
#include <string>

#include "Writer.h"

class FileChunkDownloader {
public:
    FileChunkDownloader(std::string &url, int workerID, Writer &writer);

    void initBaseRange(int base, int range);
    double getFileSize();

    void execute();
    size_t writeFile(void *ptr, size_t size, size_t nmemb);

private:
    int workerID_;
    int base_;
    int offset_;
    int range_;
    Writer &writer_;
    std::string &url_;
};



#endif //MULTIGET_FILECHUNKDOWNLOADER_H
