#include "Configuration.h"

Configuration::Configuration():url_(""), fileName_(""), size_(0)
{}

Configuration::~Configuration()
{}

std::string &Configuration::getUrl()
{
    return url_;
}

std::string &Configuration::getFileName()
{
    return fileName_;
}

int Configuration::getSize()
{
    return size_;
}

void Configuration::setUrl(std::string &url)
{
    url_ = url;
}

void Configuration::setFileName(std::string &fileName)
{
    fileName_ = fileName;
}

void Configuration::setSize(int size)
{
    size_ = size;
}


