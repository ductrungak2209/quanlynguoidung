#include <bits/stdc++.h>
using namespace std;

// ==== PHẦN BĂM MẬT KHẨU ====
string simpleHash(const string& password) {
    hash<string> hasher;
    return to_string(hasher(password));
}

// ==== HÀM XÁC THỰC OTP ====
bool verifyOTP(int otpCode, const string& userInput = "") {
    cout << "Mã OTP của bạn là: " << otpCode << endl;

    string input;
    if (userInput.empty()) {
        cout << "Nhập mã OTP: ";
        cin >> input;
    } else {
        input = userInput;
    }

    return input == to_string(otpCode);
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

unordered_map<string, int> usernameToWalletId;
unordered_map<int, Wallet> wallets;
int nextWalletId = 1000;

unordered_set<string> existingUsernames;

void loadWalletsFromFile() {
    ifstream walletFile("wallets.txt");
    if (!walletFile.is_open()) {
        cerr << "Không thể mở file wallets.txt để đọc dữ liệu.\n";
        return;
    }

    string username;
    int id, balance;
    while (walletFile >> username >> id >> balance) {
        Wallet wallet(id, balance);
        wallets[id] = wallet;
        usernameToWalletId[username] = id;

        if (id >= nextWalletId) nextWalletId = id + 1;
    }

    walletFile.close();
}

void saveUserAccount(const UserAccount& user) {
    ofstream file("users.txt", ios::app);
    if (file.is_open()) {
        file << user.user_id << "," << user.username << "," << user.email << "," << user.password_hash << "," << user.created_by_admin << endl;
        file.close();
        existingUsernames.insert(user.username);
    } else {
        cerr << "Không thể mở file để lưu người dùng.\n";
    }
}

void loadExistingUsernames() {
    ifstream file("users.txt");
    if (!file.is_open()) return;

    string id, uname, email, hash;
    bool admin;
    while (getline(file, id, ',') && getline(file, uname, ',') && getline(file, email, ',') && getline(file, hash, ',') && file >> admin) {
        file.ignore();
        existingUsernames.insert(uname);
    }
    file.close();
}

bool verifyLogin(const string& username, const string& password) {
    ifstream file("users.txt");
    if (!file.is_open()) return false;

    string id, uname, email, stored_hash;
    bool admin;
    string hashedPassword = simpleHash(password);

    while (getline(file, id, ',') && getline(file, uname, ',') && getline(file, email, ',') && getline(file, stored_hash, ',') && file >> admin) {
        file.ignore();
        if (uname == username && stored_hash == hashedPassword) return true;
    }
    return false;
}

bool isAdmin(const string& username) {
    ifstream file("users.txt");
    if (!file.is_open()) return false;

    string id, uname, email, stored_hash;
    bool admin;

    while (getline(file, id, ',') && getline(file, uname, ',') && getline(file, email, ',') && getline(file, stored_hash, ',') && file >> admin) {
        file.ignore();
        if (uname == username) return admin;
    }
    return false;
}

void changePassword(const string& username, const string& newPassword) {
    ifstream file("users.txt");
    if (!file.is_open()) return;

    ofstream tempFile("temp.txt");
    string id, uname, email, stored_hash;
    bool admin;

    while (getline(file, id, ',') && getline(file, uname, ',') && getline(file, email, ',') && getline(file, stored_hash, ',') && file >> admin) {
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

void transferPoints(const string& fromUser, const string& toUser, int amount) {
    srand(time(0));
    int otpCode = rand() % 900000 + 100000;
    if (!verifyOTP(otpCode)) {
        cout << "Xác thực OTP thất bại!\n";
        return;
    }

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

void depositPoints(const string& username, int amount) {
    srand(time(0));
    int otpCode = rand() % 900000 + 100000;
    if (!verifyOTP(otpCode)) {
        cout << "Xác thực OTP thất bại!\n";
        return;
    }

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

void createWalletForUser(const string& username) {
    wallets[nextWalletId] = Wallet(nextWalletId, 0);
    usernameToWalletId[username] = nextWalletId;

    ofstream walletFile("wallets.txt", ios::app);
    if (walletFile.is_open()) {
        walletFile << username << " " << nextWalletId << " 0\n";
        walletFile.close();
    } else {
        cerr << "Không thể mở file wallets.txt để lưu ví.\n";
    }

    nextWalletId++;
}

void AdminDashboard(const string& adminUsername);
void createAccountByAdmin() {
    string id, uname, mail, pwd;
    cout << "Nhập ID người dùng: "; cin >> id;
    cout << "Nhập tên đăng nhập: "; cin >> uname;
    if (existingUsernames.count(uname)) {
        cout << "Tên đăng nhập đã tồn tại. Vui lòng chọn tên khác.\n";
        return;
    }
    cout << "Nhập email: "; cin >> mail;
    cout << "Nhập mật khẩu: "; cin >> pwd;
    UserAccount newUser(id, uname, mail, pwd, false);
    saveUserAccount(newUser);
    createWalletForUser(uname);
    cout << "Tạo tài khoản người dùng thành công.\n";
}

int main() {
    loadWalletsFromFile();
    loadExistingUsernames();

    while (true) {
        cout << "\n==== HỆ THỐNG VÍ NGƯỜI DÙNG ====\n";
        cout << "1. Đăng ký tài khoản\n";
        cout << "2. Đăng nhập\n";
        cout << "3. Thoát\n";
        cout << "Chọn một tùy chọn: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            string id, uname, mail, pwd;
            char isAdminChar;
            cout << "Nhập ID người dùng: "; cin >> id;
            cout << "Nhập tên đăng nhập: "; cin >> uname;
            if (existingUsernames.count(uname)) {
                cout << "Tên đăng nhập đã tồn tại. Vui lòng chọn tên khác.\n";
                continue;
            }
            cout << "Nhập email: "; cin >> mail;
            cout << "Nhập mật khẩu: "; cin >> pwd;
            cout << "Tài khoản này có phải admin không? (y/n): "; cin >> isAdminChar;
            bool isAdmin = (isAdminChar == 'y' || isAdminChar == 'Y');
            UserAccount newUser(id, uname, mail, pwd, isAdmin);
            saveUserAccount(newUser);
            createWalletForUser(uname);
            cout << "Tạo tài khoản thành công.\n";
        } else if (choice == 2) {
            string uname, pwd;
            cout << "Tên đăng nhập: "; cin >> uname;
            cout << "Mật khẩu: "; cin >> pwd;

            if (verifyLogin(uname, pwd)) {
                cout << "Đăng nhập thành công.\n";

                if (isAdmin(uname)) {
                    AdminDashboard(uname);
                }

                while (true) {
                    cout << "\n--- Menu người dùng ---\n";
                    cout << "1. Xem ví\n";
                    cout << "2. Nạp điểm\n";
                    cout << "3. Chuyển điểm\n";
                    cout << "4. Đổi mật khẩu\n";
                    if (isAdmin(uname)) cout << "5. Tạo tài khoản hộ người khác\n";
                    cout << "0. Đăng xuất\n";
                    cout << "Chọn tùy chọn: ";
                    int userChoice;
                    cin >> userChoice;

                    if (userChoice == 1) {
                        viewWallet(uname);
                    } else if (userChoice == 2) {
                        int amount;
                        cout << "Nhập số điểm muốn nạp: ";
                        cin >> amount;
                        depositPoints(uname, amount);
                    } else if (userChoice == 3) {
                        string toUser;
                        int amount;
                        cout << "Tên người nhận: ";
                        cin >> toUser;
                        cout << "Số điểm chuyển: ";
                        cin >> amount;
                        transferPoints(uname, toUser, amount);
                    } else if (userChoice == 4) {
                        srand(time(0));
                        int otpCode = rand() % 900000 + 100000;
                        if (verifyOTP(otpCode)) {
                            string newPass;
                            cout << "Nhập mật khẩu mới: ";
                            cin >> newPass;
                            changePassword(uname, newPass);
                            cout << "Đổi mật khẩu thành công.\n";
                        } else {
                            cout << "Xác thực OTP thất bại. Không thể đổi mật khẩu.\n";
                        }
                    } else if (userChoice == 5 && isAdmin(uname)) {
                        createAccountByAdmin();
                    } else if (userChoice == 0) {
                        cout << "Đăng xuất...\n";
                        break;
                    } else {
                        cout << "Lựa chọn không hợp lệ.\n";
                    }
                }
            } else {
                cout << "Sai tên đăng nhập hoặc mật khẩu.\n";
            }
        } else if (choice == 3) {
            cout << "Thoát chương trình.\n";
            break;
        } else {
            cout << "Lựa chọn không hợp lệ.\n";
        }
    }

    return 0;
}

void AdminDashboard(const string& adminUsername) {
    cout << "(ADMIN) Bạn có thể tạo tài khoản cho người khác.\n";
    while (true) {
        char c;
        cout << "Tạo tài khoản mới? (y/n): ";
        cin >> c;
        if (c == 'y' || c == 'Y') {
            createAccountByAdmin();
        } else {
            break;
        }
    }
}
