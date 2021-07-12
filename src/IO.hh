#ifndef IO_H
#define IO_H

#include <pugixml.hpp>
#include <glm/vec2.hpp>

/**
 * Contains functionality to do with file reading and writing that doesn't have
 * a better place to go.
 */
namespace IO {
    /**
     * Superclass for things that can be loaded from an xml file. Because OOP
     * is kinda retarded, you do not create them straight away from xml, you
     * create an object which is uninitialised at first, and then you call the
     * functions to initialise later.
     */
    class XmlLoad {
        public:
            virtual ~XmlLoad();

            /**
             * Initialises the thing from xml.
             * @param node is the top xml node to initialise from.
             * @return true iff the initialisation worked.
             */
            virtual bool xmlLoad(pugi::xml_node const &node) = 0;
    };

    /**
     * Initialises the given xml loadable object with the top level node of the
     * given xml file.
     * @param object is the object to load.
     * @param filename is the name of the xml file.
     * @return true iff all was successful.
     */
    bool xmlLoadFromFile(XmlLoad &object, char const *filename);

    /**
     * Reads a two dimensional vector of integers out of an xml node. SHould be
     * structured like: <node><x>xValue</x><y>yValue</y></node>. If there are
     * values missing, these just get defaulted to 0.
     * @param node is the top level xml node to read from.
     * @return the parsed vector.
     */
    glm::ivec2 parseIvec2(pugi::xml_node const &node);

    /**
     * Reads a two dimensional vector of uints out of an xml node. SHould be
     * structured like: <node><x>xValue</x><y>yValue</y></node>. If there are
     * values missing, these just get defaulted to 0.
     * @param node is the top level xml node to read from.
     * @return the parsed vector.
     */
    glm::uvec2 parseUvec2(pugi::xml_node const &node);

    /**
     * Takes a node that contains some text and returns the text.
     * @param node the node to get text out of.
     * @return the text that was gotten from it, or null if there was none.
     */
    char const *parseString(pugi::xml_node const &node);

    /**
     * Logs an xml error and prints out the offending xml node.
     * @param problem is a description of the problem with the node.
     * @param node is the node to print.
     */
    void logXmlError(char const *problem, pugi::xml_node const &node);
};

#endif
