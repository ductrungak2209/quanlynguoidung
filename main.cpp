#include <bits/stdc++.h>
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


// === PHẦN B: PHÂN QUYỀN, XEM DANH SÁCH, CẬP NHẬT THÔNG TIN ===

void viewAllUsers() {
    ifstream file("users.txt");
    string id, uname, email, hash;
    bool admin;
    cout << " ===== DANH SÁCH TÀI KHOẢN ===== ";
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

void updateUserInfo(const string& username) {
    string otp;
    cout << "Nhập mã OTP để xác nhận cập nhật: ";
    cin >> otp;

    if (otp != generateOTP()) {
        cout << "OTP không hợp lệ. Hủy thao tác.
";
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

    cout << "Cập nhật thành công.
";
}

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
    cout << "Tạo tài khoản quản lý thành công.
";
}


int main() {
    cout << "----- QUẢN LÝ TÀI KHOẢN NGƯỜI DÙNG -----\n";
    return 0;
}
