
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
  //  FileChunkDownloader(const FileChunkDownloader&) = delete;
  //  FileChunkDownloader& operator=(const FileChunkDownloader&) = delete;

    int workerID_;
    int base_;
    int offset_;
    int range_;
    Writer &writer_;
    std::string &url_;
};



#endif //MULTIGET_FILECHUNKDOWNLOADER_H


/*
    int curlWriteData(char *data, size_t size, size_t nmemb, void * j);
    int curlDownloadInfo(void * j,
                     curl_off_t dltotal, curl_off_t dlnow,
                     curl_off_t ultotal, curl_off_t ulnow);
*/
