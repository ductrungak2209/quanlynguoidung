#include <bits/stdc++.h>
using namespace std;

// ==== HÀM TẠO MẬT KHẨU NGẪU NHIÊN ====
string generateRandomPassword(int length = 10) {
    const string chars =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "!@#$%^&*";

    string password;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, chars.size() - 1);
    for (int i = 0; i < length; ++i) {
        password += chars[dis(gen)];
    }
    return password;
}

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
    bool force_password_change;

    UserAccount(string id, string uname, string mail, string password, bool admin = false, bool forceChange = false) {
        user_id = id;
        username = uname;
        email = mail;
        password_hash = simpleHash(password);
        created_by_admin = admin;
        force_password_change = forceChange;
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

void createWalletForUser(const string& username) {
    if (usernameToWalletId.count(username)) {
        cout << "Người dùng đã có ví.\n";
        return;
    }
    Wallet newWallet(nextWalletId++);
    wallets[newWallet.walletId] = newWallet;
    usernameToWalletId[username] = newWallet.walletId;
    cout << "Ví mới đã được tạo cho người dùng " << username << " với ID ví: " << newWallet.walletId << endl;
}

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
        file << user.user_id << "," << user.username << "," << user.email << "," << user.password_hash << "," << user.created_by_admin << "," << user.force_password_change << endl;
        file.close();
        existingUsernames.insert(user.username);
    } else {
        cerr << "Không thể mở file để lưu người dùng.\n";
    }
}

void loadExistingUsernames() {
    ifstream file("users.txt");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, uname, email, hash, adminStr, forceChangeStr;
        getline(ss, id, ',');
        getline(ss, uname, ',');
        getline(ss, email, ',');
        getline(ss, hash, ',');
        getline(ss, adminStr, ',');
        getline(ss, forceChangeStr);
        existingUsernames.insert(uname);
    }
    file.close();
}

bool verifyLogin(const string& username, const string& password) {
    ifstream file("users.txt");
    if (!file.is_open()) return false;

    string line;
    string hashedPassword = simpleHash(password);

    while (getline(file, line)) {
        stringstream ss(line);
        string id, uname, email, stored_hash, adminStr, forceChangeStr;
        getline(ss, id, ',');
        getline(ss, uname, ',');
        getline(ss, email, ',');
        getline(ss, stored_hash, ',');
        getline(ss, adminStr, ',');
        getline(ss, forceChangeStr);
        if (uname == username && stored_hash == hashedPassword) return true;
    }
    return false;
}

bool isAdmin(const string& username) {
    ifstream file("users.txt");
    if (!file.is_open()) return false;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, uname, email, hash, adminStr, forceChangeStr;
        getline(ss, id, ',');
        getline(ss, uname, ',');
        getline(ss, email, ',');
        getline(ss, hash, ',');
        getline(ss, adminStr, ',');
        getline(ss, forceChangeStr);
        if (uname == username) return adminStr == "1";
    }
    return false;
}

bool needsPasswordChange(const string& username) {
    ifstream file("users.txt");
    if (!file.is_open()) return false;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, uname, email, hash, adminStr, forceChangeStr;
        getline(ss, id, ',');
        getline(ss, uname, ',');
        getline(ss, email, ',');
        getline(ss, hash, ',');
        getline(ss, adminStr, ',');
        getline(ss, forceChangeStr);
        if (uname == username) return forceChangeStr == "1";
    }
    return false;
}

void changePassword(const string& username, const string& newPassword, bool clearForceFlag = true) {
    ifstream file("users.txt");
    if (!file.is_open()) return;

    ofstream tempFile("temp.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, uname, email, hash, adminStr, forceChangeStr;
        getline(ss, id, ',');
        getline(ss, uname, ',');
        getline(ss, email, ',');
        getline(ss, hash, ',');
        getline(ss, adminStr, ',');
        getline(ss, forceChangeStr);

        if (uname == username) {
            hash = simpleHash(newPassword);
            if (clearForceFlag) forceChangeStr = "0";
        }
        tempFile << id << "," << uname << "," << email << "," << hash << "," << adminStr << "," << forceChangeStr << endl;
    }
    file.close();
    tempFile.close();
    remove("users.txt");
    rename("temp.txt", "users.txt");
}

void createAccountByAdmin() {
    string id, uname, mail, pwd;
    cout << "Nhập ID người dùng: "; cin >> id;
    cout << "Nhập tên đăng nhập: "; cin >> uname;
    if (existingUsernames.count(uname)) {
        cout << "Tên đăng nhập đã tồn tại. Vui lòng chọn tên khác.\n";
        return;
    }
    cout << "Nhập email: "; cin >> mail;
    cout << "Người dùng có muốn đặt mật khẩu không? (y/n): ";
    char choice; cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        cout << "Nhập mật khẩu: "; cin >> pwd;
    } else {
        pwd = generateRandomPassword(rand() % 5 + 8);
        cout << "Mật khẩu được tạo tự động là: " << pwd << endl;
    }
    UserAccount newUser(id, uname, mail, pwd, true, choice != 'y' && choice != 'Y');
    saveUserAccount(newUser);
    createWalletForUser(uname);
    cout << "Tạo tài khoản người dùng thành công.\n";
}

void AdminDashboard(const string& adminUsername);

// Function to deposit points into a user's wallet
void depositPoints(const string& username, int amount) {
    if (usernameToWalletId.count(username) == 0) {
        cout << "Người dùng không có ví.\n";
        return;
    }
    int walletId = usernameToWalletId[username];
    Wallet& wallet = wallets[walletId];
    wallet.balance += amount;

    stringstream log;
    log << "Nạp " << amount << " điểm vào ví của " << username;
    wallet.transactionLog.push_back(log.str());

    cout << "Nạp điểm thành công.\n";
}

// Function to transfer points between users
void transferPoints(const string& fromUser, const string& toUser, int amount) {
    if (usernameToWalletId.count(fromUser) == 0 || usernameToWalletId.count(toUser) == 0) {
        cout << "Một trong hai người dùng không có ví.\n";
        return;
    }

    int fromWalletId = usernameToWalletId[fromUser];
    int toWalletId = usernameToWalletId[toUser];

    Wallet& fromWallet = wallets[fromWalletId];
    Wallet& toWallet = wallets[toWalletId];

    if (fromWallet.balance < amount) {
        cout << "Số dư không đủ để thực hiện giao dịch.\n";
        return;
    }

    fromWallet.balance -= amount;
    toWallet.balance += amount;

    stringstream log;
    log << "Chuyển " << amount << " điểm từ " << fromUser << " đến " << toUser;
    fromWallet.transactionLog.push_back(log.str());
    toWallet.transactionLog.push_back(log.str());

    cout << "Chuyển điểm thành công.\n";
}

// Function to view the wallet of a user
void viewWallet(const string& username) {
    if (usernameToWalletId.count(username) == 0) {
        cout << "Người dùng không có ví.\n";
        return;
    }
    int walletId = usernameToWalletId[username];
    Wallet& wallet = wallets[walletId];
    cout << "ID ví: " << wallet.walletId << "\n";
    cout << "Số dư: " << wallet.balance << "\n";
    cout << "Lịch sử giao dịch:\n";
    for (const string& log : wallet.transactionLog) {
        cout << "- " << log << "\n";
    }
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
            cout << "Nhập mật khẩu (hoặc gõ 'auto' để tạo tự động): ";
            cin >> pwd;
            if (pwd == "auto") {
                pwd = generateRandomPassword(rand() % 5 + 8);
                cout << "Mật khẩu được tạo tự động là: " << pwd << endl;
            }
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
