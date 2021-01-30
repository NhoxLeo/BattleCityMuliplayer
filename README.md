# BattleCityMultiplayer
A Clone of Battle City with additional Multiplayer techniques


I: Hướng dẫn clone và build project
 1: Yêu cầu phần mềm:
 - Visual Studio 2019 latest update.
 - Microsoft DirectX SDK Jun 2010.
 - Visual C++ Redistributable for Visual Studio 2015
 2: Hướng dẫn Clone và Build:
 - Clone project về bằng github, select nhánh master và mở bằng Visual Studio
 - Sau khi mở bằng Visual Studio, thay đổi thông số configuration và môi trường thành Release và x86
 - Build game

II: Hướng dẫn sử dụng ( theo thứ tự đánh số ):
- 1: chạy file "BattleCityMuliplayer.exe" trong thư mục "BattleCityMultiplayer\Release"
- 2: đối với chương trình server, nhấn nút "Start Server" để bắt đầu chạy server
- 3: đối với chương trình Client, địa chỉ IP của mấy tính chứa chương trình server vào mục "Server Address", sau đó nhấn nút "Connect to Server" để kết nối vào server
     (lưu ý cổng mặc định là 8888 và nếu kết nối thành công thì số lượng packet gửi và nhận biểu thị ở thanh thông tin "Packet Sent" và "Packet Received" sẽ thay đổi liên tục )
- 4: sau khi có đủ 2 chương trình Client kết nối vào server thì dùng phìm mũi tên để di chuyển xuống mục "ON-LINE MODE", sau đó nhấn Enter để chọn
     (lưu ý các chương trinh trong cùng một máy tính sẽ dùng )
