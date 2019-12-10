/*
 * elements.h
 *
 *  Created on: Sep 6, 2019
 *      Author: Daniel
 */

#ifndef XMLOBJS_H_
#define XMLOBJS_H_

#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>

class XMLNode {
public:
	virtual ~XMLNode() = default;
	virtual std::string display() const = 0;
};

class XMLTextNode : public XMLNode {
public:
	XMLTextNode(std::string value) : value_(value) {}
	~XMLTextNode() override = default;

	const std::string& value() const { return value_; }
	std::string display() const override { return value_; }
	void setValue(std::string& value) { value_ = value; }

private:
	std::string value_;
};

class XMLElementNode : public XMLNode {
public:
	using AttributeMap = std::unordered_map<std::string, std::string>;
	using ChildList = std::vector<XMLNode*>;

	XMLElementNode(const std::string& name) : name_(name) {}
	~XMLElementNode() override {
		clearChildren();
	}

	const std::string& name() const { return name_; }
	void setName(std::string& name) { name_ = name;}

	std::string display() const override;

	const AttributeMap& attributes() const { return attributes_; }
	const std::string& attribute(std::string& name) const;

	const ChildList& children() const { return children_; }
	const XMLNode& child(int n) const { return *(children_.at(n)); }
	int size() const {return children_.size();}

	bool addAttribute (std::string& name, std::string& value);
	bool eraseAttribute (std::string& name);
	void clearAttributes () { attributes_.clear(); }

	//both addChild functions take ownership of pointers accepted
	void addChild(XMLNode* child);
	void addChild(unsigned int position, XMLNode* child);

	void eraseChild(unsigned int position);
	void eraseChild();
	void clearChildren() {
		for (const auto* node : children_) delete node;
		children_.clear();
	}

	class double_attribute : std::logic_error {
		//trying to create two attributes with same name in an element
	};

private:
	std::string name_;
	AttributeMap attributes_;
	ChildList children_;
};


#endif /* XMLOBJS_H_ */
