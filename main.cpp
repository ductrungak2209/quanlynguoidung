#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <random>

using namespace std;

// Hàm băm 
string simpleHash(const string& password) {
    hash<string> hasher;
    return to_string(hasher(password));
}

class UserAccount {
public:
    string user_id;
    string username;
    string email;
    string password_hash;
    bool created_by_admin;

    UserAccount(string id, string uname, string mail, string password, bool admin = false) {
        user_id = id;
        username = uname;
        email = mail;
        password_hash = simpleHash(password);
        created_by_admin = admin;
    }
};


int main() {
    cout << "----- QUẢN LÝ TÀI KHOẢN NGƯỜI DÙNG -----\n";
    return 0;
}
