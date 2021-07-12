#include "IO.hh"
#include <spdlog/spdlog.h>
#include <sstream>

IO::XmlLoad::~XmlLoad() {}

bool IO::xmlLoadFromFile(XmlLoad &object, char const *filename) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename);
    if (!result) {
        spdlog::error(
            "loading xml file {} failed: {}",
            filename,
            result.description()
        );
        return false;
    }
    pugi::xml_node node = doc.first_child();
    return object.xmlLoad(node);
}

glm::ivec2 IO::parseIvec2(pugi::xml_node const &node) {
    glm::ivec2 vector(0, 0);
    pugi::xml_node x = node.child("x");
    pugi::xml_node y = node.child("y");
    if (x) {
        pugi::xml_text xText = x.text();
        vector.x = xText.as_int(vector.x);
    }
    if (y) {
        pugi::xml_text yText = y.text();
        vector.y = yText.as_int(vector.y);
    }
    return vector;
}

glm::uvec2 IO::parseUvec2(pugi::xml_node const &node) {
    glm::uvec2 vector(0, 0);
    pugi::xml_node x = node.child("x");
    pugi::xml_node y = node.child("y");
    if (x) {
        pugi::xml_text xText = x.text();
        vector.x = xText.as_uint(vector.x);
    }
    if (y) {
        pugi::xml_text yText = y.text();
        vector.y = yText.as_uint(vector.y);
    }
    return vector;
}

char const *IO::parseString(pugi::xml_node const &node) {
    pugi::xml_text text = node.text();
    if (text) return text.get();
    logXmlError("no string to parse", node);
    return NULL;
}

void IO::logXmlError(char const *problem, pugi::xml_node const &node) {
    std::ostringstream stream;
    node.print(stream, "", pugi::format_raw);
    spdlog::warn("{}: {}", problem, stream.str());
}
