//Khai báo thư viện
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <ctime>

//Sử dụng std để không phải viết std::cout, std::string mỗi lần.

using namespace std;

//Hằng số quy đổi 1 điểm = 10 đồng. Dùng để hiển thị số dư ví ra tiền đồng.
const double DONGS_PER_POINT = 10.0;

//Lớp Wallet – Đại diện cho ví người dùng
class Wallet {
public:
    int walletId;
    int balance;
    vector<string> transactionLog;

    Wallet() : walletId(-1), balance(0) {}
    Wallet(int id, int initialBalance = 0) : walletId(id), balance(initialBalance) {}
};

//Lớp User – Đại diện cho người dùng
class User {
public:
    string username;
    int walletId;
    User() : username(""), walletId(-1) {}
    User(string uname, int wId) : username(uname), walletId(wId) {}
};

//Cấu trúc dữ liệu chính
unordered_map<string, User> users;
unordered_map<int, Wallet> wallets;

int nextWalletId = 3; // Start from 3 (0 = admin, 1 = thay, 2 = co)

//Hàm tạo OTP ngẫu nhiên
string sinhOTP() {
    string otp = "";
    for (int i = 0; i < 6; i++) {
        otp += to_string(rand() % 10);
    }
    return otp;
}

//Hàm chuyển điểm
void chuyenDiem(string fromUser, string toUser, int amount) {
    auto sender = users.find(fromUser);
    auto receiver = users.find(toUser);

    if (sender == users.end() || receiver == users.end()) {
        cout << "Nguoi dung khong ton tai!\n";
        return;
    }

    int fromWalletId = sender->second.walletId;
    int toWalletId = receiver->second.walletId;

    auto walletAIt = wallets.find(fromWalletId);
    auto walletBIt = wallets.find(toWalletId);

    if (walletAIt == wallets.end() || walletBIt == wallets.end()) {
        cout << "Vi khong tim thay!\n";
        return;
    }

    Wallet& walletA = walletAIt->second;
    Wallet& walletB = walletBIt->second;

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

//Hàm xem thông tin ví
void xemVi(const string& username) {
    auto it = users.find(username);
    if (it == users.end()) {
        cout << "Nguoi dung khong tim thay!\n";
        return;
    }

    int walletId = it->second.walletId;
    auto walletIt = wallets.find(walletId);
    if (walletIt == wallets.end()) {
        cout << "Vi khong ton tai!\n";
        return;
    }

    Wallet& wallet = walletIt->second;
    cout << "ID Vi: " << wallet.walletId << "\n";
    cout << "So du: " << wallet.balance << " diem (" << wallet.balance * DONGS_PER_POINT << " dong)\n";
    cout << "Lich su giao dich:\n";
    for (const string& log : wallet.transactionLog) {
        cout << " - " << log << "\n";
    }
}

//Hàm đăng ký người dùng mới
void dangKyNguoiDung() {
    string newUsername;
    cout << "Nhap ten nguoi dung moi: ";
    cin >> newUsername;

    if (users.find(newUsername) != users.end()) {
        cout << "Ten nguoi dung da ton tai!\n";
        return;
    }

    int newWalletId = nextWalletId++;
    wallets[newWalletId] = Wallet(newWalletId, 0);
    users[newUsername] = User(newUsername, newWalletId);

    cout << "Dang ky thanh cong! Wallet ID: " << newWalletId << ", so du = 0 diem.\n";
}

//Khởi tạo hệ thống với 3 người dùng mẫu admin, duy, khang
void khoiTaoHeThong() {
    srand(static_cast<unsigned int>(time(0)));

    wallets[0] = Wallet(0, 10000);
    users["admin"] = User("admin", 0);

    wallets[1] = Wallet(1, 100);
    users["thay"] = User("thay", 1);

    wallets[2] = Wallet(2, 50);
    users["co"] = User("co", 2);
}

// Hàm main – Vòng lặp giao diện người dùng
int main() {
    khoiTaoHeThong();

    int luaChon;
    string username;

    while (true) {
        cout << "\n----- Menu -----\n";
        cout << "1. Xem thong tin vi\n";
        cout << "2. Chuyen diem\n";
        cout << "3. Dang ky nguoi dung moi\n";
        cout << "4. Thoat\n";
        cout << "Lua chon: ";
        cin >> luaChon;

        switch (luaChon) {
            case 1:
                cout << "Nhap ten dang nhap: ";
                cin >> username;
                xemVi(username);
                break;
            case 2: {
                string fromUser, toUser;
                int amount;
                cout << "Nhap ten nguoi chuyen: ";
                cin >> fromUser;
                cout << "Nhap ten nguoi nhan: ";
                cin >> toUser;
                cout << "Nhap so diem: ";
                cin >> amount;
                chuyenDiem(fromUser, toUser, amount);
                break;
            }
            case 3:
                dangKyNguoiDung();
                break;
            case 4:
                cout << "Tam biet!\n";
                return 0;
            default:
                cout << "Lua chon khong hop le!\n";
        }
    }

    return 0;
}
