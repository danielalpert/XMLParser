/*
 * example.cpp
 *
 *  Created on: Sep 6, 2019
 *      Author: Daniel
 */

#include "xmlObjs.h"
#include "myParser.h"
#include <string>
#include <iostream>
using namespace std;

int main() {

	XMLElementNode root ("root");
	auto* c1 = new XMLElementNode("child1");
	auto* c2 = new XMLElementNode("child2");
	auto* txt = new XMLTextNode("text");
	string name = "name";
	string value = "val";
	string str = "";

	root.addChild(c1);
	root.addChild(c2);
	c1->addChild(txt);
	c2->addAttribute(name, value);

	cout << root.display() << endl;

	MyXMLFile file("test.xml");

	cout << file.rootElement().display();
	return 0;
}


