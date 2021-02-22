/**
 * @file DomXml.cpp
 * @author Minmin Gong
 *
 * @section DESCRIPTION
 *
 * This source file is part of KFL, a subproject of KlayGE
 * For the latest info, see http://www.klayge.org
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * You may alternatively use this source under the terms of
 * the KlayGE Proprietary License (KPL). You can obtained such a license
 * from http://www.klayge.org/licensing/.
 */

#include <KFL/KFL.hpp>
#include <KFL/ResIdentifier.hpp>
#include <KFL/Util.hpp>

#if defined(KLAYGE_COMPILER_MSVC)
#pragma warning(push)
#pragma warning(disable : 6313) // Incorrect operator: zero-valued flag cannot be tested with bitwise-and
#endif
#include <rapidxml.hpp>
#if defined(KLAYGE_COMPILER_MSVC)
#pragma warning(pop)
#endif
#if defined(KLAYGE_COMPILER_MSVC)
#pragma warning(push)
#pragma warning(disable : 4100) // 'flags': unreferenced formal parameter
#elif defined(KLAYGE_COMPILER_GCC)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter" // Ignore unused parameter 'flags'
#elif defined(KLAYGE_COMPILER_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter" // Ignore unused parameter 'flags'
#endif
#include <rapidxml_print.hpp>
#if defined(KLAYGE_COMPILER_MSVC)
#pragma warning(pop)
#elif defined(KLAYGE_COMPILER_GCC)
#pragma GCC diagnostic pop
#elif defined(KLAYGE_COMPILER_CLANG)
#pragma clang diagnostic pop
#endif

#include <KFL/Dom.hpp>

using namespace KlayGE;

namespace
{
	std::unique_ptr<DomAttrib> CreateDomAttribFromXmlAttrib(DomDocument& doc, rapidxml::xml_attribute<char> const& attrib)
	{
		return doc.AllocAttribString(
			std::string_view(attrib.name(), attrib.name_size()), std::string_view(attrib.value(), attrib.value_size()));
	}

	rapidxml::xml_attribute<char>* CreateXmlAttribFromDomAttrib(rapidxml::xml_document<char>& doc, DomAttrib const& attrib)
	{
		auto* ret = doc.allocate_attribute();

		std::string_view const name = attrib.Name();
		std::string_view const value = attrib.ValueString();
		ret->name(name.data(), name.size());
		ret->value(value.data(), value.size());

		return ret;
	}

	std::unique_ptr<DomNode> CreateDomNodeFromXmlNode(DomDocument& doc, rapidxml::xml_node<char> const& node)
	{
		DomNodeType type;
		switch (node.type())
		{
		case rapidxml::node_document:
			type = DomNodeType::Document;
			break;

		case rapidxml::node_element:
			type = DomNodeType::Element;
			break;

		case rapidxml::node_data:
			type = DomNodeType::Data;
			break;

		case rapidxml::node_cdata:
			type = DomNodeType::CData;
			break;

		case rapidxml::node_comment:
			type = DomNodeType::Comment;
			break;

		case rapidxml::node_declaration:
			type = DomNodeType::Declaration;
			break;

		case rapidxml::node_doctype:
			type = DomNodeType::Doctype;
			break;

		case rapidxml::node_pi:
		default:
			type = DomNodeType::PI;
			break;
		}

		auto ret = doc.AllocNode(type, std::string_view(node.name(), node.name_size()));
		ret->Value(std::string_view(node.value(), node.value_size()));

		for (auto* child = node.first_node(); child; child = child->next_sibling())
		{
			ret->AppendNode(CreateDomNodeFromXmlNode(doc, *child));
		}
		for (auto* attr = node.first_attribute(); attr; attr = attr->next_attribute())
		{
			ret->AppendAttrib(CreateDomAttribFromXmlAttrib(doc, *attr));
		}

		return ret;
	}

	rapidxml::xml_node<char>* CreateXmlNodeFromDomNode(rapidxml::xml_document<char>& doc, DomNode const& node)
	{
		rapidxml::node_type type;
		switch (node.Type())
		{
		case DomNodeType::Document:
			type = rapidxml::node_document;
			break;

		case DomNodeType::Element:
			type = rapidxml::node_element;
			break;

		case DomNodeType::Data:
			type = rapidxml::node_data;
			break;

		case DomNodeType::CData:
			type = rapidxml::node_cdata;
			break;

		case DomNodeType::Comment:
			type = rapidxml::node_comment;
			break;

		case DomNodeType::Declaration:
			type = rapidxml::node_declaration;
			break;

		case DomNodeType::Doctype:
			type = rapidxml::node_doctype;
			break;

		case DomNodeType::PI:
		default:
			type = rapidxml::node_pi;
			break;
		}

		auto* ret = doc.allocate_node(type);

		std::string_view const name = node.Name();
		std::string_view const value = node.ValueString();
		ret->name(name.data(), name.size());
		ret->value(value.data(), value.size());

		for (auto child = node.FirstChildNode(); child; child = child->NextSibling())
		{
			ret->append_node(CreateXmlNodeFromDomNode(doc, *child));
		}
		for (auto attr = node.FirstAttrib(); attr; attr = attr->NextAttrib())
		{
			ret->append_attribute(CreateXmlAttribFromDomAttrib(doc, *attr));
		}

		return ret;
	}
} // namespace

namespace KlayGE
{
	std::unique_ptr<DomDocument> ParseXmlDocument(ResIdentifier& source)
	{
		source.seekg(0, std::ios_base::end);
		int len = static_cast<int>(source.tellg());
		source.seekg(0, std::ios_base::beg);
		auto xml_src = MakeUniquePtr<char[]>(len + 1);
		source.read(&xml_src[0], len);
		xml_src[len] = 0;

		rapidxml::xml_document<char> doc;
		doc.parse<0>(xml_src.get());

		auto ret = MakeUniquePtr<DomDocument>();
		ret->RootNode(CreateDomNodeFromXmlNode(*ret, *doc.first_node()));

		return ret;
	}

	void PrintXmlDocument(DomDocument const& dom, std::ostream& os)
	{
		rapidxml::xml_document<char> doc;
		rapidxml::xml_node<char>* root_node = CreateXmlNodeFromDomNode(doc, *dom.RootNode());
		doc.append_node(root_node);

		os << "<?xml version=\"1.0\"?>" << std::endl << std::endl;
		os << doc;
	}
} // namespace KlayGE
