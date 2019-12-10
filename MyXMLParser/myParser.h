/*
 * myParser.h
 *
 *  Created on: Sep 7, 2019
 *      Author: Daniel
 */

#ifndef MYPARSER_H_
#define MYPARSER_H_

#include "xmlObjs.h"
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>

class MyXMLFile {
public:
	enum token_type {
		text, element_name, attribute_name, attribute_val, entity_ref,
		end_element, end_empty_element, end_attribute, pre_root_text
	};

	MyXMLFile (std::string path) : path_(path) {
		root_ = nullptr;
		entities.emplace("amp", "&");
		entities.emplace("lt", "<");
		entities.emplace("gt", ">");
		entities.emplace("apos", "'");
		entities.emplace("quot", "\"");
		parse();
	}
	~MyXMLFile () {
		delete root_;
	}

	const XMLElementNode& rootElement() const {
		if (root_ != nullptr) return *root_;
		else throw std::runtime_error("");
	}

	void parse();

private:
	// The path of the input file.
	std::string path_;
	XMLElementNode* root_;

	// A map from xx to yy.
	std::unordered_map<std::string, std::string> entities;
	int line;
};

#endif /* MYPARSER_H_ */
