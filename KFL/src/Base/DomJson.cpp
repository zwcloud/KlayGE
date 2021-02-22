/**
 * @file DomJson.cpp
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

#include <KFL/ErrorHandling.hpp>
#include <KFL/ResIdentifier.hpp>
#include <KFL/Util.hpp>

#if defined(KLAYGE_COMPILER_MSVC)
#pragma warning(push)
#pragma warning(disable : 6313) // Incorrect operator: zero-valued flag cannot be tested with bitwise-and
#endif
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#if defined(KLAYGE_COMPILER_MSVC)
#pragma warning(pop)
#endif

#include <KFL/Dom.hpp>

using namespace KlayGE;

namespace
{
	void FillDomAttribFromJsonValue(DomAttrib& attr, rapidjson::Value const& value)
	{
		if (value.IsFalse())
		{
			attr.Value(false);
		}
		else if (value.IsTrue())
		{
			attr.Value(true);
		}
		else if (value.IsBool())
		{
			attr.Value(value.GetBool());
		}
		else if (value.IsInt())
		{
			attr.Value(value.GetInt());
		}
		else if (value.IsInt64())
		{
			attr.Value(static_cast<int32_t>(value.GetInt64()));
		}
		else if (value.IsUint())
		{
			attr.Value(value.GetUint());
		}
		else if (value.IsUint64())
		{
			attr.Value(static_cast<uint32_t>(value.GetUint64()));
		}
		else if (value.IsFloat())
		{
			attr.Value(value.GetFloat());
		}
		else if (value.IsDouble())
		{
			attr.Value(static_cast<float>(value.GetDouble()));
		}
		else if (value.IsString())
		{
			attr.Value(std::string_view(value.GetString()));
		}
		else if (value.IsNull())
		{
		}
		else
		{
			KFL_UNREACHABLE("Unsupported value type.");
		}
	}

	void FillDomNodeFromJsonValue(DomDocument& doc, DomNode& node, rapidjson::Value const& value)
	{
		if (value.IsFalse())
		{
			node.Value(false);
		}
		else if (value.IsTrue())
		{
			node.Value(true);
		}
		else if (value.IsBool())
		{
			node.Value(value.GetBool());
		}
		else if (value.IsInt())
		{
			node.Value(value.GetInt());
		}
		else if (value.IsInt64())
		{
			node.Value(static_cast<int32_t>(value.GetInt64()));
		}
		else if (value.IsUint())
		{
			node.Value(value.GetUint());
		}
		else if (value.IsUint64())
		{
			node.Value(static_cast<uint32_t>(value.GetUint64()));
		}
		else if (value.IsFloat())
		{
			node.Value(value.GetFloat());
		}
		else if (value.IsDouble())
		{
			node.Value(static_cast<float>(value.GetDouble()));
		}
		else if (value.IsString())
		{
			node.Value(std::string_view(value.GetString()));
		}
		else if (value.IsNull())
		{
		}
		else if (value.IsObject())
		{
			for (auto member_iter = value.MemberBegin(); member_iter != value.MemberEnd(); ++member_iter)
			{
				char const first_char = member_iter->name.GetString()[0];
				if (first_char == '@')
				{
					// Map to attrib
					std::string_view name = member_iter->name.GetString();
					BOOST_ASSERT(name[0] == '@');
					name = name.substr(1);

					auto attr = doc.AllocAttrib(name);
					auto& attr_raw = *attr;
					node.AppendAttrib(std::move(attr));
					FillDomAttribFromJsonValue(attr_raw, member_iter->value);
				}
				else if (first_char == '#')
				{
					// Map to value
					node.Value(std::string_view(member_iter->value.GetString()));
				}
				else
				{
					auto child_node = doc.AllocNode(DomNodeType::Element, std::string_view(member_iter->name.GetString()));
					auto& child_node_raw = *child_node;
					node.AppendNode(std::move(child_node));
					FillDomNodeFromJsonValue(doc, child_node_raw, member_iter->value);
				}
			}
		}
		else if (value.IsArray())
		{
			// Map to multiple sibling nodes with the same name
			auto value_iter = value.Begin();
			FillDomNodeFromJsonValue(doc, node, *value_iter);
			++value_iter;
			for (; value_iter != value.End(); ++value_iter)
			{
				auto expand_node = doc.AllocNode(node.Type(), node.Name());
				auto& expand_node_raw = *expand_node;
				node.Parent()->AppendNode(std::move(expand_node));
				FillDomNodeFromJsonValue(doc, expand_node_raw, *value_iter);
			}
		}
		else
		{
			KFL_UNREACHABLE("Unsupported value type.");
		}
	}

	void AppendJsonValueFromDomNode(rapidjson::Document& doc, rapidjson::Value& value, DomNode const& node)
	{
		auto& allocator = doc.GetAllocator();

		std::string_view const name = node.Name();

		if (node.FirstChildNode() || node.FirstAttrib())
		{
			std::vector<std::vector<DomNode*>> cache;
			for (auto child = node.FirstChildNode(); child; child = child->NextSibling())
			{
				bool found = false;
				for (auto& objects : cache)
				{
					if (objects[0]->Name() == child->Name())
					{
						objects.push_back(child);
						found = true;
						break;
					}
				}

				if (!found)
				{
					cache.emplace_back(std::vector<DomNode*>{child});
				}
			}
			for (auto const& objects : cache)
			{
				if (objects.size() == 1)
				{
					rapidjson::Value item_value;
					AppendJsonValueFromDomNode(doc, item_value, *objects[0]);
					value.AddMember(rapidjson::StringRef(objects[0]->Name().data(), objects[0]->Name().size()), item_value, allocator);
				}
				else
				{
					rapidjson::Value array_value;
					array_value.SetArray();
					for (auto const& item : objects)
					{
						rapidjson::Value item_value;
						AppendJsonValueFromDomNode(doc, item_value, *item);
						array_value.PushBack(item_value, allocator);
					}
					value.AddMember(rapidjson::StringRef(objects[0]->Name().data(), objects[0]->Name().size()), array_value, allocator);
				}
			}
			for (auto attr = node.FirstAttrib(); attr; attr = attr->NextAttrib())
			{
				std::string name_str = "@" + std::string(attr->Name());
				rapidjson::Value attr_name(name_str.c_str(), static_cast<rapidjson::SizeType>(name_str.size()), allocator);

				std::string_view const value_sv = attr->ValueString();
				rapidjson::Value attr_value(value_sv.data(), static_cast<rapidjson::SizeType>(value_sv.size()), allocator);

				value.AddMember(attr_name, attr_value, allocator);
			}

			if (!node.ValueString().empty())
			{
				std::string_view const value_sv = node.ValueString();
				rapidjson::Value node_value(value_sv.data(), static_cast<rapidjson::SizeType>(value_sv.size()), allocator);
				value.AddMember("#text", node_value, allocator);
			}
		}
		else
		{
			std::string_view const value_sv = node.ValueString();
			value.SetString(value_sv.data(), static_cast<rapidjson::SizeType>(value_sv.size()), allocator);
		}
	}
} // namespace

namespace KlayGE
{
	std::unique_ptr<DomDocument> ParseJsonDocument(ResIdentifier& source)
	{
		source.seekg(0, std::ios_base::end);
		int len = static_cast<int>(source.tellg());
		source.seekg(0, std::ios_base::beg);
		auto json_src = MakeUniquePtr<char[]>(len + 1);
		source.read(&json_src[0], len);
		json_src[len] = 0;

		rapidjson::Document doc;
		doc.Parse(json_src.get());
		BOOST_ASSERT(!doc.HasParseError());

		auto ret = MakeUniquePtr<DomDocument>();
		auto root = ret->AllocNode(DomNodeType::Element, "");
		FillDomNodeFromJsonValue(*ret, *root, doc);
		ret->RootNode(std::move(root));

		return ret;
	}

	void PrintJsonDocument(DomDocument const& dom, std::ostream& os)
	{
		rapidjson::Document doc;
		doc.SetObject();

		AppendJsonValueFromDomNode(doc, doc, *dom.RootNode());

		rapidjson::StringBuffer sb;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
		doc.Accept(writer);
		os << sb.GetString();
	}
} // namespace KlayGE
