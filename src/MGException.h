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

#ifndef MULTIGET_MGEXCEPTION_H
#define MULTIGET_MGEXCEPTION_H
class MGException : public std::exception
{
public:
    MGException(std::string msg):m_message(msg)
    {}

    const char *what() const throw ()
    {
        return m_message.c_str();
    }

private:
    std::string m_message;
};
#endif //MULTIGET_MGEXCEPTION_H
