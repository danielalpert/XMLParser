/*
 * myParser.cpp
 *
 *  Created on: Sep 7, 2019
 *      Author: Daniel
 */
#include "xmlObjs.h"
#include "myParser.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stack>
#include <unordered_map>
#include <utility>
#include <cctype>
#include <memory>
using namespace std;

#define syntax \
	runtime_error\
	("xml syntax error in file " + path_ + " at line " + to_string(line))

void MyXMLFile::parse() {
	//throws runtime_error with description of problem if parse unsuccessful
	//ignores all text in xml file outside of root element

	runtime_error ffail("io error with file " + path_);

	//check if path is to xml file
	if (path_.length()<4) throw ffail;
	if (path_.substr(path_.length()-4, 4) != ".xml") throw ffail;

	ifstream stream(path_.c_str());
	if (stream.fail()) throw ffail;

	//stack keeping track of current element and location in file
	stack<pair<XMLElementNode*, int>> stk;
	//stack keeping track of token calling entity reference
	stack<pair<string, token_type>> entity_stk;
	//name of current attribute
	string curr_attr  = "";
	//current line in text
	line = 1;
	//last character read
	char c;
	//content of current token (e.g. element name, attribute, text)
	string token;
	//type of current token
	token_type tk_type = pre_root_text;
	//parent element of current node
	XMLElementNode* last = nullptr;
	//root element
	XMLElementNode* root = nullptr;

	//start parsing file until eof reached or syntax error
	while (!stream.eof()) {
		stream.get(c);
		if (c=='\n') line++;

		if (tk_type == text) {
			if (c == '<') {
				if (token != "") {
					stk.top().first->addChild(new XMLTextNode(token));
				}
				token = "";
				tk_type = element_name;
				continue;
			} else if (c == '&') {
				entity_stk.push(pair<string, token_type>(token, tk_type));
				token = "";
				tk_type = entity_ref;
				continue;
			}
		} else if (tk_type == element_name) {
			if (c == '<' || c == '&') {
				if (root != nullptr) delete root;
				throw syntax;
			}
			if (c == '/' && token == "") {
				if (root != nullptr) tk_type = end_element;
				else tk_type = pre_root_text;
				continue;
			}
			if (c == '>') {
				if (token == "") {
					if (root == nullptr) {
						tk_type = pre_root_text;
						continue;
					}
					delete root;
					throw syntax;
				}
				if (root != nullptr) {
					last = stk.top().first;
					stk.push(pair<XMLElementNode*, int>
						(new XMLElementNode(token), line));

					last->addChild(stk.top().first);
				} else {
					stk.push(pair<XMLElementNode*, int>
						(new XMLElementNode(token), line));

					root = stk.top().first;
				}
				token = "";
				tk_type = text;
				continue;
			}
			if (isspace(c)) {
				if (token == "") continue;
				if (root != nullptr) {
					last = stk.top().first;
					stk.push(pair<XMLElementNode*, int>
						(new XMLElementNode(token), line));

					last->addChild(stk.top().first);
				} else {
					stk.push(pair<XMLElementNode*, int>
						(new XMLElementNode(token), line));

					root = stk.top().first;
				}
				token = "";
				tk_type = attribute_name;
				continue;
			}
		} else if (tk_type == attribute_name) {
			if (c == '&') {
				if (root != nullptr) delete root;
				throw syntax;
			}
			if (c=='=') {
				if (token == "") {
					if (root != nullptr) delete root;
					throw syntax;
				}
				curr_attr = token;
				token = "=";
				tk_type = attribute_val;
				continue;
			}
			if (isspace(c)) {
				if (token == "") continue;
				curr_attr = token;
				token = "";
				tk_type = attribute_val;
				continue;
			}
			if (c=='/') {
				if (token != "") {
					if (root != nullptr) delete root;
					throw syntax;
				}
				tk_type = end_empty_element;
				continue;
			}
			if (c=='>') {
				if (token != "") {
					if (root != nullptr) delete root;
					throw syntax;
				} else {
					tk_type = text;
					continue;
				}
			}

		} else if (tk_type == attribute_val) {
			if (token == "=") {
				token = "";
			} else if (c == '&') {
				if (token == "") {
					if (root != nullptr) delete root;
					throw syntax;
				}
				entity_stk.push(pair<string, token_type>(token, tk_type));
				token = "";
				tk_type = entity_ref;
				continue;
			}
			if (isspace(c) && token == "") continue;
			if ((c != '"' && c != '\'') && token == "") {
				if (root != nullptr) delete root;
				throw syntax;
			}
			if ((c == '"' || c == '\'') && token != "") {
				if (c == token[0]) {
					token.erase(token.begin());
					stk.top().first->addAttribute(curr_attr, token);
					token = "";
					tk_type = attribute_name;
					curr_attr = "";
					continue;
				}
			}
		} else if (tk_type == end_empty_element) {
			if (c != '>') {
				if (root != nullptr) delete root;
				throw syntax;
			}
			tk_type = text;
			stk.pop();
			if (stk.empty()) break;
			continue;

		} else if (tk_type == end_element) {
			if (token=="") while (isspace(c)) stream.get(c);
			if (c == '>') {
				if (token != stk.top().first->name()) {
					if (root != nullptr) delete root;
					throw syntax;
				}
				stk.pop();
				if (stk.empty()) break;
				tk_type = text;
				token = "";
				continue;

			}
		} else if (tk_type == pre_root_text) {
			if (c != '<') continue;
			tk_type = element_name;
			continue;

		} else { //tk_type == entity_ref
			if (c == ';') {
				if (entities.count(token) == 0) {
					if (root != nullptr) delete root;
					throw syntax;
				}
				token = entity_stk.top().first + entities[token];
				tk_type = entity_stk.top().second;
				entity_stk.pop();
				continue;
			}
			if (c == '&' || c == '<') {
				if (root != nullptr) delete root;
				throw syntax;
			}
		}

		token += c;
	}

	//finished parsing file. check for syntax errors and then exit
	if (root == nullptr) throw runtime_error("couldn't find root element");
	else if (!stk.empty()) {
		delete root;
		throw runtime_error("couldn't find closing tag for element " +
			stk.top().first->name() + " at line " +
			to_string(stk.top().second));
	}

	root_ = root;

}

#undef syntax



