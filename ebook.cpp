#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std::string_literals;

struct Request {
    int type;
    int id;
    int page;
};

class StaticManager {
    
public:
    
    using CurrentPage = int;
    using UserID = int;
    using NumbersOfUsers = int;
    
    void ReadRequests(std::istream& input = std::cin) {
        int requests_count;
        input >> requests_count;
        requests.reserve(requests_count);
        int id;
        int page;
        std::string type;
        for (int i = 0; i < requests_count; ++i) {
            input >> type;
            if (type == "READ"s) {
                input >> id >> page;
                requests.emplace_back(Request{ 0, id, page });
                continue;
            }
            if (type == "CHEER"s) {
                input >> id;
                requests.emplace_back(Request{ 1, id, 0 });
                continue;
            }
        }
    }
    
    void ProcessRequests(std::ostream& output = std::cout) {
        for (const auto& request : requests) {
            if (request.type == 0) {
                ReadUsers(request.id, request.page);
                continue;
            }

            if (request.type == 1) {
                CheerUsers(request.id, output);
                continue;
            }
        }
    }
    
    void ReadUsers(int id, int page) {
        int curr_size = static_cast<int>(cash.size());
        if (curr_size < page + 1) {
            cash.resize(std::max(curr_size * 2, page + 1), 0);
        }
        int current_page = 0;
        if (page_by_users.count(id) > 0) {
            current_page = page_by_users.at(id);
        }
        else if (page > 0) {
            ++total_reading_users_;
        }
        if (current_page >= page) {
            return;
        }
        for (int i = current_page + 1; i <= page; ++i) {
            ++cash[i];
        }
        page_by_users[id] = page;
    }
    
     void CheerUsers(int id, std::ostream& output) {
        if (page_by_users.count(id) == 0) {
            output << 0 << std::endl;
            return;
        }
        int user_page = page_by_users.at(id);
        int users_read_page = cash[user_page];

        if (users_read_page == 1 || total_reading_users_ == 1) {
            output << 1 << std::endl;
            return;
        }

        double result =
            static_cast<double>(total_reading_users_ - users_read_page)
            / (total_reading_users_ - 1);
        output << std::setprecision(6)
            << result << std::endl;

    }
    
    private:
    int total_reading_users_ = 0;
    std::vector<Request> requests;
    std::unordered_map<UserID, CurrentPage> page_by_users;
    std::vector<NumbersOfUsers> cash;
};

int main() {
    
    StaticManager sm;
    sm.ReadRequests();
    sm.ProcessRequests();
    return 0;
}
