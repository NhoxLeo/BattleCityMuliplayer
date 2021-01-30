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
(lưu ý: ngoại trừ mục "ON-LINE MODE" là mục chọn ngoài màn hình menu, các nút bấm còn lại được định vị trong khung cửa sổ màu xanh trong chương trình)

- Note: trong repository hiện có 2 nhánh là master sử dụng Input System Cũ và New_Input sử dụng Input System mới 
 + với Input Sytem cũ, khi nhấn một phím trên bàn phím thì các chương trình exe của project sẽ đều nhận input đó nếu chúng ở trong cùng một máy
 + với Input System mới, các chương trình sẽ nhận input từ bàn phím độc lập nhau, nhưng sẽ cần nhấp vào từng chương trinh để chương trình đó nhận input

- 1: chạy file "BattleCityMuliplayer.exe" trong thư mục "BattleCityMultiplayer\Release"
- 2: đối với chương trình server, nhấn nút "Start Server" để bắt đầu chạy server
- 3: đối với chương trình Client, địa chỉ IP của mấy tính chứa chương trình server vào mục "Server Address", sau đó nhấn nút "Connect to Server" để kết nối vào server
     (lưu ý cổng mặc định là 8888 và nếu kết nối thành công thì số lượng packet gửi và nhận biểu thị ở thanh thông tin "Packet Sent" và "Packet Received" sẽ thay đổi liên tục )
- 4: sau khi có đủ 2 chương trình Client kết nối vào server thì dùng phìm mũi tên để di chuyển xuống mục "ON-LINE MODE", sau đó nhấn Enter để chọn
    + Lưu ý:
     4.1: Các chương trình trong cùng một máy tính sẽ đều nhận phím enter nếu dùng Input System cũ, còn các chương trình dùng Input System mới sẽ nhận phím một cách độc lập,
          và để chuyển giữa các chương trình trong khi dùng Input System mới thì sẽ cần dùng chuột để chọn chúng.
- 5: Khi các chương trình ở màn hình Lobby,những chương trình nào là client thì nhấn nút "Client Ready" để chương trình server biết là client đã sẵn sàng, sau khi có 2 client đã           kết nối và sẵn sàng, chương trình server và client sẽ vào màn hình và bắt đầu game sau 5 giây ( lưu ý là chỉ nhấn nút "CLient Ready" sau khi đã vào màn hình lobby sau           khi chọn mục "ON-LINE MODE" ở menu )
- 6: Chơi game (lưu ý trong lúc chơi game thì khi nhân vật chết, có thể respawn nhân vật bằng nút "Start Game" ở chương trình Server).


## ScreenShot tham khảo:
1. Visual studio config
<img src="https://github.com/NhoxLeo/BattleCityMultiplayer/blob/master/Screenshot/BuildConfig.JPG" width="400" height="100"/>
2. Menu game
<img src="https://github.com/NhoxLeo/BattleCityMultiplayer/blob/master/Screenshot/menu.JPG" width="600" height="600"/>
3. các nút bắm của server và client trong màn hình lobby
<img src="https://github.com/NhoxLeo/BattleCityMultiplayer/blob/master/Screenshot/one_client_ready.JPG" width="800" height="200"/>
4. màn hình menu của server và client sau khi đã kết nối
<img src="https://github.com/NhoxLeo/BattleCityMultiplayer/blob/master/Screenshot/server_and_client.JPG" width="600" height="200"/>
5. màn hình ingame của server
<img src="https://github.com/NhoxLeo/BattleCityMultiplayer/blob/master/Screenshot/servermenu.JPG" width="600" height="600"/>
