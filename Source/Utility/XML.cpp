#include "XML.hpp"

#include <iostream>  // For std::cout
#include <fstream>   // For std::ifstream
#include <stack>     // For std::stack<T>
#include <vector>    // For std::vector<T>
#include <string>    // For std::string
#include <algorithm> // For std::find()
#include <iterator>  // For std::prev()
#include <map>       // For std::map<K,V>

using std::vector;
using std::map;
using std::stack;

using std::string;
using std::getline;
using std::ifstream;
using std::cout;

using std::find;
using std::find_if;
using std::prev;

using Util::XML;
using Util::XMLTag;

string trim(string s) {
	if(find_if(s.begin(), s.end(), [](char c) -> bool{ return isspace(c); }) == s.end())
		return s;

	size_t i;
	int ii;
	for (i = 0; i < s.length() && isspace(s[i]); i++);  // Find front trim position 
	for (ii = s.length() - 1; ii >= 0 && isspace(s[ii]); ii--);  // Find back trim position

	s.erase(0, i);

	ii -= i;

	if (ii > 0 && s[ii] == '=') 
		s.erase(ii);

	return s;
}

string unquote(string s){
	string::iterator it;
	while((it = find(s.begin(), s.end(), '\"')) != s.end()){
		s.erase(it);
	}
	return s;
}	

vector<string> toContents(string line){
	vector<string> lineContents;
	
	line = trim(line);

	size_t prevI = 0;
	for (size_t i = 0; prevI + i < line.length(); i++) {
		if(line[prevI + i] == '\"'){
			i++;
			while(line[prevI + i] != '\"')
			   i++;	
		} else if (std::isspace(line[prevI + i])) {
			lineContents.push_back(line.substr(prevI, i));
			prevI += i;
			i = 0;
		}
	}

	lineContents.push_back(line.substr(prevI));

	return lineContents;
}

XMLTag parseTag(const string& line){
	XMLTag res;
	
	auto v = toContents(line);
	v[0] = trim(v[0]);
	res.name = v[0].erase(0, 1);
	
	for(auto it = ++v.begin(); it != v.end(); it++){
		auto eqSignPos = it->find('=');
		string name = trim(it->substr(0, eqSignPos));
		
		string value;
		if(eqSignPos != string::npos)
			value = unquote(it->substr(eqSignPos+1, it->length() - eqSignPos));
		else
			value = "";

		if(value.length() && value.find_last_of('/') == value.length()-1){
			value.erase(--value.end());
			res.attribs.insert({"/", ""});
		}
		
		if(name.length() != 0)
			res.attribs.insert({name, value});
	}

	return res;
}

XML Util::ReadXML(string filename){
	XML res;
	res.filename = filename;

	ifstream file(filename);
	
	string rootLine;
	bool gotRoot = false;
	// Handle first <?xml ...?> tag explicitly 
	{
		getline(file, rootLine, '>');

		if(rootLine[1] == '?'){
			res.declTag = parseTag(rootLine);
			res.hasDeclTag = true;
		} else {
			gotRoot = true;
		}
	}

	// Do the same for root
	if(gotRoot){
		res.root = parseTag(rootLine);
	} else {
		string line;
		getline(file, line, '>');
		res.root = parseTag(line);
	}
	
	stack<XMLTag*> tags;
	tags.push(&res.root);

	for(string line; getline(file, line, '>') && !tags.empty();){
		string trimmed = trim(line);
		
		if(trimmed.length() && trimmed[0] != '<'){
			int i;
			for(i = 0; trimmed[i] != '<'; i++);
			tags.top()->data = trimmed.substr(0, i);
			trimmed.erase(0, i);
		}

		XMLTag* tag = new XMLTag(parseTag(trimmed));	
		
		if(tag->name[0] == '/'){ // Closing tag (e.g. </user>
			tags.pop();
			delete tag;
		} else if(tag->hasAttrib("/")) { // Self-closing tag (e.g. <img/>
			tag->attribs.erase(tag->attribs.find("/"));
			tags.top()->children.push_back(tag);
		} else { // Opening tag (e.g. <user>)
			tags.top()->children.push_back(tag);
			tags.push(tag);
		}
	}

	return res;
}

vector<const XMLTag*> XMLTag::findTagsWithName(string name){
	vector<const XMLTag*> res;
	
	for(auto c : children){
		if(c->name == name)
			res.push_back(c);
		else
			c->findTagsWithName(name, res);
	}

	return res;
}

void XMLTag::findTagsWithName(const string& name, vector<const XMLTag*>& v){
	for(auto c : children){
		if(c->name == name)
			v.push_back(c);
		else
			c->findTagsWithName(name, v);
	}
}

/*
void print(const XMLTag& tag, int indent = 8){
	string i = string(indent, ' ');
	
	cout << i << "\"" << tag.name << "\" {\n"
		 << i << "    data: \"" << tag.data << "\" ,\n"
		 << i << "    attributes: {";
	
	if(!tag.attribs.empty()){
		cout << '\n';
		for(auto p : tag.attribs){
			cout << i << "        [\"" << p.first << "\"] : \"" << p.second << "\",\n";
		}
		cout << i << "    },\n";
	} else {
		cout << " [none] },\n";
	}
	cout << i << "    children: {";

	if(!tag.children.empty()){
		cout << "\n";
		for(auto c : tag.children){
			print(*c, indent + 8);
		}
		cout << i << "    }\n";
	} else {
		cout << " [none] }\n";
	}
}

void print(const XML& xml){
	cout << std::boolalpha;

	cout << "XML file {\n" 
		 << "    hasDeclTag: " << xml.hasDeclTag << ",\n"
		 << "    declTag: ";

	if(xml.hasDeclTag){	
		cout << "\n";
		print(xml.declTag);
	} else {
		cout << "[none],\n";
	}
	cout << "    root: {\n";

	print(xml.root);
	cout << "    }\n"
		 << "}\n";
}
*/
