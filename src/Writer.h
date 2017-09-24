/*  multiGet
 *  ===============
 *  Copyright (C) 2017 yaohuang2005@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

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
