#include <bits/stdc++.h>
using namespace std;

// ========== HASH FUNCTION ==========
string simpleHash(const string& password) {
    hash<string> hasher;
    return to_string(hasher(password));
}

// ========== USER ACCOUNT SYSTEM ==========
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

void saveUserAccount(const UserAccount& user) {
    ofstream file("users.txt", ios::app);
    if (file.is_open()) {
        file << user.user_id << "," << user.username << "," << user.email << "," << user.password_hash << "," << user.created_by_admin << endl;
        file.close();
    } else {
        cerr << "Khong the mo file de luu nguoi dung.\n";
    }
}

bool verifyLogin(const string& username, const string& password) {
    ifstream file("users.txt");
    if (!file.is_open()) {
        cerr << "Khong the mo file nguoi dung.\n";
        return false;
    }

    string id, uname, email, stored_hash;
    bool admin;
    string hashedPassword = simpleHash(password);

    while (getline(file, id, ',') &&
           getline(file, uname, ',') &&
           getline(file, email, ',') &&
           getline(file, stored_hash, ',') &&
           file >> admin) {
        file.ignore();
        if (uname == username && stored_hash == hashedPassword) {
            return true;
        }
    }
    return false;
}

string generateRandomPassword(int length = 12) {
    const string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, chars.size() - 1);

    string password;
    for (int i = 0; i < length; i++) {
        password += chars[distr(gen)];
    }
    return password;
}

string generateOTP() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(100000, 999999);
    return to_string(distr(gen));
}

void changePassword(const string& username, const string& newPassword) {
    ifstream file("users.txt");
    if (!file.is_open()) {
        cerr << "Khong the mo file nguoi dung.\n";
        return;
    }

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

void backupData() {
    ifstream source("users.txt", ios::binary);
    ofstream backup("users_backup.txt", ios::binary);
    if (!source.is_open() || !backup.is_open()) {
        cerr << "Khong the sao luu du lieu.\n";
        return;
    }
    backup << source.rdbuf();
    source.close();
    backup.close();
}

void viewAllUsers() {
    ifstream file("users.txt");
    if (!file.is_open()) {
        cerr << "Khong the mo file nguoi dung.\n";
        return;
    }

    string id, uname, email, hash;
    bool admin;
    cout << "===== DANH SACH TAI KHOAN =====\n";
    while (getline(file, id, ',') &&
           getline(file, uname, ',') &&
           getline(file, email, ',') &&
           getline(file, hash, ',') &&
           file >> admin) {
        file.ignore();
        cout << "ID: " << id << ", Username: " << uname
             << ", Email: " << email
             << ", Quan ly: " << (admin ? "Co" : "Khong") << endl;
    }
    file.close();
}

void updateUserInfo(const string& username) {
    string otp;
    string actualOtp = generateOTP();
    cout << "Ma OTP cua ban: " << actualOtp << "\nNhap ma OTP de xac nhan cap nhat: ";
    cin >> otp;

    if (otp != actualOtp) {
        cout << "OTP khong hop le. Huy thao tac.\n";
        return;
    }

    string newEmail;
    cout << "Nhap email moi: ";
    cin >> newEmail;

    ifstream file("users.txt");
    if (!file.is_open()) {
        cerr << "Khong the mo file nguoi dung.\n";
        return;
    }

    ofstream temp("temp.txt");
    string id, uname, email, hash;
    bool admin;

    while (getline(file, id, ',') &&
           getline(file, uname, ',') &&
           getline(file, email, ',') &&
           getline(file, hash, ',') &&
           file >> admin) {
        file.ignore();
        if (uname == username) {
            email = newEmail;
        }
        temp << id << "," << uname << "," << email << "," << hash << "," << admin << endl;
    }

    file.close();
    temp.close();
    remove("users.txt");
    rename("temp.txt", "users.txt");

    cout << "Cap nhat thanh cong.\n";
}

void createAdminAccount() {
    string id, username, email, password;
    cout << "Nhap ID: "; cin >> id;
    cout << "Nhap ten dang nhap: "; cin >> username;
    cout << "Nhap email: "; cin >> email;
    cout << "Nhap mat khau (hoac nhap 'auto'): "; cin >> password;

    if (password == "auto") {
        password = generateRandomPassword();
        cout << "Mat khau tu tao: " << password << endl;
    }

    UserAccount adminUser(id, username, email, password, true);
    saveUserAccount(adminUser);
    cout << "Tao tai khoan quan ly thanh cong.\n";
}

// ========== WALLET SYSTEM ==========
const double DONGS_PER_POINT = 10.0;

class Wallet {
public:
    int walletId;
    int balance;
    vector<string> transactionLog;

    Wallet() : walletId(-1), balance(0) {}
    Wallet(int id, int initialBalance = 0) : walletId(id), balance(initialBalance) {}
};

class WalletUser {
public:
    string username;
    int walletId;
    WalletUser() : username(""), walletId(-1) {}
    WalletUser(string uname, int wId) : username(uname), walletId(wId) {}
};

unordered_map<string, WalletUser> users;
unordered_map<int, Wallet> wallets;
int nextWalletId = 3;

string sinhOTP() {
    string otp = "";
    for (int i = 0; i < 6; i++) {
        otp += to_string(rand() % 10);
    }
    return otp;
}

void chuyenDiem(string fromUser, string toUser, int amount) {
    auto sender = users.find(fromUser);
    auto receiver = users.find(toUser);

    if (sender == users.end() || receiver == users.end()) {
        cout << "Nguoi dung khong ton tai!\n";
        return;
    }

    int fromWalletId = sender->second.walletId;
    int toWalletId = receiver->second.walletId;

    Wallet& walletA = wallets[fromWalletId];
    Wallet& walletB = wallets[toWalletId];

    string otp = sinhOTP();
    cout << "OTP giao dich: " << otp << "\nNhap OTP: ";
    string enteredOtp;
    cin >> enteredOtp;

    if (enteredOtp != otp) {
        cout << "OTP khong hop le! Giao dich bi huy.\n";
        return;
    }

    if (walletA.balance >= amount) {
        walletA.balance -= amount;
        walletB.balance += amount;

        string log = "Chuyen " + to_string(amount) + " diem tu " + fromUser + " den " + toUser + " luc " + to_string(time(0));
        walletA.transactionLog.push_back(log);
        walletB.transactionLog.push_back(log);

        cout << "Giao dich thanh cong!\n";
    } else {
        cout << "So du khong du! Giao dich bi huy.\n";
    }
}

void xemVi(const string& username) {
    auto it = users.find(username);
    if (it == users.end()) {
        cout << "Nguoi dung khong tim thay!\n";
        return;
    }

    int walletId = it->second.walletId;
    Wallet& wallet = wallets[walletId];

    cout << "ID Vi: " << wallet.walletId << "\n";
    cout << "So du: " << wallet.balance << " diem (" << wallet.balance * DONGS_PER_POINT << " do)\n";
    cout << "Lich su giao dich:\n";
    for (const string& log : wallet.transactionLog) {
        cout << " - " << log << "\n";
    }
}

void dangKyNguoiDungVi() {
    string newUsername;
    cout << "Nhap ten nguoi dung moi: ";
    cin >> newUsername;

    if (users.find(newUsername) != users.end()) {
        cout << "Ten nguoi dung da ton tai!\n";
        return;
    }

    int newWalletId = nextWalletId++;
    wallets[newWalletId] = Wallet(newWalletId, 0);
    users[newUsername] = WalletUser(newUsername, newWalletId);

    cout << "Dang ky thanh cong! Wallet ID: " << newWalletId << ", so du = 0 diem.\n";
}

void khoiTaoHeThongVi() {
    srand(static_cast<unsigned int>(time(0)));
    wallets[0] = Wallet(0, 10000);
    users["admin"] = WalletUser("admin", 0);
    wallets[1] = Wallet(1, 100);
    users["duy"] = WalletUser("duy", 1);
    wallets[2] = Wallet(2, 50);
    users["khang"] = WalletUser("khang", 2);
}

// ========== MAIN FUNCTION ==========
int main() {
    khoiTaoHeThongVi();
    int luaChon;
    while (true) {
        cout << "\n----- Menu -----\n";
        cout << "1. Dang ky tai khoan\n2. Dang nhap\n3. Doi mat khau\n4. Tao OTP (demo)\n5. Sao luu du lieu\n6. Xem danh sach nguoi dung\n7. Cap nhat email\n8. Tao tai khoan quan ly\n9. Xem thong tin vi\n10. Chuyen diem\n11. Dang ky nguoi dung moi cho vi\n12. Thoat\n";
        cout << "Lua chon: ";
        cin >> luaChon;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Vui long nhap mot so hop le!\n";
            continue;
        }

        switch (luaChon) {
            case 1: {
                string id, username, email, password;
                cout << "Nhap ID: "; cin >> id;
                cout << "Nhap ten dang nhap: "; cin >> username;
                cout << "Nhap email: "; cin >> email;
                cout << "Nhap mat khau (hoac nhap 'auto'): "; cin >> password;
                if (password == "auto") {
                    password = generateRandomPassword();
                    cout << "Mat khau tu tao: " << password << endl;
                }
                UserAccount user(id, username, email, password);
                saveUserAccount(user);
                cout << "Tai khoan da duoc tao!\n";
                break;
            }
            case 2: {
                string username, password;
                cout << "Nhap ten dang nhap: ";
                cin >> username;
                cout << "Nhap mat khau: ";
                cin >> password;
                if (verifyLogin(username, password)) {
                    cout << "Dang nhap thanh cong!\n";
                } else {
                    cout << "Sai thong tin dang nhap!\n";
                }
                break;
            }
            case 3: {
                string username, newPassword;
                cout << "Nhap ten dang nhap: ";
                cin >> username;
                cout << "Nhap mat khau moi: ";
                cin >> newPassword;
                changePassword(username, newPassword);
                cout << "Mat khau da duoc thay doi!\n";
                break;
            }
            case 4:
                cout << "Ma OTP (demo): " << generateOTP() << endl;
                break;
            case 5:
                backupData();
                cout << "Du lieu da duoc sao luu!\n";
                break;
            case 6:
                viewAllUsers();
                break;
            case 7: {
                string username;
                cout << "Nhap ten dang nhap can cap nhat: ";
                cin >> username;
                updateUserInfo(username);
                break;
            }
            case 8:
                createAdminAccount();
                break;
            case 9: {
                string username;
                cout << "Nhap ten dang nhap: ";
                cin >> username;
                xemVi(username);
                break;
            }
            case 10: {
                string fromUser, toUser;
                int amount;
                cout << "Nhap nguoi chuyen: ";
                cin >> fromUser;
                cout << "Nhap nguoi nhan: ";
                cin >> toUser;
                cout << "Nhap so diem: ";
                cin >> amount;
                chuyenDiem(fromUser, toUser, amount);
                break;
            }
            case 11:
                dangKyNguoiDungVi();
                break;
            case 12:
                cout << "Thoat chuong trinh.\n";
                return 0;
            default:
                cout << "Lua chon khong hop le!\n";
        }
    }
    return 0;
}
