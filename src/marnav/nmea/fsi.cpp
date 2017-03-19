#include "fsi.hpp"
#include <marnav/nmea/checks.hpp>
#include <marnav/nmea/io.hpp>

namespace marnav
{
namespace nmea
{
constexpr const char * fsi::TAG;

fsi::fsi()
	: sentence(ID, TAG, talker_id::global_positioning_system)
{
}

fsi::fsi(talker talk, fields::const_iterator first, fields::const_iterator last)
	: sentence(ID, TAG, talk)
{
	if (std::distance(first, last) != 5)
		throw std::invalid_argument{"invalid number of fields in fsi"};

	read(*(first + 0), tx_frequency);
	read(*(first + 1), rx_frequency);
	read(*(first + 2), communications_mode);
	read(*(first + 3), power_level);
	read(*(first + 4), sentence_status);
}

void fsi::set_power_level(uint32_t t)
{
	if (t > 9)
		throw std::invalid_argument{"invalid value for power_level (0..9)"};
	power_level = t;
}

void fsi::set_sentence_status(char t)
{
	check_value(t, {'R', 'C'}, "sentence_status");
	sentence_status = t;
}

void fsi::append_data_to(std::string & s) const
{
	append(s, to_string(tx_frequency));
	append(s, to_string(rx_frequency));
	append(s, to_string(communications_mode));
	append(s, to_string(power_level));
	append(s, to_string(sentence_status));
}
}
}
