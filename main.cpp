#include <bits/stdc++.h>
using namespace std;

// ==== PHẦN BĂM MẬT KHẨU ====
string simpleHash(const string& password) {
    hash<string> hasher;
    return to_string(hasher(password));
}

// ==== LỚP NGƯỜI DÙNG ====
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

// ==== LỚP VÍ ====
class Wallet {
public:
    int walletId;
    int balance;
    vector<string> transactionLog;

    Wallet() : walletId(-1), balance(0) {}
    Wallet(int id, int initialBalance = 0) : walletId(id), balance(initialBalance) {}
};

// ==== QUẢN LÝ NGƯỜI DÙNG & VÍ ====
unordered_map<string, int> usernameToWalletId;
unordered_map<int, Wallet> wallets;
int nextWalletId = 1000;

// ==== LƯU NGƯỜI DÙNG ====
void saveUserAccount(const UserAccount& user) {
    ofstream file("users.txt", ios::app);
    if (file.is_open()) {
        file << user.user_id << "," << user.username << "," << user.email << "," << user.password_hash << "," << user.created_by_admin << endl;
        file.close();
    } else {
        cerr << "Không thể mở file để lưu người dùng.\n";
    }
}

// ==== XÁC THỰC ĐĂNG NHẬP ====
bool verifyLogin(const string& username, const string& password) {
    ifstream file("users.txt");
    if (!file.is_open()) return false;

    string id, uname, email, stored_hash;
    bool admin;
    string hashedPassword = simpleHash(password);

    while (getline(file, id, ',') &&
           getline(file, uname, ',') &&
           getline(file, email, ',') &&
           getline(file, stored_hash, ',') &&
           file >> admin) {
        file.ignore();
        if (uname == username && stored_hash == hashedPassword) return true;
    }
    return false;
}

// ==== ĐỔI MẬT KHẨU ====
void changePassword(const string& username, const string& newPassword) {
    ifstream file("users.txt");
    if (!file.is_open()) return;

    ofstream tempFile("temp.txt");
    string id, uname, email, stored_hash;
    bool admin;

    while (getline(file, id, ',') &&
           getline(file, uname, ',') &&
           getline(file, email, ',') &&
           getline(file, stored_hash, ',') &&
           file >> admin) {
        file.ignore();
        if (uname == username) {
            stored_hash = simpleHash(newPassword);
        }
        tempFile << id << "," << uname << "," << email << "," << stored_hash << "," << admin << endl;
    }

    file.close();
    tempFile.close();
    remove("users.txt");
    rename("temp.txt", "users.txt");
}

// ==== CHUYỂN ĐIỂM ====
void transferPoints(const string& fromUser, const string& toUser, int amount) {
    if (usernameToWalletId.count(fromUser) == 0 || usernameToWalletId.count(toUser) == 0) {
        cout << "Tên người dùng không tồn tại!\n";
        return;
    }

    int fromId = usernameToWalletId[fromUser];
    int toId = usernameToWalletId[toUser];

    Wallet& fromWallet = wallets[fromId];
    Wallet& toWallet = wallets[toId];

    if (fromWallet.balance < amount) {
        cout << "Số dư không đủ!\n";
        return;
    }

    fromWallet.balance -= amount;
    toWallet.balance += amount;

    string log = "Chuyển " + to_string(amount) + " điểm từ " + fromUser + " đến " + toUser + " tại thời gian " + to_string(time(0));
    fromWallet.transactionLog.push_back(log);
    toWallet.transactionLog.push_back(log);

    cout << "Chuyển điểm thành công!\n";
}

// ==== NẠP ĐIỂM ====
void depositPoints(const string& username, int amount) {
    if (usernameToWalletId.count(username) == 0) {
        cout << "Người dùng không tồn tại hoặc chưa có ví.\n";
        return;
    }
    Wallet& wallet = wallets[usernameToWalletId[username]];
    wallet.balance += amount;
    string log = "Nạp " + to_string(amount) + " điểm vào ví lúc " + to_string(time(0));
    wallet.transactionLog.push_back(log);
    cout << "Nạp điểm thành công!\n";
}

// ==== XEM VÍ ====
void viewWallet(const string& username) {
    if (usernameToWalletId.count(username) == 0) {
        cout << "Người dùng không có ví.\n";
        return;
    }

    Wallet& wallet = wallets[usernameToWalletId[username]];
    cout << "ID ví: " << wallet.walletId << "\n";
    cout << "Số dư: " << wallet.balance << " điểm\n";
    cout << "Lịch sử giao dịch:\n";
    for (const string& log : wallet.transactionLog) {
        cout << " - " << log << endl;
    }
}

// ==== TẠO VÍ MỚI KHI ĐĂNG KÝ ====
void createWalletForUser(const string& username) {
    wallets[nextWalletId] = Wallet(nextWalletId, 0);
    usernameToWalletId[username] = nextWalletId;
    nextWalletId++;
}

// ==== MAIN ====
int main() {
    cout << "----- HỆ THỐNG QUẢN LÝ NGƯỜI DÙNG & VÍ -----\n";

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Đăng ký\n2. Đăng nhập\n3. Đổi mật khẩu\n4. Xem ví\n5. Chuyển điểm\n6. Nạp điểm\n7. Thoát\n";
        int choice;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Vui lòng nhập số hợp lệ.\n";
            continue;
        }

        if (choice == 1) {
            string id, username, email, password;
            cout << "Nhập ID: "; cin >> id;
            cout << "Nhập tên đăng nhập: "; cin >> username;
            cout << "Nhập email: "; cin >> email;
            cout << "Nhập mật khẩu: "; cin >> password;

            UserAccount user(id, username, email, password);
            saveUserAccount(user);
            createWalletForUser(username);
            cout << "Tài khoản và ví đã được tạo thành công.\n";
        }
        else if (choice == 2) {
            string username, password;
            cout << "Tên đăng nhập: "; cin >> username;
            cout << "Mật khẩu: "; cin >> password;
            if (verifyLogin(username, password)) {
                cout << "Đăng nhập thành công.\n";
            } else {
                cout << "Sai tên đăng nhập hoặc mật khẩu.\n";
            }
        }
        else if (choice == 3) {
            string username, newPass;
            cout << "Tên đăng nhập: "; cin >> username;
            cout << "Mật khẩu mới: "; cin >> newPass;
            changePassword(username, newPass);
            cout << "Đổi mật khẩu thành công.\n";
        }
        else if (choice == 4) {
            string username;
            cout << "Nhập tên đăng nhập: ";
            cin >> username;
            viewWallet(username);
        }
        else if (choice == 5) {
            string fromUser, toUser;
            int amount;
            cout << "Từ tài khoản: "; cin >> fromUser;
            cout << "Tới tài khoản: "; cin >> toUser;
            cout << "Số điểm: "; cin >> amount;
            transferPoints(fromUser, toUser, amount);
        }
        else if (choice == 6) {
            string username;
            int amount;
            cout << "Nhập tên đăng nhập: "; cin >> username;
            cout << "Số điểm cần nạp: "; cin >> amount;
            depositPoints(username, amount);
        }
        else if (choice == 7) {
            cout << "Tạm biệt!\n";
            break;
        }
        else {
            cout << "Lựa chọn không hợp lệ!\n";
        }
    }

    return 0;
}
