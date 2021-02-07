#include "../../Simple/Simple/Simple.h"

using namespace Simple;
using System::BinaryFile;
using System::Color;
using System::ConsoleColor;
using System::Console;
using System::Exception;
using Utility::Cipher;
using Utility::ConsoleMenu;
using Utility::Message;
using Utility::Result;
using Utility::Tools;

#include "User.h"

const ConsoleColor $CursorColor{ Color::Green, Color::Black };
const String $Key{ "SINARMART" };
const Regex $Standard{ "[a-zA-Z0-9]+" };

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
			else if (!regex_match(username, $Standard) || !regex_match(password, $Standard))
				Tools::PrintMessage(2, 13, Message::Warning, "Username dan password hanya menggunakan huruf dan angka.");
			else if (username.length() < 6 || username.length() > 15 || password.length() < 6 || password.length() > 15)
				Tools::PrintMessage(2, 13, Message::Information, "Panjang username dan password 6-15 karakter.");
			else if (password != password2)
				Tools::PrintMessage(2, 13, Message::Information, "Password tidak sama.");
			else {
				DataUser data;
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

 Result<bool, DataUser> Masuk() {
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
				DataUser data = $User.Read()[0];

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

 void Dashboard(DataUser dataUser) {
	 Console::Print(dataUser.Username);

	 Console::GetKey();
 }

int main() {
	try {
		/* Configure windows layout */ {
			Console::SetFont(L"Jetbrains Mono", 18);
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
		if (masuk.First)
			Console::Clear(), Dashboard(masuk.Second);
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

		return -1;
	}
	catch (std::exception& e) {
		Console::Clear();
		Console::Print(
			"Program mengalami error!",
			"\nDescription: ", e.what()
		);
		Console::GetKey();

		return -1;
	}

	return 0;
}