#include <iostream>

#include "Writer.h"

using namespace std;

Writer::Writer(std::string &fileName) : fileName_(fileName)
{}

void Writer::openBuffer()
{
    outfile_.open(fileName_);
}

void Writer::closeBuffer()
{
    outfile_.close();
}

int Writer::getReceiveByte() {
    return received_;
}

void Writer::outputToFile(int workerID, int chunkSize, int offset, void *dataPtr, size_t size, size_t nmemb) {

    std::lock_guard<std::mutex> guard(mutex_);
    std::cout << "The #" << workerID << " thread is writing " << size * nmemb <<" byte to buffer" << std::endl;

    long position = workerID  * chunkSize + offset;
    received_ += size * nmemb;
    outfile_.seekp(position);
    outfile_.write ((char * )dataPtr, size * nmemb);
}

