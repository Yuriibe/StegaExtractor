#include <iostream>
#include <vector>
#include <string>
#include "lodepng.h"
#include "extract.h"

int main() {
	// load image
	std::vector<unsigned char> image;
	unsigned width, height;

	unsigned error = lodepng::decode(image, width, height, "stego_output.png");
	if (error) {
		std::cerr << "Error loading PNG: " << lodepng_error_text(error) << "\n";
		system("pause");
		return 1;
	}

	std::cout << "Loaded image: " << width << "x" << height << "\n";

	// create bitstring from LSBs of the image
	std::string bitstring = extract::extract_bitstring_from_lsb(image, 4);

	// bitstring to byte conversion
	std::vector<uint8_t> byteData = extract::bitstring_to_bytes(bitstring);

	// Look for "STEG" header
	if (byteData.size() < 8) {
		std::cerr << "Data too short to contain header\n";
		system("pause");
		return 1;
	}

	// Check for "STEG" marker
	if (!extract::is_valid_steg_marker)
	{
		return 1;
	}

	// Read payload length (big-endian)
	int payloadLength = extract::get_payload_length(byteData);
	std::cout << "Payload Length: " << payloadLength << "\n";

	// Check if the payload length is valid
	if (byteData.size() < 8 + payloadLength) {
		std::cerr << "Incomplete payload data\n";
		system("pause");
		return 1;
	}

	// Extract Base64 payload
	std::string base64Payload = extract::decode_base64(byteData, payloadLength); // Declare base64Payload as a string
	std::cout << "Base64 Payload: " << base64Payload << "\n";
	system("pause");
	return 0;
}
