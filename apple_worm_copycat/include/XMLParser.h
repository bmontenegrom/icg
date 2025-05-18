#pragma once
#include <string>
#include <vector>
#include <map>

class XMLNode {
public:
    std::string name;
    std::map<std::string, std::string> attributes;
    std::string value;
    std::vector<XMLNode*> children;
    
    XMLNode(const std::string& nodeName) : name(nodeName) {}
    ~XMLNode() {
        for (auto child : children) {
            delete child;
        }
    }
    
    XMLNode* getChild(const std::string& childName) {
        for (auto child : children) {
            if (child->name == childName) {
                return child;
            }
        }
        return nullptr;
    }
    
    std::string getAttribute(const std::string& attrName) {
        auto it = attributes.find(attrName);
        if (it != attributes.end()) {
            return it->second;
        }
        return "";
    }
};

class XMLParser {
public:
    static XMLNode* parseFile(const std::string& filename);
    static XMLNode* parseString(const std::string& content);
    
private:
    static XMLNode* parseNode(const std::string& content, size_t& pos);
    static void parseAttributes(const std::string& content, size_t& pos, XMLNode* node);
    static std::string parseValue(const std::string& content, size_t& pos);
    static void skipWhitespace(const std::string& content, size_t& pos);
}; 