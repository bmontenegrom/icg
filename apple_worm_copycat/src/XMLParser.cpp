#include "XMLParser.h"
#include <fstream>
#include <sstream>
#include <iostream>

XMLNode* XMLParser::parseFile(const std::string& filename) {
    //std::cout << "[XMLParser] Intentando abrir: " << filename << std::endl;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[XMLParser] Error al abrir el archivo: " << filename << std::endl;
        return nullptr;
    }
    //std::cout << "[XMLParser] Archivo abierto correctamente." << std::endl;
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return parseString(buffer.str());
}

XMLNode* XMLParser::parseString(const std::string& content) {
    size_t pos = 0;
    skipWhitespace(content, pos);
    
    if (content[pos] != '<') {
        std::cerr << "[XMLParser] El contenido no comienza con '<'" << std::endl;
        return nullptr;
    }
    
    return parseNode(content, pos);
}

XMLNode* XMLParser::parseNode(const std::string& content, size_t& pos) {
    pos++; // Skip '<'
    skipWhitespace(content, pos);
    
    // Get node name
    std::string name;
    while (pos < content.length() && content[pos] != '>' && content[pos] != ' ' && content[pos] != '/') {
        name += content[pos++];
    }
    //std::cout << "[XMLParser] Nodo encontrado: <" << name << ">" << std::endl;
    
    XMLNode* node = new XMLNode(name);
    
    // Parse attributes
    parseAttributes(content, pos, node);
    
    // Check if it's a self-closing tag
    if (content[pos] == '/') {
        pos += 2; // Skip '/>'
        return node;
    }
    
    pos++; // Skip '>'
    
    // Parse value and children
    std::string value = parseValue(content, pos);
    if (!value.empty()) {
        node->value = value;
    }
    
    // Parse children
    while (pos < content.length()) {
        skipWhitespace(content, pos);
        if (content[pos] == '<') {
            if (content[pos + 1] == '/') {
                // End of current node
                pos += name.length() + 3; // Skip </name>
                break;
            }
            node->children.push_back(parseNode(content, pos));
        }
    }
    
    return node;
}

void XMLParser::parseAttributes(const std::string& content, size_t& pos, XMLNode* node) {
    skipWhitespace(content, pos);
    
    while (pos < content.length() && content[pos] != '>' && content[pos] != '/') {
        std::string attrName;
        while (pos < content.length() && content[pos] != '=' && content[pos] != '>' && content[pos] != '/') {
            attrName += content[pos++];
        }
        
        if (content[pos] == '=') {
            pos++; // Skip '='
            skipWhitespace(content, pos);
            
            char quote = content[pos++];
            std::string attrValue;
            while (pos < content.length() && content[pos] != quote) {
                attrValue += content[pos++];
            }
            pos++; // Skip closing quote
            
            node->attributes[attrName] = attrValue;
        }
        
        skipWhitespace(content, pos);
    }
}

std::string XMLParser::parseValue(const std::string& content, size_t& pos) {
    std::string value;
    while (pos < content.length() && content[pos] != '<') {
        value += content[pos++];
    }
    return value;
}

void XMLParser::skipWhitespace(const std::string& content, size_t& pos) {
    while (pos < content.length() && (content[pos] == ' ' || content[pos] == '\n' || content[pos] == '\r' || content[pos] == '\t')) {
        pos++;
    }
} 