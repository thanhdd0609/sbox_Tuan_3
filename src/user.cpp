#include <iostream>
#include "user.h"
#include<bits/stdc++.h>

using namespace std;

// Connect to SQLite
int ConnectSQlite::callback(void *NotUsed, int argc, char **argv, char **azColName){
           int i;
           for(i=0; i<argc; i++){
              printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
           }
           printf("\n");
           return 0;
        }
void ConnectSQlite::Open(){
        rc = sqlite3_open("tinnhan.db", &db);
       if( rc ){
          fprintf(stderr, "Khong the ket noi toi SQLite: %s\n", sqlite3_errmsg(db));
          exit(0);
       }else{

       }
}
void User::DangKy(){
    tennguoidung = new (char);
    matkhau = new char;
    tendaydu = new char;
    ngaysinh = new char;
    gioitinh = new char;
    diachi = new char;

    cout << "Ten Dang Nhap: ";
	cin >> tennguoidung;
	cout << "Mat Khau: ";
	cin >> matkhau;
	cout << "Ten Day Du: ";
	cin.ignore(1);
    cin.getline(tendaydu,30);
	cout << "Ngay Sinh: ";
	cin >> ngaysinh;
	cout << "Gioi Tinh: ";
	cin >> gioitinh;
	cout << "Dia Chi: ";
	cin.ignore(1);
    cin.getline(diachi,30);
	ConnectSQlite conn ;
	conn.Open();
    conn.RegisterSQL(conn.db,tennguoidung,matkhau,tendaydu,ngaysinh,gioitinh,diachi);

}
//Thong tin dang ky
void ConnectSQlite::RegisterSQL(sqlite3 *db, char *tennguoidung, char *matkhau,char *tendaydu,char *ngaysinh, char *gioitinh, char *diachi){
  if (!db)return;
  szSQL = "INSERT INTO user (tennguoidung, matkhau,tendaydu,ngaysinh,gioitinh,diachi) values (?,?,?,?,?,?)";

  int rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);

  if( rc == SQLITE_OK ) {
    // bind the value
    sqlite3_bind_text(stmt, 1, tennguoidung, strlen(tennguoidung), 0);
    sqlite3_bind_text(stmt, 2, matkhau, strlen(matkhau), 0);
    sqlite3_bind_text(stmt, 3, tendaydu, strlen(tendaydu), 0);
    sqlite3_bind_text(stmt, 4, ngaysinh, strlen(ngaysinh), 0);
    sqlite3_bind_text(stmt, 5, gioitinh, strlen(gioitinh), 0);
    sqlite3_bind_text(stmt, 6, diachi, strlen(diachi), 0);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  }else{

      fprintf(stderr, "Loi SQL: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);

  }
}
void  User::DangNhap(){
    tennguoidung = new char;
    matkhau = new char;
    cout << "Ten Dang Nhap: ";
	cin >> tennguoidung;
	cout << "Mat Khau: ";
	cin >> matkhau;
	ConnectSQlite conn ;
	conn.Open();

	if(conn.LoginSQL(conn.db,tennguoidung,matkhau) > 0) {
        cout << "Dang nhap thanh cong!\n";
        id = conn.LoginSQL(conn.db,tennguoidung,matkhau);
        flag = 1;
	}
	else {
        cout << "Dang nhap that bai!\n";
        flag = 0;
	}
}

//Dang nhap voi thong tin tu SQLite
int ConnectSQlite::LoginSQL(sqlite3 *db,char *tennguoidung, char *matkhau){
        int id = 0;
        if (!db)return 0;
        szSQL = "SELECT * FROM  user WHERE tennguoidung = ? and matkhau = ? ";
        rc = sqlite3_prepare(db, szSQL, strlen(szSQL), &stmt, &pzTest);
       if( rc == SQLITE_OK ) {
            
            sqlite3_bind_text(stmt, 1, tennguoidung, strlen(tennguoidung), 0);
            sqlite3_bind_text(stmt, 2, matkhau, strlen(matkhau), 0);
            
           while(sqlite3_step(stmt) == SQLITE_ROW) {
                stringstream str;
                str << sqlite3_column_text(stmt, 0);
                str >> id ;
            }
            sqlite3_finalize(stmt);


      }else{

          fprintf(stderr, "Loi SQL: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);
      }
    return id;

}

bool User::isDangNhap(){
    ConnectSQlite conn;
    if(flag == 1) return true;
    return false;
}

void User::DangXuat(){
    flag = 0;
    cout << "------Dang Xuat Thanh Cong-------\n";
}

void User::ThemBan(int id){
    if (isDangNhap()){
        cout << "Nhap ten nguoi dung: ";
        char *newfr;
        newfr = new char;
        cin >> newfr;
        ConnectSQlite conn;
        conn.Open();
        int id2 = conn.TransNameToId(newfr);
        if(id2 > 0){
            if(conn.CheckBlock(conn.db,id,id2) && !conn.CheckBan(conn.db,id,id2) ){
                conn.VietTenBan(conn.db,id,id2);
                cout<<"Thanh cong\n";
            }else {
                cout << "\t Da chan / Ban \n";
                conn.CheckFriend(conn.db,id,id2);
            }
        }else cout <<  "\nTai khoan khong ton tai \n";
    }else {
        cout << "\nMoi dang nhap.\n";
    }

}

void ConnectSQlite::VietTenBan(sqlite3 *db,int id1, int id2){
    if (!db)return ;
     szSQL = "INSERT INTO friend VALUES (?,?,?)";
     rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
     if( rc == SQLITE_OK ) {
     
        int block = 0;
        sqlite3_bind_int(stmt,1,id1);
        sqlite3_bind_int(stmt,2,id2);
        sqlite3_bind_int(stmt,3,block);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

    }else{

          fprintf(stderr, "Loi SQL: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);

      }
}
void User::HienBan(int id){
    if (isLogIn()){
        ConnectSQlite conn;
        conn.Open();
        conn.ShowFriend(conn.db,id);
    }else {
        cout << "\nXin moi dang nhap.\n";
    }
}

void User::BlockBan(int id1){
    if (isLogIn()){
        cout <<"\nBlock nguoi dung :\n";
        char *name;
        name = new char;
        cin >> name;
        ConnectSQlite conn;
        conn.Open();
        int id2 = conn.TransNameToId(name);
        if(id2 > 0){
            conn.BlockBan(conn.db,id1,id2);
            cout <<"\nBlock Thanh Cong\n";
            conn.ShowFriend(conn.db,id1);
        } else cout <<  "\nTai khoan khong ton tai.\n";
    }else {
        cout << "\nXin moi dang nhap\n";
    }
}

int ConnectSQlite::TransNameToId(char *fr){
    int id2 = -1;
    szSQL = "SELECT id FROM user WHERE tennguoidung = ?";
    rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);

    if( rc == SQLITE_OK ){
        sqlite3_bind_text(stmt,1,fr,strlen(fr),0);
        if(sqlite3_step(stmt) == SQLITE_ROW) {
            stringstream str;
            str << sqlite3_column_text(stmt, 0);
            str >> id2 ;

        }
        sqlite3_finalize(stmt);
    }else{
          fprintf(stderr, "Loi SQL: %s\n",zErrMsg);
          sqlite3_free(zErrMsg);
    }
    return id2;

}
bool ConnectSQlite::CheckBan(sqlite3 *db, int id1,int id2){
    int isFriend = 0;
    szSQL = "SELECT * FROM friend WHERE ( id1 = ? AND id2 = ?) OR ( id1 = ? AND id2 = ?)";
    rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
      if( rc == SQLITE_OK ) {

        sqlite3_bind_int(stmt,1,id1);
        sqlite3_bind_int(stmt,2,id2);
        sqlite3_bind_int(stmt,3,id2);
        sqlite3_bind_int(stmt,4,id1);

       if(sqlite3_step(stmt) == SQLITE_ROW ){
           stringstream str;
            str << sqlite3_column_text(stmt, 0);
            str >> isFriend;

            if(isFriend != 0)
                isFriend = 1;
            else isFriend = 0;
            sqlite3_finalize(stmt);
        }
    }else{
          fprintf(stderr, "Loi SQL: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);

      }
      return isFriend;
}

void ConnectSQlite::HienBan(sqlite3 *db,int id1){
    cout << "Danh sach ban be:\n";
    if (!db)return ;
    szSQL = "SELECT DISTINCT user.tennguoidung FROM"
            "(SELECT * FROM friend where ((id1 = ? OR id2 = ?) AND block = 0)) as A "
              "LEFT JOIN user ON (A.id2 = user.id OR A.id1 = user.id)";
    rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
    if( rc == SQLITE_OK ) {

        sqlite3_bind_int(stmt,1,id1);
        sqlite3_bind_int(stmt,1,id1);

        while(sqlite3_step(stmt) == SQLITE_ROW) {
           cout <<"- ";
           cout<< string((const char *) sqlite3_column_text(stmt, 0)) << endl;
        }
        sqlite3_finalize(stmt);
            cout <<"\n";
    }else{

          fprintf(stderr, "Loi SQL: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);

      }
}
void ConnectSQlite::BlockBan(sqlite3 *db,int id1, int id2){
     if (!db)return ;
     szSQL = "UPDATE friend SET block = 1 WHERE id1 = ? AND id2 = ?";
     rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
     if( rc == SQLITE_OK ) {

        sqlite3_bind_int(stmt,1,id1);
        sqlite3_bind_int(stmt,2,id2);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

    }else{

          fprintf(stderr, "Loi SQL: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);

      }
}
bool ConnectSQlite::CheckBlock(sqlite3 *db,int id1, int id2){
    int isblock = 0;
     if (!db)return 0;
     szSQL = "SELECT block FROM friend WHERE (id1 = ? AND id2 = ?) OR (id1 = ? AND id2 = ?)";
     rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
      if( rc == SQLITE_OK ) {

        sqlite3_bind_int(stmt,1,id1);
        sqlite3_bind_int(stmt,2,id2);
        sqlite3_bind_int(stmt,3,id2);
        sqlite3_bind_int(stmt,4,id1);

       if(sqlite3_step(stmt) == SQLITE_ROW ){
           stringstream str;
            str << sqlite3_column_text(stmt, 0);
            str >> isblock ;
            sqlite3_finalize(stmt);
        }
    }else{

          fprintf(stderr, "Loi SQL: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);

      }
      return !isblock;
}
void User::GuiTin(int id){
    if (isDangNhap()){
        int id2;
        cout << "Gui Toi: ";
        char *frien;
        frien = new char;
        cin >> frien;
        ConnectSQlite conn;
        conn.Open();
        id2 = conn.TransNameToId(frien);
        if(id2 > 0){
            char *mess,*dt,*title;
            mess = new char;
            dt =new char;
            title= new char;
            if(!conn.CheckBlock(conn.db,id,id2)){
                        cout << "Da chan\n";
            }else{
                cout << "Tieu de:";
                cin.ignore(1);
                cin.getline(tieude,10);
                cout << "Noi Dung:\n";
                cin.getline(mess,120);
                 time_t hientai = thoigian(0);
                 dt = ctime(&hientai);
                 conn.SoanTin(conn.db,id,id2,mess,dt,tieude);
                 cout<<"GUI\n";

            }
        }else cout <<  "Tai khoan khong ton tai\n";

    }else {
        cout << "Moi dang nhap\n\n";
    }
}
void ConnectSQlite::SoanTin(sqlite3 *db,int id1, int id2,char *noidung,char *thoigian,char *tieude){
         szSQL = "INSERT INTO message VALUES (?,?,?,?,?)";
         rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
         if( rc == SQLITE_OK ) {
                
            sqlite3_bind_int(stmt,1,id1);
            sqlite3_bind_int(stmt,2,id2);
            sqlite3_bind_text(stmt,3,noidung,strlen(noidung),0);
            sqlite3_bind_text(stmt,4,thoigian,strlen(thoigian),0);
            sqlite3_bind_text(stmt,5,tieude,strlen(tieude),0);
            
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);

        }else{

              fprintf(stderr, "Loi SQL: %s\n", zErrMsg);
              sqlite3_free(zErrMsg);
              
          }

}
void User::HienTin(int id){
    if (isDangNhap()){
        ConnectSQlite conn;
        conn.Open();
        conn.ChonTin(conn.db,id);
    }else {
        cout << "Moi dang nhap\n\n";
    }
}
void ConnectSQlite::ChonTin(sqlite3 *db, int id){
    if (!db)return ;
    szSQL ="SELECT DISTINCT tieude FROM"
                "(SELECT * FROM message where idsen = ? ) as A LEFT JOIN user ON A.idrec = user.id";
    rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
    if( rc == SQLITE_OK ) {
       
        sqlite3_bind_int(stmt,1,id);

        
        while(sqlite3_step(stmt) == SQLITE_ROW) {
            answer.push_back(string((const char *) sqlite3_column_text(stmt, 0)));
        }
        sqlite3_finalize(stmt);
        cout <<"Danh sach tin nhan da gui\n" ;
        for( list<std::string>::const_iterator it = answer.begin(); it != answer.end(); it++)

            cout << "-Title:"<<*it << endl;

    }else{

          fprintf(stderr, "SQL error: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);
          
      }
}
void ConnectSQlite::HienTinChiTiet(sqlite3 *db,int id1,int id2){
    if (!db)return ;
    cout << "Chi tiet:\n";
    szSQL = "SELECT * FROM (SELECT * FROM message WHERE (idsen = ? OR idsen = ?) AND (idrec = ? OR idrec =?) ) AS B LEFT JOIN  user ON user.id = B.idsen";
    rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
    if( rc == SQLITE_OK ) {
            
        sqlite3_bind_int(stmt,1,id1);
        sqlite3_bind_int(stmt,2,id2);
        sqlite3_bind_int(stmt,3,id1);
        sqlite3_bind_int(stmt,4,id2);
        
        while(sqlite3_step(stmt) == SQLITE_ROW) {

                cout << " Gui toi: " << string((const char *) sqlite3_column_text(stmt, 5)) << "" <<  endl;
                cout << " Noi dung: "<< string((const char *) sqlite3_column_text(stmt, 2)) << "\n" ;
                cout << " Thoi gian: "<<string((const char *) sqlite3_column_text(stmt, 3)) << endl ;

        }
        sqlite3_finalize(stmt);

    }else{

          fprintf(stderr, "Loi SQL: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);
          
      }
}
void User::HienTinCHiTiet(int id1){
    if (isDangNhap()){
        int id2;
        cout << "Nhap ten nguoi dung: ";
        char *frien;
        frien = new char;
        cin >> frien;
        ConnectSQlite conn;
        conn.Open();
        id2 = conn.TransNameToId(frien);

        if(id2 > 0)
        conn.ShowMessDetail(conn.db,id1,id2);
        else cout <<  "Tai khoan khong ton tai\n";
    }else {
        cout << "Moi dang nhap\n";
    }

}
