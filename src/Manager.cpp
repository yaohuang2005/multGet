#include "Manager.h"

Manager::Manager(Writer &writer, std::vector<std::thread> &threads) :
        writer_(writer), threads_(threads)
{}

Manager::~Manager()
{}

Writer &Manager::getWriter_()
{
    return writer_;
}
std::vector<std::thread> &Manager::getThreads_()
{
    return threads_;
}

