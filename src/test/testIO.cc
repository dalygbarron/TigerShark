#include "test.hh"
#include "../IO.hh"
#include "../Util.hh"
#include "../Gfx.hh"
#include <pugixml.hpp>
#include <glm/vec2.hpp>
#include <stdio.h>

TEST_CASE("xmlLoadFromFile Rect", "[io]") {
    char const *file = TMP(
        <ringo>
            <x>12</x>
            <y>   43</y>
            <w>-1024</w>
            <h>987654321</h>
        </ringo>
    );
    REQUIRE(file);
    Util::Rect rect;
    bool result = IO::xmlLoadFromFile(rect, file);
    REQUIRE(result);
    REQUIRE(rect.pos.x == 12);
    REQUIRE(rect.pos.y == 43);
    REQUIRE(rect.size.x == -1024);
    REQUIRE(rect.size.y == 987654321);
}

TEST_CASE("xmlLoadFromFile Atlas", "[io][screen]") {
    char const *file = TMP(
        <atlas>
            <pic>plant.png</pic>
            <sprite name="wf"><x>0</x><y>0</y><w>3</w><h>12</h></sprite>
            <sprite name="fi"><x>3</x><y>0</y><w>5</w><h>10</h></sprite>
            <sprite name="agro"><x>8</x><y>0</y><w>8</w><h>9</h></sprite>
        </atlas>
    );
    REQUIRE(file);
    IN_WINDOW(
        Gfx::Atlas atlas;
        bool result = IO::xmlLoadFromFile(atlas, file);
        REQUIRE(result);
        Util::Rect wf = atlas.getSprite("wf");
        Util::Rect fi = atlas.getSprite("fi");
        Util::Rect agro = atlas.getSprite("agro");
        Util::Rect nope = atlas.getSprite("nope");
        REQUIRE(wf.pos.x == 0);
        REQUIRE(wf.pos.y == 0);
        REQUIRE(wf.size.x == 3);
        REQUIRE(wf.size.y == 12);
        REQUIRE(fi.pos.x == 3);
        REQUIRE(fi.pos.y == 0);
        REQUIRE(fi.size.x == 5);
        REQUIRE(fi.size.y == 10);
        REQUIRE(agro.pos.x == 8);
        REQUIRE(agro.pos.y == 0);
        REQUIRE(agro.size.x == 8);
        REQUIRE(agro.size.y == 9);
        REQUIRE(nope.pos.x == 0);
        REQUIRE(nope.pos.y == 0);
        REQUIRE(nope.size.x == 0);
        REQUIRE(nope.size.y == 0);
    );
}

TEST_CASE("test parseIvec2", "[io][math]") {
    pugi::xml_document doc;
    pugi::xml_node ivec = doc.append_child("ivec");
    pugi::xml_node x = ivec.append_child("x");
    pugi::xml_node y = ivec.append_child("y");
    x.append_child(pugi::node_pcdata).set_value("34");
    y.append_child(pugi::node_pcdata).set_value("-21");
    glm::ivec2 vector = IO::parseIvec2(ivec);
    REQUIRE(vector.x == 34);
    REQUIRE(vector.y == -21);
}

TEST_CASE("test parseUvec2", "[io][math]") {
    pugi::xml_document doc;
    pugi::xml_node uvec = doc.append_child("ivec");
    pugi::xml_node x = uvec.append_child("x");
    pugi::xml_node y = uvec.append_child("y");
    x.append_child(pugi::node_pcdata).set_value("34");
    y.append_child(pugi::node_pcdata).set_value("-21");
    glm::uvec2 vector = IO::parseUvec2(uvec);
    REQUIRE(vector.x == 34);
    REQUIRE(vector.y == 0);
}
