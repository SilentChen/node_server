/*
 * Xml.h
 *
 *  Created on: Jul 15,2016
 *      Author: zhangyalei
 */

#ifndef XML_H_
#define XML_H_

#include <string>
#include "tinyxml.h"

#define XML_LOOP_BEGIN(NODE)\
	do { \
		if(NODE->Type() != TiXmlNode::TINYXML_ELEMENT) \
			continue;

#define XML_LOOP_END(NODE)\
	} while ((NODE = NODE->NextSibling()));

#define XML_ATTR_LOOP_BEGIN(NODE, ATTR)\
	TiXmlElement *NODE##element = node->ToElement();\
	TiXmlAttribute *ATTR = NODE##element->FirstAttribute();\
	do { 

#define XML_ATTR_LOOP_END(ATTR)\
	} while ((ATTR = ATTR->Next()));


class Xml {
public:
	Xml();
	Xml(const char *path);
	~Xml();
	int load_xml(const char *path);

	//��ȡroot�����һ���ӽڵ�
	TiXmlNode *enter_root_node();
	//����key��ȡroot�����ӽڵ�
	TiXmlNode *get_root_node(const char *key);
	//��ȡnode��������Ϊkey�ڵ�ĵ�һ���ӽڵ�
	TiXmlNode *enter_node(TiXmlNode *node, const char *key);

	//��ȡnode�ڵ�key����
	std::string get_key(TiXmlNode *node);
	//�ж�node�ڵ����Ƿ��������Ϊkey�Ľڵ�
	bool has_key(TiXmlNode *node, const char *key);
	//�ж�node�ڵ��Ƿ����ֽڵ�
	bool has_child(TiXmlNode *node);
	//�ж�node�ڵ��Ƿ�������
	bool has_attribute(TiXmlNode *node);
	//ͳ��node�ڵ�������Ϊkey�ڵ�����
	int count_key(TiXmlNode *node, const char *key);

	/** The meaning of 'value' changes for the specific type of TiXmlNode.
		Document:	filename of the xml file
		Element:	name of the element
		Comment:	the comment text
		Unknown:	the tag contents
		Text:		the text string
	*/
	std::string get_val_str(TiXmlNode *node);
	int get_val_int(TiXmlNode *node);
	float get_val_float(TiXmlNode *node);

	//��ȡ����ֵ
	std::string get_attr_str(TiXmlNode* node, const char *key);
	int get_attr_int(TiXmlNode* node, const char *key);
	float get_attr_float(TiXmlNode* node, const char *key);

private:
	TiXmlElement *rootElement_;
	TiXmlDocument *doc_;
};

#endif