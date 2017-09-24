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
    if (outfile_.is_open()) {
        outfile_.close();
    }
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

