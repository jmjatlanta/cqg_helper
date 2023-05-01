#include <string>

class CQGSession
{
    public:
    CQGSession();
    bool connect();

    public:
    std::string host_name;
    std::string user_name;
    std::string password;
};
