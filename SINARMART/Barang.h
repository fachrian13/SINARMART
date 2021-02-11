#pragma once
#ifndef _BARANG_
#define _BARANG_

struct DatabaseBarang {
	long Id;
	char Date[32];
	char NamaBarang[64];
	char Kategori[32];
	Uint64 Harga;
	char Keterangan[128];
};

class Barang :public BinaryFile<DatabaseBarang> {
public:
	Barang() :BinaryFile("Barang.bin") {}

	long GetId() {
		DatabaseBarang temp;

		$File.open($FileName.c_str(), Mode::in | Mode::binary);
		$File.seekp(-1 * sizeof(DatabaseBarang), Mode::end);
		$File.read((char*)&temp, sizeof(DatabaseBarang));
		$File.close();

		return temp.Id;
	}
} $Barang;

#endif // !_BARANG_