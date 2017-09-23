#include <iostream>

#include "FileChunkDownloader.h"
#include "MGException.h"

FileChunkDownloader::FileChunkDownloader(std::string &url, int workerID, Writer &writer) :
        url_(url),
        workerID_(workerID),
        writer_(writer),
        base_(0),
        offset_(0),
        range_(0)
{}

void FileChunkDownloader::initBaseRange(int base, int range)
{
    base_ = base;
    range_ = range;
}

size_t FileChunkDownloader::writeFile(void *dataPtr, size_t size, size_t nmemb)
{
    writer_.outputToFile(workerID_, range_, offset_, dataPtr, size, nmemb);
    offset_ += size * nmemb;  // accumulated position for next received data

    return size * nmemb;
}

/*
 * Register a static function for libcurl(c library) callback setting,
 * then get instance for the data.
 * */
static size_t writeData(void *data, size_t size, size_t nmemb, void* pInstance)
{
    FileChunkDownloader *pInstance1 = static_cast<FileChunkDownloader *>(pInstance);
    return pInstance1->writeFile(data, size, nmemb);
}


void FileChunkDownloader::execute()
{
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    std::string start = std::to_string(base_);
    std::string end   = std::to_string(base_ + range_ - 1);   // 0 - 99, 100-199

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(curl, CURLOPT_RANGE, (start + "-" + end).c_str());
        res = curl_easy_perform(curl);
        if (res) {
            throw MGException("Cannot get file chunk");
        }
        curl_easy_cleanup(curl);
    }
}

double FileChunkDownloader::getFileSize()
{
    double cl;
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 500);
        res = curl_easy_perform(curl);
        if(!res) {
            curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &cl);
        } else {
            throw MGException("Cannot get file size");
        }
    }
    curl_easy_cleanup(curl);
    return cl;
}




/*
int FileChunkDownloader::curlWriteData(char *data, size_t size, size_t nmemb, void * j)
{
    if (j == nullptr)
    {
        return 0;
    }

    FileChunkDownloader * job = static_cast<FileChunkDownloader *>(j);
    char outfilename[FILENAME_MAX] = "384MB.jar_";
    strcat(outfilename, name_.c_str());
    FILE *fp = fopen(outfilename,"wb");

   // job->data.insert(job->data.end(), data, data + size * nmemb);
    size_t written = fwrite(data, size, nmemb, fp);
    fclose(fp);

    return size * nmemb;
}
*/
/*
static int xferinfo(void *p,
                    curl_off_t dltotal, curl_off_t dlnow,
                    curl_off_t ultotal, curl_off_t ulnow)
{
    FileChunkDownloader *myp = static_cast<FileChunkDownloader *>(p);
    return 0;
}
*/
