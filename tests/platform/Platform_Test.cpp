#define CATCH_CONFIG_MAIN

#define CATCH_CONFIG_COLOUR_NONE

#include "Platform.h"
#include "catch.hpp"

using namespace MiniEngine;



TEST_CASE("Platoform_TestStatic")
{



    Platform platform;
	
	rde::string name = platform.computerName();

    int bla=20;

}
