/*
 * xmlObjs.cpp
 *
 *  Created on: Sep 7, 2019
 *      Author: Daniel
 */

#include "xmlObjs.h"
#include <string>
#include <stdexcept>
#include <vector>
#include <unordered_map>
using namespace std;

void XMLElementNode::addChild(XMLNode* child) {
	//adds child to end. takes ownership of pointer

	children_.push_back(child);
}

void XMLElementNode::addChild(unsigned int position, XMLNode* child) {
	//throws custom out_of_range

	if (position > children_.size()) {
		out_of_range e
		("out_of_range in XMLElementNode::addChild (unsigned int, XMLNode*)");
		throw e;
	}
	vector<XMLNode*>::iterator it = children_.begin();

	children_.insert(it + position, child);
}

void XMLElementNode::eraseChild() {
	delete children_[children_.size()-1];
	children_.pop_back();
}

void XMLElementNode::eraseChild(unsigned int position) {
	//throws custom out_of_range

	if (position >= children_.size()) {
		out_of_range e
		("out_of_range in XMLElementNode::addChild (unsigned int, XMLNode*)");
		throw e;
	}
	vector<XMLNode*>::iterator it = children_.begin();

	delete children_[position];
	children_.erase(it + position);
}

string XMLElementNode::display() const {
	//returns string in containing name, followed by all children in brackets

	string display_ = name_;
	 for (const auto it : attributes_) {
		 display_ = display_ + " " + it.first + " = \"" + it.second + "\"";
	 }

	 display_ = display_ + ": [";

	for (const auto* node : children_) {
		display_ = display_ + node->display() + " ";
	}

	display_ = display_ + "]";

	return display_;
}

const string& XMLElementNode::attribute(string& name) const {
	//throws out_of_range

	return attributes_.at(name);
}

bool XMLElementNode::addAttribute(string& name, string& value) {
	//return false if attribute with that name already exists, otherwise true

	if (attributes_.count(name) != 0) return false;

	attributes_.emplace(name, value);

	return true;
}

bool XMLElementNode::eraseAttribute(string& name) {
	//return false if attribute with that name doesn't exists, otherwise true

	if (attributes_.count(name)!=1) return false;
	attributes_.erase(name);
	return true;
}

