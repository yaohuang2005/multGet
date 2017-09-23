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
