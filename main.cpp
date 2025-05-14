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

// Lưu tài khoản vào file
void saveUserAccount(const UserAccount& user) {
    ofstream file("users.txt", ios::app);
    if (file.is_open()) {
        file << user.user_id << "," << user.username << "," << user.email << "," << user.password_hash << "," << user.created_by_admin << endl;
        file.close();
    }
}

// Kiểm tra đăng nhập
bool verifyLogin(const string& username, const string& password) {
    ifstream file("users.txt");
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

// Tạo mật khẩu ngẫu nhiên
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

// Sinh mã OTP
string generateOTP() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(100000, 999999);
    return to_string(distr(gen));
}

// Đổi mật khẩu
void changePassword(const string& username, const string& newPassword) {
    ifstream file("users.txt");
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

// Sao lưu dữ liệu
void backupData() {
    ifstream source("users.txt", ios::binary);
    ofstream backup("users_backup.txt", ios::binary);
    backup << source.rdbuf();
    source.close();
    backup.close();
}

// Hiển thị tất cả người dùng
void viewAllUsers() {
    ifstream file("users.txt");
    string id, uname, email, hash;
    bool admin;
    cout << "===== DANH SÁCH TÀI KHOẢN =====\n";
    while (getline(file, id, ',') &&
           getline(file, uname, ',') &&
           getline(file, email, ',') &&
           getline(file, hash, ',') &&
           file >> admin) {
        file.ignore();
        cout << "ID: " << id << ", Username: " << uname
             << ", Email: " << email
             << ", Quản lý: " << (admin ? "Có" : "Không") << endl;
    }
    file.close();
}

// Cập nhật email người dùng với OTP xác nhận
void updateUserInfo(const string& username) {
    string otp;
    cout << "Nhập mã OTP để xác nhận cập nhật: ";
    cin >> otp;

    if (otp != generateOTP()) {
        cout << "OTP không hợp lệ. Hủy thao tác.\n";
        return;
    }

    string newEmail;
    cout << "Nhập email mới: ";
    cin >> newEmail;

    ifstream file("users.txt");
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

    cout << "Cập nhật thành công.\n";
}

// Tạo tài khoản quản trị
void createAdminAccount() {
    string id, username, email, password;
    cout << "Nhập ID: "; cin >> id;
    cout << "Nhập tên đăng nhập: "; cin >> username;
    cout << "Nhập email: "; cin >> email;
    cout << "Nhập mật khẩu (hoặc nhập 'auto'): "; cin >> password;

    if (password == "auto") {
        password = generateRandomPassword();
        cout << "Mật khẩu tự tạo: " << password << endl;
    }

    UserAccount adminUser(id, username, email, password, true);
    saveUserAccount(adminUser);
    cout << "Tạo tài khoản quản lý thành công.\n";
}

// Hàm main chính
int main() {
    cout << "----- QUẢN LÝ TÀI KHOẢN NGƯỜI DÙNG -----\n";
    while (true) {
        cout << "\nChọn thao tác:\n";
        cout << "1. Đăng ký\n2. Đăng nhập\n3. Đổi mật khẩu\n4. Tạo OTP\n5. Sao lưu dữ liệu\n6. Xem danh sách người dùng\n7. Cập nhật email\n8. Tạo tài khoản quản lý\n9. Thoát\n";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string id, username, email, password;
            cout << "Nhập ID: ";
            cin >> id;
            cout << "Nhập tên đăng nhập: ";
            cin >> username;
            cout << "Nhập email: ";
            cin >> email;
            cout << "Nhập mật khẩu (hoặc nhập 'auto' để tạo tự động): ";
            cin >> password;

            if (password == "auto") {
                password = generateRandomPassword();
                cout << "Mật khẩu tự tạo: " << password << endl;
            }

            UserAccount user(id, username, email, password);
            saveUserAccount(user);
            cout << "Tài khoản đã được tạo!\n";
        }
        else if (choice == 2) {
            string username, password;
            cout << "Nhập tên đăng nhập: ";
            cin >> username;
            cout << "Nhập mật khẩu: ";
            cin >> password;
            if (verifyLogin(username, password)) {
                cout << "Đăng nhập thành công!\n";
            } else {
                cout << "Sai thông tin đăng nhập!\n";
            }
        }
        else if (choice == 3) {
            string username, newPassword;
            cout << "Nhập tên đăng nhập: ";
            cin >> username;
            cout << "Nhập mật khẩu mới: ";
            cin >> newPassword;
            changePassword(username, newPassword);
            cout << "Mật khẩu đã được thay đổi!\n";
        }
        else if (choice == 4) {
            cout << "Mã OTP: " << generateOTP() << endl;
        }
        else if (choice == 5) {
            backupData();
            cout << "Dữ liệu đã được sao lưu!\n";
        }
        else if (choice == 6) {
            viewAllUsers();
        }
        else if (choice == 7) {
            string username;
            cout << "Nhập tên đăng nhập cần cập nhật: ";
            cin >> username;
            updateUserInfo(username);
        }
        else if (choice == 8) {
            createAdminAccount();
        }
        else if (choice == 9) {
            cout << "Thoát chương trình.\n";
            break;
        }
        else {
            cout << "Lựa chọn không hợp lệ!\n";
        }
    }
    return 0;
}
