#include <iostream>
#include "sqlite3.h"
#include<bits/stdc++.h>


using namespace std;
class User{
	public:
		int  id;
		char *tennguoidung;
		char *matkhau;
		char *ngaysinh;
		char *gioitinh;
		char *tendaydu;
		char *diachi;
		int flag ; 

	public :
        User(){};
		void DangKy();
        void DangNhap();
        bool isDangNhap();
        void DangXuat();
        bool Check(User u);
        void HienTin(int);
        void HienTinChiTiet(int);
        void GuiTin(int);
        void ThemBan(int);
        void HienBan(int);
        void BlockBan(int);

};

class ConnectSQlite{
    public:
        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;
        const char *szSQL;
        const char *pzTest;
        sqlite3_stmt *stmt;
        list<string> answer;
    public :
        static int callback(void *,int,char **,char **);
        void Open();
        //MODULE 1
        void RegisterSQL(sqlite3 *db,char *, char *,char *,char *,char *,char *);
        int LoginSQL(sqlite3 *db, char *tennguoidung, char *matkhau);
        int TransNameToId(char *fr);
        bool CheckAccount(sqlite3 *db,char *tennguoidung);
        //MODULE 2
        void ChonTin(sqlite3 *db, int id);
        void HienTinChiTiet(sqlite3 *db,int id1,int id2);
        void SoanTin(sqlite3 *db,int id1, int id2,char *noidung,char *thoigian,char *tieude);
         //MODULE 3
        bool CheckBan(sqlite3 *db, int id1,int id2);
        void VietTenBan(sqlite3 *db,int id1, int id2);
        void HienBan(sqlite3 *db,int id1);
        void BlockBan(sqlite3 *db,int id1, int id2);
        bool CheckBlock(sqlite3 *db,int id1, int id2);

};

