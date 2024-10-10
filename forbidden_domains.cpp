#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;

class Domain {
public:
    // Добавляем точку и переворачиваем домен
    Domain(const std::string& domain) {
        reverse_domain_ = '.' + domain;
        reverse(reverse_domain_.begin(), reverse_domain_.end());
    }
    
    const std::string& Get() const {
        return reverse_domain_;
    }
    
    bool operator==(const Domain& other) const {
        return reverse_domain_ == other.reverse_domain_;
    }

    // Проверяем, является ли данный домен поддоменом другого
    bool IsSubdomain(const Domain& other) const {
        return reverse_domain_.size() >= other.reverse_domain_.size() && 
               reverse_domain_.substr(0, other.reverse_domain_.size()) == other.reverse_domain_;
    }

private:
    std::string reverse_domain_;
};

class DomainChecker {
public:
    DomainChecker(std::vector<Domain> forbidden_domains) {
        sort(forbidden_domains.begin(), forbidden_domains.end(),
             [](const Domain& lhs, const Domain& rhs) {
                 return lhs.Get() < rhs.Get();
             });
        
        forbidden_domains.erase(
            unique(forbidden_domains.begin(), forbidden_domains.end(),
                   [](const Domain& lhs, const Domain& rhs) {
                       return rhs.IsSubdomain(lhs);
                   }),
            forbidden_domains.end()
        );
        
        forbidden_domains_ = move(forbidden_domains);
    }
    
    // возвращающий true, если домен запрещён 
    bool IsForbidden(const Domain& domain) const {
        auto it = upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain,
                              [](const Domain& lhs, const Domain& rhs) {
                                  return lhs.Get() < rhs.Get();
                              });
        
        if (it == forbidden_domains_.begin()) {
            return false;
        }
        return domain.IsSubdomain(*prev(it));
    }

private:
    std::vector<Domain> forbidden_domains_;
};

std::vector<Domain> ReadDomains(std::istream& input, size_t count) {
    std::vector<Domain> domains;
    std::string domain_line;
    
    while (count--) {
        std::getline(input, domain_line);
        domains.emplace_back(domain_line);
    }
    
    return domains;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);
    
    Number num;
    std::istringstream(line) >> num;
    
    return num;
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains);
    
    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}