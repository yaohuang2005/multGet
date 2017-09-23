#ifndef MULTIGET_WRITER_H
#define MULTIGET_WRITER_H

#include <thread>
#include <mutex>
#include <string>
#include <fstream>

// a singleton writer
class Writer {
public:
    static Writer& getInstance(std::string &fileName) {
        static Writer s(fileName);
        return s;
    }

    void outputToFile(int workerID, int chunkSize, int offset, void *dataPtr, size_t size, size_t nmemb);
    void openBuffer();
    void closeBuffer();
    int getReceiveByte();

private:
    Writer(std::string &fileName);
    Writer(const Writer&) = delete;
    Writer& operator=(const Writer&) = delete;

    std::mutex mutex_;
    std::string &fileName_;
    std::ofstream outfile_;
    int received_;

};

#endif //MULTIGET_WRITER_H
