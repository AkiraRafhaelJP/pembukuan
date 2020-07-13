#include<iostream>
#include<fstream>
#include<ctime>
#include<utility>
#include<map>
#include<string>
#include<cstring>
#include<sstream>
#include<iomanip>
#include<limits>
using namespace std;

void inputBelanjaanJasa();
void writeToHistory(map<string, int> belanjaan);
void totalDay();
void inputTanggal(int tanggal[]);
void totalMonth();
const string W_DAY[] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
const string MONTH[] = {"Januari", "Februari", "Maret", "April", "Mei", "Juni",
                        "Juli", "Agustus", "September", "Oktober", "November", "Desember"};

int main(){
    bool isOver = false;
    do{
        short pilihan;
        cout << "===APLIKASI PEMBUKUAN===\n";
        cout << "1. Input belanjaan hari ini\n";
        cout << "2. Liat total hari\n";
        cout << "3. Liat total bulan\n";
        cout << "4. Exit\n";
        cout << "Pilihan : ";
        cin >> pilihan;
        switch (pilihan){
            case 1:
                inputBelanjaanJasa();
                break;

            case 2:
                totalDay();
                break;

            case 3:
                totalMonth();
                break;

            case 4:
                isOver = true;
                break;
            default:
                cout << "Pilihan anda tidak valid, coba lagi\n";
        }
        system("pause");
        system("cls");
    } while(!isOver);
    cout << "===Tekan apapun untuk menutup===\n";
    cin.get();
    return 0;
}

void inputBelanjaanJasa(){
    short banyakBelanjaan;
    map<string, int> belanjaan;
    cout << "Banyak belanjaan / jasa : ";
    cin >> banyakBelanjaan;
    for(int i = 0; i < banyakBelanjaan; i++){
        string namaBarangJasa;
        int hargaBarangJasa;
        cout << "Belanjaan / jasa ke-" << i + 1 << ": ";
        cin >> namaBarangJasa;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Harga : ";
        cin >> hargaBarangJasa;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        belanjaan[namaBarangJasa] = hargaBarangJasa;
    }
    writeToHistory(belanjaan);
    cout << "Berhasil dimasukkan ke daftar history\n";
}

void writeToHistory(map<string, int> belanjaan){
    //variabel total digunakan untuk menampung
    //harga total semua belanjaan pada objek map
    //belanjaan yang telah di-pass dari fungsi inputBelanjaan()
    int total = 0;

    //membuat objek stream ke file history.txt
    fstream file;

    //mengambil waktu sistem dan kemudian dikonversi menjadi
    //objek tm untuk mendapatkan bentuk yang dapat diolah
    time_t now = time(0);
    tm *date = localtime(&now);

    //iterator untuk mengakses map belanjaan yang berisikan 
    //nama barang/jasa dan harganya
    map<string, int>::iterator it_belanjaan = belanjaan.begin();

    try{
        //file history.txt dibuka dengan mode append
        file.open("history.txt", ios::app);

        //pengecekan jika file berhasil dibuka
        // if(file.is_open()){
        //     cout << "File history berhasil dibuka\n";
        // }

        while(it_belanjaan != belanjaan.end()){
            //output untuk nama dan harga
            file << it_belanjaan->first << '#' << it_belanjaan->second << '#';

            //output untuk tanggal update
            file << date->tm_mday << '-' << date->tm_mon << '-' << date->tm_year << '\n';
            it_belanjaan++;
        }
    }
    catch(const exception& e){
        cerr << e.what() << '\n';
    }
    file.close();
}

void totalDay(){
    //objek ifstream untuk membaca file
    ifstream file;

    //cek apakah tanggal yang diinputkan ada atau tidak
    bool check = false;

    //line digunakan untuk menampung string pada file per baris
    string line;

    //mengambil waktu sistem dan kemudian dikonversi menjadi
    //objek tm untuk mendapatkan bentuk yang dapat diolah
    time_t now = time(0);
    tm *date = localtime(&now);

    //no untuk memberikan nomor pada output barang dan jasa, total begitulah (self-explanatory)
    int no = 1, total = 0;

    //array untuk menyimpan tanggal hari, bulan, dan tahun
    int tanggal[3];

    //file history dibuka dengan mode membaca
    file.open("history.txt", ios::in);
    
    //pemanggilan fungsi untuk input tanggal
    inputTanggal(tanggal);

    //target digunakan untuk menampung tanggal yang diinputkan pengguna pada fungsi sebelumnya dalam 
    //bentuk string
    string target = to_string(tanggal[0]) + "-" + to_string(tanggal[1]) + "-" + to_string(tanggal[2]);

    cout << tanggal[0] << " " << MONTH[tanggal[1] - 1] << " " << tanggal[2] << '\n';

    // cout << file.good() << '\n';
    
    //pembacaan file per baris
    while(getline(file, line)){
        //cek apakah tanggal terdapat pada baris yang sedang dicek
        if(line.find(target) != string::npos){
            check = true;
            string nama_barang_jasa = "";
            string harga_barang_jasa = "";
            cout << no++ << ". ";

            //variabel count digunakan untuk menghitung karakter '#'
            int count = 0;

            for(int i = 0; i < line.length(); i++){
                if(line[i] == '#') count++, i++;

                //format
                //<nama barang/jasa>-<harga>-<tanggal>
                if(count == 0) nama_barang_jasa += line[i];
                else if(count == 1) harga_barang_jasa += line[i];
                else break;
            }
            cout << left << setw(30) << nama_barang_jasa << " Rp. " << right << setw(6) << harga_barang_jasa << "\n";
            total += stoi(harga_barang_jasa.c_str());
        }
    }
    if(check){
        cout << setfill('-') << setw(46) << "+\n";
        cout << setfill(' ') << left << setw(32) << "Total" << ": Rp. " << right << setw(6) << total << '\n';
    } else{
        cout << "Data pada tanggal tersebut tidak ditemukan\n";
    }
    
    file.close();
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void inputTanggal(int tanggal[]){
    bool check[3];
    memset(check, 0, 3);
    do{
        cout << "Masukkan tanggal (1-31): ";
        cin >> tanggal[0];
        cin.ignore();
        if(tanggal[0] >= 1 && tanggal[0] <= 31)
            check[0] = true;
        else 
            cout << "Tanggal tidak valid, isi dengan tanggal lain!\n\n";
    }while(!check[0]);
    
    do{
        cout << "Masukkan bulan (1-12): ";
        cin >> tanggal[1];
        cin.ignore();
        if(tanggal[1] >= 1 && tanggal[1] <= 12)
            check[1] = true;
        else
            cout << "Bulan tidak valid, isi dengan bulan lain!\n\n";
    }while(!check[1]);

    do{
        cout << "Masukkan tahun (1970-sekarang): ";
        cin >> tanggal[2];
        if(tanggal[2] >= 1970)
            check[2] = true;
        else
            cout << "Tahun tidak valid, isi dengan tahun lain!\n\n";
    }while(!check[2]);
        
    cin.ignore();
    cout << "Input tanggal berhasil!\n\n";
}

void totalMonth(){

}