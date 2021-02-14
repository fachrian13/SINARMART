#include "../../Simple/Simple/Simple.h"

using namespace Simple;
using System::BinaryFile;
using System::Color;
using System::ConsoleColor;
using System::Console;
using System::Exception;
using System::Time;
using Utility::Cipher;
using Utility::ConsoleMenu;
using Utility::ConsoleTable;
using Utility::Convert;
using Utility::Message;
using Utility::Result;
using Utility::Tools;

#include "User.h"
#include "Barang.h"

const ConsoleColor $CursorColor{ Color::Green, Color::Black };
const String $Key{ "SINARMART" };
const Regex $StandardEx{ "[a-zA-Z0-9]+" };
const Regex $IdBarangEx{ "\\b(100)([^ ]*)" };

void Registrasi() {
	ConsoleMenu mRegistrasi{
		{
			"Username         :",
			"Password         :",
			"Confirm password :",
			"[Registrasi]",
			"[Kembali]"
		},
		$CursorColor,
		{2, 7}
	};
	ConsoleMenu::Selection sRegistrasi;
	String username;
	String password;
	String password2;

	do {
		Tools::Print(2, 2, "================");
		Tools::Print(2, 3, "   REGISTRASI");
		Tools::Print(2, 4, "================");
		Tools::Print(2, 5, "Silakan buat akun untuk melindungi data.");

		sRegistrasi = mRegistrasi.Print();

		switch (sRegistrasi.First) {
		case 0:
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), username);
			username = Console::GetLine();
			break;
		case 1:
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), password);
			password = Tools::GetPassword();
			break;
		case 2:
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), password2);
			password2 = Tools::GetPassword();
			break;
		}

		if (sRegistrasi.Second == "[Registrasi]") {
			if (username.empty() || password.empty() || password2.empty())
				Tools::PrintMessage(2, 13, Message::Warning, "Silakan lengkapi semua data.");
			else if (!regex_match(username, $StandardEx) || !regex_match(password, $StandardEx))
				Tools::PrintMessage(2, 13, Message::Warning, "Username dan password hanya menggunakan huruf dan angka.");
			else if (username.length() < 6 || username.length() > 15 || password.length() < 6 || password.length() > 15)
				Tools::PrintMessage(2, 13, Message::Information, "Panjang username dan password 6-15 karakter.");
			else if (password != password2)
				Tools::PrintMessage(2, 13, Message::Information, "Password tidak sama.");
			else {
				DatabaseUser data;
				data.Blocked = false;
				strcpy_s(data.Username, username.c_str());
				strcpy_s(data.Password, Cipher::Vigenere(password, $Key).c_str());

				$User.Write(data);

				Tools::PrintMessage(2, 13, Message::Information, "Registrasi user berhasil.");
				break;
			}
		}
	} while (sRegistrasi.Second != "[Kembali]");
}
Result<bool, DatabaseUser> Masuk() {
	ConsoleMenu mMasuk{
		{
			"Username :",
			"Password :",
			"[Masuk]",
			"[Kembali]"
		},
		$CursorColor,
		{2, 7}
	};
	ConsoleMenu::Selection sMasuk;
	String username;
	String password;

	do {
		Tools::Print(2, 2, "================");
		Tools::Print(2, 3, "   MASUK AKUN");
		Tools::Print(2, 4, "================");
		Tools::Print(2, 5, "Silakan masuk ke akun anda.");

		sMasuk = mMasuk.Print();

		switch (sMasuk.First) {
		case 0:
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), username);
			username = Console::GetLine();
			break;
		case 1:
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), password);
			password = Tools::GetPassword();
			break;
		}

		if (sMasuk.Second == "[Masuk]") {
			if (username.empty() || password.empty())
				Tools::PrintMessage(2, 12, Message::Warning, "Silakan lengkapi semua data.");
			else {
				DatabaseUser data = $User.Read()[0];

				if (strcmp(data.Username, username.c_str()) != 0)
					Tools::PrintMessage(2, 12, Message::Information, "Username tidak terdaftar.");
				else if (strcmp(data.Password, Cipher::Vigenere(password, $Key).c_str()))
					Tools::PrintMessage(2, 12, Message::Information, "Password salah.");
				else
					return { true, data };
			}
		}
	} while (sMasuk.Second != "[Kembali]");

	return{ 0, {} };
}

void TambahBarang() {
	ConsoleMenu mTambah{
		{
			"Nama barang  :",
			"Kategori     :",
			"Harga barang :",
			"Keterangan   :",
			"[Tambah]",
			"[Kembali]"
		},
		$CursorColor,
		{2, 6}
	};
	ConsoleMenu mKategori{
		{
			"Sabun Cuci",
			"Sabun Mandi",
			"Rokok",
			"Minuman Botol",
			"Minuman Kaleng",
			"Minuman Gelas",
			"Sikat Gigi",
			"Kopi Sachet",
			"Susu Sachet",
			"Alat Sekolah",
			"Elektronik",
			"Makanan Ringan",
			"Roti",
			"Mainan Anak",
			"Kebutuhan Rumah Tangga"
		},
		$CursorColor,
		{17, 7},
		true
	};
	ConsoleMenu::Selection sTambah;
	String nama;
	String kategori;
	String harga;
	String keterangan;

	do {
		Tools::Print(2, 2, "========================");
		Tools::Print(2, 3, "   TAMBAH DATA BARANG");
		Tools::Print(2, 4, "========================");

		Tools::Print(17, 6, nama);
		Tools::Print(17, 7, kategori);
		Tools::Print(17, 8, harga);
		Tools::Print(17, 9, keterangan);
		
		sTambah = mTambah.Print();

		switch (sTambah.First) {
		case 0:
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), nama);
			nama = Console::GetLine();
			break;
		case 1:
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), kategori);
			kategori = mKategori.Print(5).Second;
			mKategori.Clear();
			break;
		case 2:
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), harga);
			harga = Tools::GetNumber();
			break;
		case 3:
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), keterangan);
			keterangan = Console::GetLine();
			break;
		}

		if (sTambah.Second == "[Tambah]") {
			if (nama.empty() || kategori.empty() || harga.empty())
				Tools::PrintMessage(2, 13, Message::Warning, "Silakan lengkapi semua data.");
			else if (!Tools::IsNumber(harga))
				Tools::PrintMessage(2, 13, Message::Warning, "Harga barang berupa angka.");
			else {
				DatabaseBarang data;
				String sId = Convert::ToString($Barang.GetId());

				data.Id = !$Barang.Empty() ? Convert::ToInt32(sId.replace(3, -1, Convert::ToString(Convert::ToInt32(sId.substr(3)) + 1))) : 1001;
				strcpy_s(data.Date, Time::Now().StandardFormat().c_str());
				strcpy_s(data.NamaBarang, nama.c_str());
				strcpy_s(data.Kategori, kategori.c_str());
				data.Harga = Convert::ToUint64(harga);
				strcpy_s(data.Keterangan, keterangan.c_str());

				$Barang.Write(data);

				Tools::PrintMessage(2, 13, Message::Information, "Data barang berhasil ditambahkan.");
				break;
			}
		}
	} while (sTambah.Second != "[Kembali]");
}
void TampilkanBarang() {
	ConsoleTable tBarang{
		"No",
		"Id Barang",
		"Tanggal Penambahan",
		"Nama Barang",
		"Kategori",
		"Harga",
		"Keterangan"
	};
	Vector<DatabaseBarang> stored = $Barang.Read();

	for (SizeType i = 0, no = 1; i < stored.size(); i++, no++)
		tBarang += {
		Convert::ToString(no),
		Convert::ToString(stored[i].Id),
		stored[i].Date,
		stored[i].NamaBarang,
		stored[i].Kategori,
		"Rp. " + Convert::ToString(stored[i].Harga) + ",-",
		stored[i].Keterangan
	};

	Tools::Print(2, 2, "=============================");
	Tools::Print(2, 3, "   DAFTAR BARANG DI GUDANG");
	Tools::Print(2, 4, "=============================");

	Console::SetCursorPosition(0, 6); tBarang.Print();
}
void CariBarang() {
	ConsoleMenu mCari{
		{
			"Id/Nama Barang :",
			"[Cari]",
			"[Kembali]"
		},
		$CursorColor,
		{2, 7}
	};
	ConsoleMenu::Selection sCari;
	String cari;

	do {
		Tools::Print(2, 2, "======================");
		Tools::Print(2, 3, "   CARI DATA BARANG");
		Tools::Print(2, 4, "======================");
		Tools::Print(2, 5, "Silakan masukkan Id atau Nama barang.");

		sCari = mCari.Print();

		switch (sCari.First) {
		case 0:
			Console::Print(" ");
			Tools::Clear(Console::GetCursorPosition(), cari);
			cari = Console::GetLine();
			break;
		}

		if (sCari.Second == "[Cari]") {
			if (cari.empty())
				Tools::PrintMessage(2, 11, Message::Warning, "Silakan isi barang yang akan dicari.");
			else {
				Vector<DatabaseBarang> dataBarang = $Barang.Read();

				if (Tools::IsNumber(cari)) {
					if (!regex_match(cari, $IdBarangEx))
						Tools::PrintMessage(2, 11, Message::Warning, "Id barang salah.");
					else {
						long lId = Convert::ToLong(cari);
						Result<bool, DatabaseBarang> result{ false, {} };

						for (SizeType i = 0; i < dataBarang.size(); i++)
							if (lId == dataBarang[i].Id) {
								result = { true, dataBarang[i] };
								break;
							}

						if (!result.First)
							Tools::PrintMessage(2, 11, Message::Information, "Barang dengan ID: " + cari + " tidak ditemukan.");
						else {
							ConsoleTable tBarang{
								"Id Barang",
								"Tanggal Penambahan",
								"Nama Barang",
								"Kategori",
								"Harga",
								"Keterangan"
							};
							tBarang += {
								Convert::ToString(result.Second.Id),
								result.Second.Date,
								result.Second.NamaBarang,
								result.Second.Kategori,
								"Rp. " + Convert::ToString(result.Second.Harga) + ",-",
								result.Second.Keterangan
							};

							Console::Print("\n\n\n");
							tBarang.Print();
						}
					}
				}
				else {
					ConsoleTable tBarang{
						"No",
						"Id Barang",
						"Tanggal Penambahan",
						"Nama Barang",
						"Kategori",
						"Harga",
						"Keterangan"
					};
					Vector<DatabaseBarang> result;
					String lowCari = cari;
					
					std::transform(lowCari.begin(), lowCari.end(), lowCari.begin(), ::tolower);
					for (DatabaseBarang index : dataBarang) {
						String lowData = index.NamaBarang;

						std::transform(lowData.begin(), lowData.end(), lowData.begin(), ::tolower);
						if (lowData.find(lowCari) != -1)
							result.push_back(index);
					}

					for (SizeType i = 0, no = 1; i < result.size(); i++, no++)
						tBarang += {
						Convert::ToString(no),
						Convert::ToString(result[i].Id),
						result[i].Date,
						result[i].NamaBarang,
						result[i].Kategori,
						"Rp. " + Convert::ToString(result[i].Harga) + ",-",
						result[i].Keterangan
					};

					Console::Print("\n\n\n");
					tBarang.Print();
				}
			}
		}
	} while (sCari.Second != "[Kembali]");

}
void DataBarang() {
	ConsoleMenu mBarang{
		{
			"1. Tambah data barang",
			"2. Tampilkan semua data",
			"3. Cari data",
			"4. Update data",
			"5. Hapus data barang",
			"[Kembali]"
		},
		$CursorColor,
		{2, 6}
	};
	ConsoleMenu::Selection sBarang;

	do {
		Tools::Print(2, 2, "=================");
		Tools::Print(2, 3, "   DATA BARANG");
		Tools::Print(2, 4, "=================");

		sBarang = mBarang.Print();

		switch (sBarang.First) {
		case 0:
			Console::Clear();
			TambahBarang();
			Console::Clear();
			break;
		case 1:
			Console::Clear();
			TampilkanBarang();
			Console::Clear();
			break;
		case 2:
			Console::Clear();
			CariBarang();
			Console::Clear();
			break;
		}
	} while (sBarang.Second != "[Kembali]");
}

void Dashboard(DatabaseUser userData) {
	ConsoleMenu mDashboard{
		{
			"1. Data barang",
			"2. Data pelanggan",
			"3. Data pengguna",
			"[Kembali]"
		},
		$CursorColor,
		{2, 7}
	};
	ConsoleMenu::Selection sDashboard;

	do {
		Tools::Print(2, 2, "===============");
		Tools::Print(2, 3, "   SINARMART");
		Tools::Print(2, 4, "===============");
		Tools::Print(2, 5, "Jl. Karaton, No.58, RT.05, RW.08, Cigembor, Ciamis.");

		sDashboard = mDashboard.Print();

		switch (sDashboard.First) {
		case 0:
			Console::Clear();
			DataBarang();
			Console::Clear();
			break;
		}

	} while (sDashboard.Second != "[Kembali]");
}

int main() {
	try {
		/* Configure windows layout */ {
			Console::SetFont(L"Consolas", 18);
			Console::SetSize(120, 30);
			Console::DisableMaximizeButton();
			Console::DisableResize();
			Console::SetPositionToCenter();
		}

		Console::Clear();
		if ($User.Empty())
			Registrasi();

		Console::Clear();
		Result masuk = Masuk();
		if (masuk.First) {
			Console::Clear();
			Dashboard(masuk.Second);
		}
	}
	catch (Exception& e) {
		Console::Clear();
		Console::Print(
			"Program mengalami error!",
			"\nFile: ", e.File(),
			"\nLine: ", e.Line(),
			"\nFunction: ", e.Function(),
			"\nDescription: ", e.Description()
		);
		Console::GetKey();

		return EXIT_FAILURE;
	}
	catch (std::exception& e) {
		Console::Clear();
		Console::Print(
			"Program mengalami error!",
			"\nDescription: ", e.what()
		);
		Console::GetKey();

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}