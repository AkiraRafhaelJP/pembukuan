#include<iostream>
#include<fstream>
#include<ctime>
#include<utility>
#include<map>
#include<string>
#include<cstring>
#include<sstream>
#include<iomanip>
#include<regex>
using namespace std;

//fungsi untuk menginputkan item atau banyaknya belanjaan atau jasa
//pada hari penginputtan
void inputItem();

//fungsi untuk menuliskan item yang telah diinputkan pada fungsi inputItem()
//pada file history.txt
void writeToHistory(map<string, int> item);

//fungsi untuk menampilkan daftar item dengan harga
//tiap item pada tanggal tertentu
void totalDay();

//fungsi untuk menginputkan tanggal sekaligus validasi pada fungsi totalDay()
void inputTanggal(int tanggal[]);

//fungsi untuk menampilkan daftar item dengan harga
//tiap item pada bulan tertentu, belum dibuat
void totalMonth();

//fungsi untuk menginputkan bulan dan tahun pada fungsi totalMonth()
void inputTanggal2(int tanggal[]);

//fungsi untuk validasi input integer(harga, pilihan, banyak item)
bool validasiInputInteger(string str);

//digunakan untuk memodifikasi output
//W_DAY belum dipakai
const string W_DAY[] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
const string MONTH[] = {"Januari", "Februari", "Maret", "April", "Mei", "Juni",
                        "Juli", "Agustus", "September", "Oktober", "November", "Desember"};

//variabel string global untuk menampung input integer sementara 
//yang akan digunakan untuk validasi integer
string temp;

int main(){
    //variabel isOver digunakan untuk menampung status 
    //program apakah masih berlangsung atau tidak
    //ketika pengguna memilih opsi pada awal program
    bool isOver = false;

    //tampilan utama program
    do{
        short pilihan;
        cout << "===APLIKASI PEMBUKUAN===\n";
        cout << "1. Input item hari ini\n";
        cout << "2. Liat total hari\n";
        cout << "3. Liat total bulan\n";
        cout << "4. Exit\n";
        cout << "Pilihan : ";
        cin >> temp;

        if(!validasiInputInteger(temp)){
            cout << "Input pilihan tidak valid, masukkan angka bulat!\n";
            system("pause");
            system("cls");
            continue;
        }
        
        pilihan = stoi(temp.c_str());

        switch (pilihan){
            case 1: // opsi no.1
                inputItem();
                break;

            case 2: // opsi no.2
                totalDay();
                break;

            case 3: // opsi no.3
                totalMonth();
                break;

            case 4: // opsi no.4
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

void inputItem(){
    //variabel untuk menampung jumlah banyak item atau data yang
    //ingin dimasukkan
    short banyakItem;

    cout << "Banyak item : ";
    cin >> temp;

    //validasi input banyak item
    //jika input valid, maka input dikonversi menjadi integer
    if(!validasiInputInteger(temp)){
        cout << "Input banyak item tidak valid, masukkan angka bulat!" << endl;
        inputItem();
        return;
    }
    banyakItem = stoi(temp.c_str());

    //variabel map yang digunakan untuk menampung nama item sebagai
    //kunci dan harga sebagai value
    map<string, int> item;

    //kode untuk input nama item dan harga
    for(int i = 0; i < banyakItem; i++){
        string namaItem;
        int hargaItem;
        cout << "Item ke-" << i + 1 << ": ";
        getline(cin >> ws, namaItem);
        cout << "Harga : ";
        
        //validasi input harga
        //jika input valid, maka input dikonversi menjadi integer
        cin >> temp;
        if(!validasiInputInteger(temp)){
            cout << "Input harga tidak valid, masukkan angka bulat!" << endl;
            return;
        }
        hargaItem = stoi(temp.c_str());

        item[namaItem] = hargaItem;
    }
    writeToHistory(item);
    cout << "Berhasil dimasukkan ke daftar history\n";
}

void writeToHistory(map<string, int> item){
    //variabel total digunakan untuk menampung
    //harga total semua item pada objek map
    //item yang telah di-pass dari fungsi inputitem()
    int total = 0;

    //membuat objek stream ke file history.txt
    fstream file;

    //mengambil waktu sistem dan kemudian dikonversi menjadi
    //objek tm untuk mendapatkan bentuk yang dapat diolah
    time_t now = time(0);
    tm *date = localtime(&now);

    //iterator untuk mengakses map item yang berisikan 
    //nama barang/jasa dan harganya
    map<string, int>::iterator it_item = item.begin();

    //kode untuk menambahkan daftar item yang telah diinputkan
    //pada fungsi inputItem()
    try{
        //file history.txt dibuka dengan mode append
        file.open("history.txt", ios::app);

        //pengecekan jika file berhasil dibuka
        // if(file.is_open()){
        //     cout << "File history berhasil dibuka\n";
        // }

        while(it_item != item.end()){
            //output untuk nama dan harga pada file
            file << it_item->first << '#' << it_item->second << '#';

            //output untuk tanggal update pada file
            file << date->tm_mday << '-' << date->tm_mon + 1 << '-' << 1900 + date->tm_year << '\n';
            it_item++;
        }
    }
    catch(const exception& e){
        cerr << e.what() << '\n';
    }

    //tutup stream file
    file.close();
}

void totalDay(){
    //objek ifstream untuk membaca file history.txt
    ifstream file;

    //cek apakah tanggal yang diinputkan pengguna ada atau tidak
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

    cout << tanggal[0] << " " << MONTH[tanggal[1] - 1] << " " << tanggal[2] << "\n\n";

    //untuk mengecek stream file apakah berhasil diakses atau tidak
    // cout << file.good() << '\n';
    
    //pembacaan file per baris
    while(getline(file, line)){
        //cek apakah tanggal terdapat pada baris yang sedang dicek
        if(line.find(target) != string::npos){
            check = true;

            //self-explanatory
            string namaItem = "";
            string hargaItem = "";
            cout << right << setw(3) << no++ << ". ";

            //variabel count digunakan untuk menghitung karakter '#'
            int count = 0;

            
            for(int i = 0; i < line.length(); i++){
                if(line[i] == '#') count++, i++;

                //format
                //<nama barang/jasa>-<harga>-<tanggal>
                if(count == 0) namaItem += line[i];
                else if(count == 1) hargaItem += line[i];
                else break;
            }

            //kode program untuk output fitur menghitung total hari (no.2)
            //dengan nama item dialokasikan sebanyak 30 karakter rata kiri
            //dengan nama harga dialokasikan sebanyak 6 karakter rata kanan
            cout << left << setw(30) << namaItem << " Rp. " << right << setw(6) << hargaItem << "\n";

            //proses konversi hargaItem dari string ke integer
            total += stoi(hargaItem.c_str());
        }
    }

    //jika terdapat data pada tanggal yang diinputkan
    //maka fitur no.2 akan meng-outputkan total harga daftar item
    //jika tidak, tampilkan pesan di else
    if(check){
        cout << setfill('-') << setw(49) << "+\n";
        cout << setfill(' ') << left << setw(34) << "Total" << ": Rp. " << right << setw(6) << total << '\n';
    } else{
        cout << "Data pada tanggal tersebut tidak ditemukan\n";
    }
    
    //tutup stream file
    file.close();
}

void inputTanggal(int tanggal[]){
    //array of boolean untuk menampung nilai validitas
    //input tanggal, bulan, dan tahun
    bool check[3];

    //array check diinisialisasikan dengan nilai 0 atau false
    memset(check, 0, 3);

    //kode program untuk proses validasi

    //perulangan do while untuk validasi tanggal
    do{
        cout << "Masukkan tanggal (1-31): ";
        cin >> temp;

        //validasi tanggal hari
        //jika input valid, maka input dikonversi menjadi integer
        if(!validasiInputInteger(temp)){
            cout << "Input tanggal tidak valid, masukkan angka bulat!" << endl;
            continue;
        }
        tanggal[0] = stoi(temp.c_str());

        if(tanggal[0] >= 1 && tanggal[0] <= 31)
            check[0] = true;
        else 
            cout << "Tanggal tidak sesuai range, isi dengan tanggal lain!\n\n";
    }while(!check[0]);
    
    //validasi bulan
    do{
        cout << "Masukkan bulan (1-12): ";
        cin >> temp;

        //validasi tanggal hari
        //jika input valid, maka input dikonversi menjadi integer
        if(!validasiInputInteger(temp)){
            cout << "Input bulan tidak valid, masukkan angka bulat!" << endl;
            continue;
        }
        tanggal[1] = stoi(temp.c_str());

        if(tanggal[1] >= 1 && tanggal[1] <= 12)
            check[1] = true;
        else
            cout << "Bulan tidak sesuai range, isi dengan bulan lain!\n\n";
    }while(!check[1]);

    //validasi tahun
    //nilai tahun minimal ditetapkan 1900 mengikuti aturan tipe tm
    //agar bisa diolah
    do{
        cout << "Masukkan tahun (1900-sekarang): ";
        cin >> temp;

        //validasi tanggal hari
        //jika input valid, maka input dikonversi menjadi integer
        if(!validasiInputInteger(temp)){
            cout << "Input tahun tidak valid, masukkan angka bulat!" << endl;
            continue;
        }
        tanggal[2] = stoi(temp.c_str());

        if(tanggal[2] >= 1900)
            check[2] = true;
        else
            cout << "Tahun tidak sesuai range, isi dengan tahun lain!\n\n";
    }while(!check[2]);
    
    //pesan untuk proses validasi yang telah selesai
    cout << "Input tanggal berhasil!\n\n";
}

void totalMonth(){
    //objek ifstream untuk membaca file history.txt
    ifstream file;

    //cek apakah tanggal yang diinputkan pengguna ada atau tidak
    bool check = false;

    //line digunakan untuk menampung string pada file per baris
    string line;

    //mengambil waktu sistem dan kemudian dikonversi menjadi
    //objek tm untuk mendapatkan bentuk yang dapat diolah
    time_t now = time(0);
    tm *date = localtime(&now);

    //no untuk memberikan nomor pada output barang dan jasa, total begitulah (self-explanatory)
    int no = 1, total = 0;

    //array untuk menyimpan bulan, dan tahun
    int tanggal[2];

    //file history dibuka dengan mode membaca
    file.open("history.txt", ios::in);
    
    //pemanggilan fungsi untuk input tanggal
    inputTanggal2(tanggal);

    //target digunakan untuk menampung tanggal yang diinputkan pengguna pada fungsi sebelumnya dalam 
    //bentuk string
    string target = to_string(tanggal[0]) + "-" + to_string(tanggal[1]);

    cout << MONTH[tanggal[0] - 1] << " " << tanggal[1] << "\n\n";

    //untuk mengecek stream file apakah berhasil diakses atau tidak
    // cout << file.good() << '\n';
    
    //pembacaan file per baris
    while(getline(file, line)){
        //cek apakah tanggal terdapat pada baris yang sedang dicek
        if(line.find(target) != string::npos){
            check = true;

            //self-explanatory
            string namaItem = "";
            string hargaItem = "";
            cout << right << setw(3) << no++ << ". ";

            //variabel count digunakan untuk menghitung karakter '#'
            int count = 0;

            for(int i = 0; i < line.length(); i++){
                if(line[i] == '#') count++, i++;

                //format
                //<nama barang/jasa>-<harga>-<tanggal>
                if(count == 0) namaItem += line[i];
                else if(count == 1) hargaItem += line[i];
                else break;
            }

            //kode program untuk output fitur menghitung total hari (no.2)
            //dengan nama item dialokasikan sebanyak 30 karakter rata kiri
            //dengan nama harga dialokasikan sebanyak 6 karakter rata kanan
            cout << left << setw(30) << namaItem << " Rp. " << right << setw(6) << hargaItem << "\n";

            //proses konversi hargaItem dari string ke integer
            total += stoi(hargaItem.c_str());
        }
    }

    //jika terdapat data pada tanggal yang diinputkan
    //maka fitur no.2 akan meng-outputkan total harga daftar item
    //jika tidak, tampilkan pesan di else
    if(check){
        cout << setfill('-') << setw(49) << "+\n";
        cout << setfill(' ') << left << setw(34) << "Total" << ": Rp. " << right << setw(6) << total << '\n';
    } else{
        cout << "Data pada tanggal tersebut tidak ditemukan\n";
    }
    
    //tutup stream file
    file.close();
}

void inputTanggal2(int tanggal[]){
    //array of boolean untuk menampung nilai validitas
    //input tanggal, bulan, dan tahun
    bool check[2];

    //array check diinisialisasikan dengan nilai 0 atau false
    memset(check, 0, 2);

    //kode program untuk proses validasi
    
    //validasi bulan
    do{
        cout << "Masukkan bulan (1-12): ";
        cin >> temp;

        //validasi tanggal hari
        //jika input valid, maka input dikonversi menjadi integer
        if(!validasiInputInteger(temp)){
            cout << "Input bulan tidak valid, masukkan angka bulat!" << endl;
            continue;
        }
        tanggal[0] = stoi(temp.c_str());

        if(tanggal[0] >= 1 && tanggal[1] <= 12)
            check[0] = true;
        else
            cout << "Bulan tidak sesuai range, isi dengan bulan lain!\n\n";
    }while(!check[0]);

    //validasi tahun
    //nilai tahun minimal ditetapkan 1900 mengikuti aturan tipe tm
    //agar bisa diolah
    do{
        cout << "Masukkan tahun (1900-sekarang): ";
        cin >> temp;

        //validasi tanggal hari
        //jika input valid, maka input dikonversi menjadi integer
        if(!validasiInputInteger(temp)){
            cout << "Input tahun tidak valid, masukkan angka bulat!" << endl;
            continue;
        }
        tanggal[1] = stoi(temp.c_str());

        if(tanggal[1] >= 1900)
            check[1] = true;
        else
            cout << "Tahun tidak sesuai range, isi dengan tahun lain!\n\n";
    }while(!check[1]);
    
    //pesan untuk proses validasi yang telah selesai
    cout << "Input tanggal berhasil!\n\n";
}

bool validasiInputInteger(string str){
    regex reg("([^0-9])");
    smatch matches;
    if(regex_search(str, matches, reg)){
        return false;
    }
    return true;
}