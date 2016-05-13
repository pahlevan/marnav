#include <gtest/gtest.h>
#include <marnav/nmea/manufacturer.hpp>

namespace
{
using namespace marnav;

class Test_nmea_manufacturer : public ::testing::Test
{
};

TEST_F(Test_nmea_manufacturer, get_manufacturer_id)
{
	EXPECT_EQ(nmea::manufacturer_id::UNKNOWN, nmea::get_manufacturer_id("P"));
	EXPECT_EQ(nmea::manufacturer_id::UNKNOWN, nmea::get_manufacturer_id("Px"));
	EXPECT_EQ(nmea::manufacturer_id::NMEA, nmea::get_manufacturer_id("Pxx"));
	EXPECT_EQ(nmea::manufacturer_id::UNKNOWN, nmea::get_manufacturer_id("Pxxx"));
	EXPECT_EQ(nmea::manufacturer_id::UNKNOWN, nmea::get_manufacturer_id("Pxxx"));
	EXPECT_EQ(nmea::manufacturer_id::UNKNOWN, nmea::get_manufacturer_id("Pxxxxx"));
	EXPECT_EQ(nmea::manufacturer_id::NMEA, nmea::get_manufacturer_id("GPRMC"));
	EXPECT_EQ(nmea::manufacturer_id::GRM, nmea::get_manufacturer_id("PGRME"));
}

TEST_F(Test_nmea_manufacturer, get_manufacturer_name)
{
	EXPECT_STREQ("NMEA", nmea::get_manufacturer_name("GPRMC").c_str());
	EXPECT_STREQ("UNKNOWN", nmea::get_manufacturer_name("Pxxx").c_str());
	EXPECT_STREQ("GARMIN CORPORATION", nmea::get_manufacturer_name("PGRME").c_str());
}
}