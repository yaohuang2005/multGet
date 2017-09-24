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

#ifndef MULTIGET_CONFIGURATION_H
#define MULTIGET_CONFIGURATION_H

#include <string>

class Configuration {
public:
    Configuration();
    virtual ~Configuration();

    std::string &getUrl();
    std::string &getFileName();
    int getSize();

    void setUrl(std::string &url);
    void setFileName(std::string &fileName);
    void setSize(int size);

private:
    std::string url_;
    std::string fileName_;
    int size_;

};




#endif //MULTIGET_CONFIGURATION_H
