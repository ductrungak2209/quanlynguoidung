# CHƯƠNG TRÌNH QUẢN LÍ VÍ ĐIỆN TỬ

## I. Giới thiệu dự án

- Ứng dụng ví điện tử này được phát triển bằng ngôn ngữ lập trình C++ với mục tiêu mô phỏng các chức năng cơ bản của một ví điện tử. Người dùng có thể thực hiện nhiều thao tác như tạo tài khoản, đăng nhập, nạp và chuyển điểm, xác minh OTP, thay đổi mật khẩu, cập nhật thông tin cá nhân, cũng như tra cứu lịch sử giao dịch.
- Tất cả dữ liệu được lưu trữ cục bộ trong các tệp văn bản như users.txt và wallets.txt, đồng thời có cơ chế sao lưu tự động vào các tệp dự phòng backup_users.txt và backup_wallets.txt. 
- Ví có các chức năng và phân quyền cho người dùng và admin với các chức năng và giao diện khác nhau.

## II. Giới thiệu thành viên và phân công công việc

1. **Họ tên**: Lê Anh Đức
   - **Vai trò**: Nhóm trưởng, Lập trình viên
   - **Công việc thực hiện**:
     - Thiết kế cấu trúc cốt lõi của hệ thống, bao gồm định nghĩa các lớp Wallet và User.
     - Lập trình chức năng đăng ký tài khoản, bao gồm xác thực OTP và tạo tài khoản mới.
     - Tham gia phát triển chức năng chuyển điểm, bao gồm kiểm tra người dùng và ví.
     - Quản lý nhóm, phân công công việc.

2. **Họ tên**: Vũ Hoàng Khang, Tuấn Kha
   - **Vai trò**: Lập trình viên
   - **Công việc thực hiện**:
     - Triển khai lưu trữ dữ liệu vào các tệp users.txt, wallets.txt và sao lưu vào backup_users.txt, backup_wallets.txt.
     - Lập trình chức năng đăng nhập, đổi mật khẩu và một phần của cập nhật thông tin cá nhân.
     - Hoàn thiện chức năng chuyển điểm, xem báo cáo ví, và xem danh sách người dùng dành cho quản lý.
     - Viết báo cáo dự án.
       
3. **Họ tên**: Pham Khánh Duy, Nguyễn Anh Tuấn
   - **Vai trò**: Lập trình viên
   - **Công việc thực hiện**:
     - Phát triển các hàm tiện ích bảo mật: mã hóa mật khẩu, sinh mật khẩu ngẫu nhiên, và sinh OTP.
     - Triển khai hàm tải dữ liệu từ tệp chính hoặc tệp sao lưu.
     - Lập trình chức năng nạp điểm, bao gồm xử lý quy đổi tiền và xác thực OTP.
     - Kiểm tra chất lượng mã nguồn.

## III. Đặc tả chức năng

### Chương trình bao gồm các chức năng sau:

1. **Đăng ký tài khoản**: 
   - Nhập tên đăng nhập, họ tên, email, mật khẩu 

2. **Đăng nhập**: 
   - Kiểm tra tên đăng nhập và mật khẩu, cho phép truy cập hệ thống. 
   - Tài khoản tự sinh yêu cầu đổi mật khẩu lần đầu.

3. **Nạp điểm**: 
   - Người dùng nhập số tiền và xác thực OTP để nạp điểm vào ví.

4. **Chuyển điểm**: 
   - Chuyển điểm đến tài khoản khác, yêu cầu xác thực OTP và kiểm tra số dư.

5. **Đổi mật khẩu**: 
   - Thay đổi mật khẩu sau khi xác thực OTP.

6. **Đổi mật khẩu**: 
   - Thay đổi mật khểu người dùng, yêu cầu OTP. 
   - Quản lý có thể cập nhật thông tin người dùng khác.

7. **Xem ví**: 
   - Hiển thị số dư và lịch sử giao dịch của ví.

8. **Xem danh sách người dùng**: 
   - Chỉ dành cho quản lý, hiển thị thông tin tất cả người dùng.

9. **Lưu dữ liệu**: 
   - Tự động lưu thông tin người dùng và ví vào tệp văn bản, với sao lưu tự động.

10. **Xác thực OTP**: 
    - Tạo mã OTP 6 chữ số để bảo mật đăng ký, giao dịch, và cập nhật thông tin.

### Cấu trúc file dữ liệu

- **users.txt**: Lưu thông tin người dùng (tên đăng nhập, mật khẩu mã hóa, họ tên, email, ID ví, trạng thái quản lý, trạng thái tự sinh).
- **wallets.txt**: Lưu thông tin ví (ID ví, số dư, lịch sử giao dịch).
- **backup_users.txt**: Sao lưu dữ liệu người dùng.
- **backup_wallets.txt**: Sao lưu dữ liệu ví.

## IV. Hướng dẫn tải và biên dịch chương trình

- **Bước 1**: Tải mã nguồn từ kho lưu trữ này hoặc từ tệp main.cpp đã được chia sẻ.
- **Bước 2**: Mở Visual Studio Code (hoặc bất kỳ IDE nào hỗ trợ C++).
- **Bước 3**: Biên dịch chương trình bằng trình biên dịch g++ (hoặc công cụ tương đương).  
  Ví dụ dòng lệnh: `g++ main.cpp -o main.exe`
- **Bước 4**: Đảm bảo các tệp dữ liệu (users.txt, wallets.txt, backup_users.txt, backup_wallets.txt) nằm trong cùng thư mục với tệp thực thi.

## V. Hướng dẫn sử dụng chương trình

1. Mở Visual Studio Code (hoặc bất kỳ IDE nào hỗ trợ C++).
2. Chạy chương trình: `./main` (Linux) hoặc `main.exe` (Windows).
3. Làm theo hướng dẫn hiển thị trên màn hình để:
   - **Đăng ký tài khoản**: Nhập thông tin.
   - **Đăng nhập tài khoản**: Yêu cầu đổi mật khẩu nếu tài khoản tự sinh.
   - **Nạp điểm**: Nhập số điểm, xác thực OTP.
   - **Chuyển điểm**: Nhập người nhận, số điểm, xác thực OTP.
   - **Đổi mật khẩu**: Xác thực OTP.
   - **Xem báo cáo ví**: Xem số dư, lịch sử giao dịch.
   - **Quản lý** (nếu là quản lý): Đăng ký hộ người dùng, cập nhật thông tin người dùng khác, xem danh sách người dùng.
   - **Thoát chương trình**: Dữ liệu sẽ tự động lưu vào users.txt, wallets.txt và sao lưu vào backup_users.txt, backup_wallets.txt.

## VI. Mô tả chi tiết cách chương trình hoạt động

Khi khởi chạy chương trình, hệ thống tải dữ liệu từ users.txt và wallets.txt. Menu chính hiển thị các tùy chọn sau:

1. **Đăng ký tài khoản**:
   - Người dùng nhập tên đăng nhập, họ tên, email, và mật khẩu.
   - Quản lý có thể đăng ký tài khoản với mật khẩu tự sinh, được đánh dấu là tài khoản tự sinh.
   - Thông tin được lưu vào users.txt, ví mới được tạo và lưu vào wallets.txt.

2. **Đăng nhập hệ thống**:
   - Nhập tên đăng nhập và mật khẩu. Mật khẩu được mã hóa và so sánh với dữ liệu lưu trữ.
   - Nếu là tài khoản tự sinh, yêu cầu đổi mật khẩu ngay lần đăng nhập đầu.
   - Nếu thông tin sai, báo lỗi.

3. **Sau khi đăng nhập**, menu cung cấp các lựa chọn:
   - **Nạp điểm**:
     - Người dùng nhập số tiền
     - Xác thực OTP để hoàn tất.
     - Điểm được cộng vào ví, lịch sử giao dịch cập nhật trong wallets.txt.

   - **Chuyển điểm**:
     - Nhập tên tài khoản người nhận và số điểm.
     - Kiểm tra số dư, yêu cầu xác thực OTP.
     - Trừ điểm từ ví người gửi, cộng điểm cho người nhận, cập nhật lịch sử giao dịch.

   - **Đổi mật khẩu**:
     - Yêu cầu xác thực OTP, nhập mật khẩu mới.
     - Mật khẩu được mã hóa và cập nhật trong users.txt.

   - **Xem ví**:
     - Hiển thị ID ví, số dư (điểm và đô la tương đương), và lịch sử giao dịch.

   - **Xem danh sách người dùng** (chỉ quản lý):
     - Hiển thị thông tin tất cả người dùng: tên đăng nhập, họ tên, email, ID ví, trạng thái quản lý.

   - **Thoát chương trình**:
     - Lưu toàn bộ dữ liệu vào users.txt, wallets.txt.
     - Tạo bản sao lưu vào backup_users.txt, backup_wallets.txt bằng lệnh hệ thống (copy trên Windows, cp trên Unix).


## VII. Tài liệu tham khảo

- Tài liệu C++ từ trang [cppreference.com](https://en.cppreference.com/w/).
- Diễn đàn thảo luận [StackOverflow](https://stackoverflow.com/).
- Trang tài liệu học lập trình: [GeeksforGeeks](https://www.geeksforgeeks.org/cpp-programming-language/).
- Tài liệu về quản lý tệp và bảo mật trong C++: [cplusplus.com](https://www.cplusplus.com/).
