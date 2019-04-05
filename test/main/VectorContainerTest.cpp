#define CATCH_CONFIG_MAIN
#include "../include/catch.hpp"
#include <VectorContainer.h>

TEST_CASE("VectorContainers can be set and get") {
	cgl::VectorContainer vectorContainer;
	SECTION("Setting the x value changes the get") {
		vectorContainer.setX(5);
		REQUIRE(vectorContainer )
	}
}
