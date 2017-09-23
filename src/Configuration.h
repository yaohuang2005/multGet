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
