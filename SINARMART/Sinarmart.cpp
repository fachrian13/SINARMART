#include "../../Simple/Simple/Simple.h"

using namespace Simple;
using System::Console;
using System::Exception;

int main() {
	try {
		/* Configure windows layout */ {
			Console::SetFont(L"Jetbrains Mono", 18);
			Console::SetSize(120, 30);
			Console::DisableMaximizeButton();
			Console::DisableResize();
			Console::SetPositionToCenter();
		}

		Console::Print("Hello World");

		Console::GetKey();
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