# XMLParser
This is my first try at programing with C++.

Features:
1. A function returning xml data as a tree with each xml element as a node, and text as childless nodes belonging to elements
2. An [example](https://github.com/danielalpert/XMLParser/blob/master/MyXMLParser/example.cpp) for how data may be easily manipulated in a C++ program
3. Supports attributes for xml elements
4. Supports standard pre-defined "entities" in xml file which are replaced by appropriate text by the parser: &amp;, &lt;, &rt;, &apos;, &quot;. Written in a generic manner, allowing for easy addition of locally-defined entities in future ([check it out](https://github.com/danielalpert/XMLParser/blob/master/MyXMLParser/myParser.h#L26))
