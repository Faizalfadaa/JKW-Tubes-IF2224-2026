# JKW-Tubes-IF2224-2026
## IF2224 Teori Bahasa Formal dan Otomata
<img width="800" height="566" alt="m s (4)" src="https://github.com/user-attachments/assets/91c25d53-9d7d-4d6c-af09-0a555208e86e" />



## Deskripsi Program
Program ini mengimplementasikan interpreter untuk bahasa pemrograman Arion menggunakan bahasa C++. Program bekerja melalui beberapa tahap pemrosesan secara berurutan, yaitu Lexical Analysis menggunakan Deterministic Finite Automata (DFA) untuk menghasilkan token dari source code, Syntax Analysis menggunakan recursive descent parsing untuk membangun parse tree, Semantic Analysis untuk memverifikasi makna program dan menghasilkan Decorated Abstract Syntax Tree (Decorated AST) beserta Symbol Table, serta Intermediate Code Generation yang menerjemahkan Decorated AST menjadi kumpulan instruksi stack-machine. Instruksi yang dihasilkan kemudian dieksekusi oleh Stack Interpreter untuk menghasilkan keluaran program.

## Requirements
| Requirement | Minimum | Description |
|-------------|---------|-------------|
| g++ | C++17 support | GNU C++ compiler  |
| make | GNU Make 4.2.1 | Build automation tool, jalankan dengan WSL |
| OS | WSL / Linux |  - |



## Cara Instalasi dan Penggunaan Program


### Kompilasi
Buka terminal WSL, lalu masuk ke folder root project. Setelah itu, jalankan perintah berikut:

```bash
make
```

Perintah tersebut akan mengompilasi seluruh source code dan menghasilkan file executable `main.exe`.


### Run program
Setelah proses kompilasi selesai, jalankan program dengan perintah berikut:

```bash
make run
```



### Membersihkan Hasil Kompilasi

Jika ingin menghapus file executable hasil kompilasi, gunakan perintah berikut:

```bash
make clean
```




## Identitas Kelompok
| NIM | Nama |
|-----|------|
| 13524007 | Muhammad Ashkar |
| 13524039 | Rhenaldy Cahyadi Putra |
| 13524069 | Miguel Rangga Deardo Sinaga |
| 13524097 | Muhammad Faiz Alfada Dharma |

Made with 🤍 by **JKW-JarakKecepatanWaktu**
