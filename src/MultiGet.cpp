#include <vector>
#include <iostream>
#include <getopt.h>

#include "FileChunkDownloader.h"
#include "MGException.h"
#include "Manager.h"
#include "Configuration.h"

static const int ONE_KB = 1024;
static const int ONE_MB = 1024 * ONE_KB;

// Global config
Configuration config;

// Global manager variable, used for shutdown hook
Manager *managerPtr;


// -u http://5d9a03a0.bwtest-aws.pravala.com/384MB.jar -o receivedFile
void printHelp(int rc)
{
    std::cout <<
              "-u --url <url>:         Set url to download\n"
              "-o --output <fileName>: File to write to, default is receivedFile\n"
              "-s --size <s>:          Set maximum file to get"
              "-s --help:              Print this help\n";
    exit(rc);
}

void processArgs(int argc, char** argv, Configuration &config)
{
    std::string url = "";
    std::string fileName = "receivedFile";
    long size = 4 * ONE_MB;

    const char* const short_opts = "u:o:s:h";
    const option long_opts[] = {
            {"url",       1, nullptr, 'u'},  // required
            {"output",    0, nullptr, 'o'},  // optional
            {"size",      0, nullptr, 's'},  // optional
            {"help",      0, nullptr, 'h'},
            {nullptr,     0, nullptr, 0}
    };

    while (true)
    {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if (-1 == opt)
            break;

        switch (opt)
        {
            case 'u':
                url = std::string(optarg);
                std::cout << "URL set to: " << url << std::endl;
                break;
            case 'o':
                fileName = std::string(optarg);
                std::cout << "Output file set to: " << fileName << std::endl;
                break;
            case 's':
                size = std::atoi(optarg);
                std::cout << "Want to file size set to: " << size << std::endl;
                break;
            case 'h':
                printHelp(EXIT_SUCCESS);
                break;
            case '?': // Unrecognized option
            default:
                printHelp(EXIT_FAILURE);
                break;
        }
    }

    if (url.length() == 0) {
        printHelp(EXIT_FAILURE);
    }

    if (size == 0) {
        printHelp(EXIT_FAILURE);
    }

    config.setUrl(url);
    config.setFileName(fileName);
    config.setSize(size);
}

static void shutdown()
{
    //clean up, join threads
    std::cout << "shutdown" << std::endl;
    managerPtr->getWriter_().closeBuffer();
    for (int i = 0; i < managerPtr->getThreads_().size(); i++){
        managerPtr->getThreads_()[i].join();
    }
}

static void signalHandler(int signal)
{
    write(STDERR_FILENO, "[engine] caught signal\n", 23);
    switch (signal) {
        case SIGINT:
        case SIGQUIT:
        case SIGHUP:
        case SIGTERM:
            shutdown();
            _exit(EXIT_SUCCESS);
        default:
            break;
    }
}

static void signalInit()
{
    signal(SIGINT,  &signalHandler);
    signal(SIGQUIT, &signalHandler);
    signal(SIGHUP,  &signalHandler);
    signal(SIGTERM, &signalHandler);
}

int main(int argc, char *argv[]) {

    processArgs(argc, argv, config);
    std::string& url = config.getUrl();
    std::string& fileName = config.getFileName();
    long size = config.getSize();

    Writer &writer = Writer::getInstance(fileName);

    std::vector<std::unique_ptr<FileChunkDownloader>> workers;
    // one thread is for one chunk downloader
    std::vector<std::thread> threads;
    managerPtr = new Manager(writer, threads);

    /* Signal handler */
    signalInit();

    // for test only
    //url = "http://www.tehhayley.com/blog/2012/partial-http-downloads-with-curl/";
    int workerID = 0;
    auto fileChunkDownloaderHead = std::unique_ptr<FileChunkDownloader>(
            new FileChunkDownloader(url, workerID, writer));

    long fileSize;
    try {
        fileSize = fileChunkDownloaderHead->getFileSize();
    } catch (MGException &ex) {
        std::cout << "cannot connect to get file size, will exit" << std::endl;
        _exit(EXIT_FAILURE);
    }

    std::cout << "The url file size is " << size << std::endl;
    if (size < fileSize) {
        std::cout << "only pull " << size << " byte, even the file has " << fileSize << " byte" << std::endl;
    } else {
        std::cout << "the file has only " << fileSize << " byte, so we can only pull this size" << std::endl;
        size = fileSize;
    }

    if (size > 0) {
        int base = 0;
        if (size > 4 * ONE_KB ) {  // file size > 4KB... or even > 4MB
            int chunkSize = size / 4;
            int remainder = size % 4;

            if (chunkSize > ONE_MB) {  // means the file is > 4 MB
                chunkSize = ONE_MB;    // only pull first 4M content
            }

            fileChunkDownloaderHead->initBaseRange(base, chunkSize);
            workers.push_back(std::move(fileChunkDownloaderHead));

            for (int i = 1; i < 4; i++) {  // already have workder 0
                workerID = i;
                auto fileChunkReadeFollower = std::unique_ptr<FileChunkDownloader>(
                        new FileChunkDownloader(url, workerID, writer));

                base = i * chunkSize ;
                if (i < 3) {
                    fileChunkReadeFollower->initBaseRange(base, chunkSize);
                } else {
                    // last thread pull all the rest bytes, DO NOT LOSE data
                    fileChunkReadeFollower->initBaseRange(base, chunkSize + remainder);
                }
                workers.push_back(std::move(fileChunkReadeFollower));
            }
        } else {  // file size is 1 ~ 4KB,
            // less than 4K, no need multithreads, one thread enough
           fileChunkDownloaderHead->initBaseRange(base, size);
            // will decrease 1 in chunkDownloader
           workers.push_back(std::move(fileChunkDownloaderHead));
        }

    } else {
        std::cout << "file size <=0 , no need to pull " << url << std::endl;
        _exit(EXIT_SUCCESS);
    }

    // before start thread, open the buffer
    writer.openBuffer();

    std::cout << "create thread to pull file chunk" << std::endl;
    try {
        for (int i = 0; i < workers.size(); i++)
        {
          threads.push_back(std::thread(&FileChunkDownloader::execute, std::move(workers[i])));
        }
    } catch(MGException &ex) {
        std::cout << "cannot get file chunk, the multiGet will exit" << std::endl;
        _exit(EXIT_SUCCESS);
    }


    for (int i = 0; i < threads.size(); i++){
        threads[i].join();
    }

    std::cout << std::endl;
    std::cout << "Final report:" << std::endl;
    if (size < fileSize) {
        std::cout << "only pull " << size << " byte, even the file has " << fileSize << " byte" << std::endl;
    } else {
        std::cout << "the file has only " << fileSize << " byte, so we can only pull this size" << std::endl;
    }

    writer.closeBuffer();
    std::cout << "Received url file byte " << writer.getReceiveByte() << std::endl;

    delete managerPtr;

    return 0;
}
