#include "Debug.hpp"
#include "JSON.hpp"
#include <QString>
#include <catch2/catch.hpp>

TEST_CASE("to_json/from_json") {
    SECTION("QString") {
        QString originalString("Lorem ipsum dolor sit amet");
        nlohmann::json json = originalString;
        QString string = json;
        REQUIRE(string == originalString);
    }
}
