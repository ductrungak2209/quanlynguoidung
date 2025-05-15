#include <bits/stdc++.h>
using namespace std;
// ==== HÀM XÁC ĐỊNH SỐ ====
int checkNumber(){
    int number;
    while (true) {
        std::cin >> number;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Lỗi:Nhập lại số hợp lệ!" << endl;
        } else {
            break; 
        }
    }
    return number;
}

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
// ==== SAO LƯU FILE ====
void backupFile(const string& originalFile) {
    ifstream src(originalFile, ios::binary);
    if (!src.is_open()) {
        cerr << "Không thể mở file " << originalFile << " để sao lưu.\n";
        return;
    }
    string backupFile = originalFile + ".bak";
    ofstream dst(backupFile, ios::binary);
    dst << src.rdbuf();
    src.close();
    dst.close();
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
    bool is_admin;
    bool require_password_change;

    UserAccount(string id, string uname, string mail, string password, bool admin = false, bool requireChange = false) {
        user_id = id;
        username = uname;
        email = mail;
        password_hash = simpleHash(password);
        is_admin = admin;
        require_password_change = requireChange;
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

void saveWalletsToFile() {
    ofstream walletFile("wallets.txt");
    if (!walletFile.is_open()) {
        cerr << "Không thể mở file wallets.txt để ghi dữ liệu.\n";
        return;
    }

    for (const auto& entry : usernameToWalletId) {
        const string& username = entry.first;
        int walletId = entry.second;
        Wallet& wallet = wallets[walletId];
        walletFile << username << " " << wallet.walletId << " " << wallet.balance << endl;
    }

    walletFile.close();
}

void saveUserAccount(const UserAccount& user) {
    ofstream file("users.txt", ios::app);
    if (file.is_open()) {
        file << user.user_id << "," << user.username << "," << user.email << "," << user.password_hash << ","
             << user.is_admin << "," << user.require_password_change << endl;
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
        string id, uname, email, hash, adminStr, requireChangeStr;
        getline(ss, id, ',');
        getline(ss, uname, ',');
        getline(ss, email, ',');
        getline(ss, hash, ',');
        getline(ss, adminStr, ',');
        getline(ss, requireChangeStr);
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
        string id, uname, email, stored_hash, adminStr, requireChangeStr;
        getline(ss, id, ',');
        getline(ss, uname, ',');
        getline(ss, email, ',');
        getline(ss, stored_hash, ',');
        getline(ss, adminStr, ',');
        getline(ss, requireChangeStr);
        if (uname == username && stored_hash == hashedPassword) return true;
    }
    return false;
}
bool changePassword(const string& username, const string& newPassword){
    ifstream file("users.txt");
    if (!file.is_open()) return false;
    ofstream tempFile("temp.txt");
    string line;
    string hashedPassword = simpleHash(newPassword);
    while (getline(file, line)){
        stringstream ss(line);
        string id, uname, email, stored_hash, adminStr, requireChangeStr;
        getline(ss, id, ',');
        getline(ss, uname, ',');
        getline(ss, email, ',');
        getline(ss, stored_hash, ',');
        getline(ss, adminStr, ',');
        getline(ss, requireChangeStr);
        if (uname == username){
            tempFile << id << "," << uname << "," << email << "," << hashedPassword << "," << adminStr << "," << requireChangeStr << endl;
        }
            
        
    } file.close();
    tempFile.close();
    remove("users.txt");
    rename("temp.txt", "users.txt");
     return true;}
bool isAdmin(const string& username) {
    ifstream file("users.txt");
    if (!file.is_open()) return false;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, uname, email, stored_hash, adminStr, requireChangeStr;
        getline(ss, id, ',');
        getline(ss, uname, ',');
        getline(ss, email, ',');
        getline(ss, stored_hash, ',');
        getline(ss, adminStr, ',');
        getline(ss, requireChangeStr);
        if (uname == username) return (adminStr == "1" || adminStr == "true");
    }
    return false;
}

bool checkRequirePasswordChange(const string& username) {
    ifstream file("users.txt");
    if (!file.is_open()) return false;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, uname, email, stored_hash, adminStr, requireChangeStr;
        getline(ss, id, ',');
        getline(ss, uname, ',');
        getline(ss, email, ',');
        getline(ss, stored_hash, ',');
        getline(ss, adminStr, ',');
        getline(ss, requireChangeStr);
        if (uname == username) return (requireChangeStr == "1" || requireChangeStr == "true");
    }
    return false;
}

void clearRequirePasswordChangeFlag(const string& username) {
    ifstream file("users.txt");
    if (!file.is_open()) return;

    ofstream tempFile("temp.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, uname, email, hash, adminStr, requireChangeStr;
        getline(ss, id, ',');
        getline(ss, uname, ',');
        getline(ss, email, ',');
        getline(ss, hash, ',');
        getline(ss, adminStr, ',');
        getline(ss, requireChangeStr);
        if (uname == username) requireChangeStr = "0";
        tempFile << id << "," << uname << "," << email << "," << hash << "," << adminStr << "," << requireChangeStr << endl;
    }
    file.close();
    tempFile.close();
    remove("users.txt");
    rename("temp.txt", "users.txt");
}

void transferPoints(const string& fromUser, const string& toUser, int amount, bool isSendOtp = true ) {
    if (isSendOtp) {
        srand(time(0));
        int otpCode = rand() % 900000 + 100000;
        if (!verifyOTP(otpCode)) {
            cout << "Xác thực OTP thất bại!\n";
            return;
        }
        
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

    saveWalletsToFile();

    cout << "Chuyển điểm thành công!\n";
}

void depositPoints(const string& username, int amount, bool isSendOtp = true) {
    if (isSendOtp){
        srand(time(0));
        int otpCode = rand() % 900000 + 100000;
        if (!verifyOTP(otpCode)) {
            cout << "Xác thực OTP thất bại!\n";
            return;
        }
    }

    if (usernameToWalletId.count(username) == 0) {
        cout << "Người dùng không tồn tại hoặc chưa có ví.\n";
        return;
    }
    Wallet& wallet = wallets[usernameToWalletId[username]];
    wallet.balance += amount;
    string log = "Nạp " + to_string(amount) + " điểm vào ví lúc " + to_string(time(0));
    wallet.transactionLog.push_back(log);

    saveWalletsToFile();

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

void createAccountByAdmin() {
    string id, uname, mail, pwd;
    bool isGen = false;
    cout << "Nhập ID người dùng: "; cin >> id;
    cout << "Nhập tên đăng nhập người dùng: "; cin >> uname;
    if (existingUsernames.count(uname)) {
        cout << "Tên đăng nhập đã tồn tại. Vui lòng chọn tên khác.\n";
        return;
    }
    cout << "Nhập email: "; cin >> mail;
    cout << "Nhập mật khẩu (hoặc gõ 'auto' để tạo tự động): "; cin >> pwd;

    bool requirePasswordChange = false;
    if (pwd == "auto") {
        pwd = generateRandomPassword();
        isGen = true;
        cout << "Mật khẩu tự động được tạo: " << pwd << endl;
        requirePasswordChange = true;
    }

    UserAccount newUser(id, uname, mail, pwd, false, isGen); // false vì đây là tài khoản người dùng
    saveUserAccount(newUser);
    createWalletForUser(uname);

    if (requirePasswordChange) {
    cout << "Sau khi đăng nhập bằng mật khẩu tự tạo bạn cần đổi mật khẩu mới.\n";    
    }

    cout << "Tạo tài khoản người dùng thành công.\n";
}

void AdminDashboard(const string& adminUsername) {
    while (true) {
        cout << "\n==== BẢNG ĐIỀU KHIỂN ADMIN ====\n";
        cout << "1. Xem danh sách người dùng\n";
        cout << "2. Thay đổi thông tin người dùng\n";
        cout << "3. Tạo tài khoản hộ người dùng\n";
        cout << "4. Nạp điểm cho người dùng\n";
        cout << "5. Chuyển điểm cho người dùng\n";
        cout << "6. Đăng xuất\n";
        cout << "Chọn một tùy chọn: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            ifstream file("users.txt");
            string line;
            cout << "\n--- Danh sách người dùng ---\n";
            while (getline(file, line)) {
                cout << line << endl;
            }
            file.close();
        } else if (choice == 2) {
            string username, newPass;
            cout << "Nhập tên đăng nhập cần đổi mật khẩu: ";
            cin >> username;
            cout << "Nhập mật khẩu mới: ";
            cin >> newPass;
            changePassword(username, newPass);
            cout << "Đã cập nhật mật khẩu thành công.\n";
        } else if (choice == 3) {
            createAccountByAdmin();
        } else if (choice == 4) {
            string targetuser;
            int amount;
            cout << "Nhập tên người dùng cần nạp điểm: ";
            cin >> targetuser;
            cout << "Nhập số điểm cần nạp: ";
            amount = checkNumber();
            depositPoints(targetuser, amount, false);
            cout << "Đã nạp điểm thành công.\n";
        } else if (choice == 5) {
            string targetuser, touser;
            int amount;
            cout << "Nhập tên người dùng cần chuyển điểm: ";
            cin >> targetuser;
            cout << "Nhập tên người dùng nhận điểm: ";
            cin >> touser;
            cout << "Nhập số điểm cần chuyển: ";
            amount = checkNumber();
            transferPoints(targetuser, touser, amount, false);
            cout << "Đã chuyển điểm thành công.\n";
        } else if (choice == 6) {
            cout << "Đăng xuất khỏi admin.\n" << endl;
            break;
        } else {
            cout << "Lựa chọn không hợp lệ.\n";
        }
    }
}

int main() {
    
    backupFile("users.txt");
    backupFile("wallets.txt");
    
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

            if (checkRequirePasswordChange(uname)) {
                cout << "Bạn cần đổi mật khẩu trước khi tiếp tục.\n";
                string newPass;
                cout << "Nhập mật khẩu mới: ";
                cin >> newPass;
                changePassword(uname, newPass);
                clearRequirePasswordChangeFlag(uname);
                cout << "Đổi mật khẩu thành công.\n";
            }
                
                if (isAdmin(uname)) {
                    AdminDashboard(uname);
                    continue;
                }

                while (true) {
                    cout << "\n--- Menu người dùng ---\n";
                    cout << "1. Xem ví\n";
                    cout << "2. Nạp điểm\n";
                    cout << "3. Chuyển điểm\n";
                    cout << "4. Đổi mật khẩu\n";
                    cout << "0. Đăng xuất\n";
                    cout << "Chọn tùy chọn: ";
                    int userChoice;
                    cin >> userChoice;

                    if (userChoice == 1) {
                        viewWallet(uname);
                    } else if (userChoice == 2) {
                        int amount;
                        cout << "Nhập số điểm muốn nạp: "; 
                        amount = checkNumber();
                        depositPoints(uname, amount);
                    } else if (userChoice == 3) {
                        string toUser;
                        int amount;
                        cout << "Tên người nhận: ";
                        cin >> toUser;
                        cout << "Số điểm chuyển: ";
                        amount = checkNumber();
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

