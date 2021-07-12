#include "../IO.hh"
#include "../Util.hh"
#include <catch2/catch.hpp>
#include <pugixml.hpp>
#include <glm/vec2.hpp>

TEST_CASE("Rect XmlLoad", "[io][math]") {
    pugi::xml_document doc;
    pugi::xml_node rectNode = doc.append_child("rect");
    pugi::xml_node x = rectNode.append_child("x");
    pugi::xml_node y = rectNode.append_child("y");
    pugi::xml_node w = rectNode.append_child("w");
    pugi::xml_node h = rectNode.append_child("h");
    x.append_child(pugi::node_pcdata).set_value("34");
    y.append_child(pugi::node_pcdata).set_value("-21");
    w.append_child(pugi::node_pcdata).set_value("-69");
    h.append_child(pugi::node_pcdata).set_value("3");
    Util::Rect rect;
    rect.xmlLoad(rectNode);
    REQUIRE(rect.pos.x == 34);
    REQUIRE(rect.pos.y == -21);
    REQUIRE(rect.size.x == -69);
    REQUIRE(rect.size.y == 3);
}
