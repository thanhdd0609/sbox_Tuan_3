#include <bits/stdc++.h>
#include "user.h"

using namespace std;

 main(){
    while(1){
        cout << "\n-------MENU------\n"
                "1.Dang ky\n"
                "2.Dang nhap\n"
                "3.Gui tin nhan\n"
                "4.Tin nhan\n"
                "5.Hien thi chi tiet tin nhan\n"
                "6.Them ban\n"
                "7.Danh sach ban be\n"
                "8.Block\n"
                "9.Dang xuat\n"
                "----------->SELECT:";

        char choose;
        cin >> choose;
        User u = User();
        switch(choose){
            case '1':
                u.DangKy();
                break;
            case '2':
                u.DangNhap();
                break;
            case '3':
                u.GuiTin(u.id);
                break;
            case '4':
                u.HienTin(u.id);
                break;
            case '5':
                u.HienTinChiTiet(u.id);
                break;
            case '6':
                u.ThemBan(u.id);
                break;
            case '7':
                u.HienBan(u.id);
                break;
            case '8':
                u.BlockBan(u.id);
                break;
            case '9':
                u.DangXuat();
                break;
            default:
                cout << "\nKhong chinh xac\n";
                break;
        }

    }

	return 0;
}
