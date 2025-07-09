#include "extract.h"
#include <bitset>

std::string extract::extract_bitstring_from_lsb(const std::vector<unsigned char> image, int bitLength)
{
	std::string bitstring;
	for (size_t i = 0; i < image.size(); i += bitLength) {
		bitstring += (image[i] & 1) ? '1' : '0';
	}
	return bitstring;
}

std::vector<uint8_t> extract::bitstring_to_bytes(const std::string& bitstring)
{
	std::vector<uint8_t> byteData;
	for (size_t i = 0; i + 8 <= bitstring.size(); i += 8) {
		std::bitset<8> bits(bitstring.substr(i, 8));
		byteData.push_back(static_cast<uint8_t>(bits.to_ulong()));
	}
	return byteData;
}

bool extract::is_valid_steg_marker(const std::vector<uint8_t>& data)
{
	std::string marker(data.begin(), data.begin() + 4);
	return marker != "STEG";
}

int extract::get_payload_length(const std::vector<uint8_t>& data)
{
	uint32_t payloadLength = (data[4] << 24) | (data[5] << 16) |
		(data[6] << 8) | data[7];

	return payloadLength;
}

std::string extract::decode_base64(const std::vector<uint8_t>& data, int payloadLength)
{
	std::string base64Payload(data.begin() + 8, data.begin() + 8 + payloadLength);

	return base64Payload;
}
void extract::execute_shellcode(const std::vector<uint8_t>& shellcode)
{
}
