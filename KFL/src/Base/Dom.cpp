/**
 * @file Dom.cpp
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
#include <KFL/StringUtil.hpp>
#include <KFL/Util.hpp>

#include <iterator>
#include <string>
#ifdef KLAYGE_CXX17_LIBRARY_CHARCONV_SUPPORT
#include <charconv>
#endif

#include <KFL/Dom.hpp>

using namespace KlayGE;

namespace
{
	bool TryConvertStringToValue(std::string const& value_str, int32_t& val)
	{
#ifdef KLAYGE_CXX17_LIBRARY_CHARCONV_SUPPORT
		char const* str = value_str.data();
		std::from_chars_result result = std::from_chars(str, str + value_str.size(), val);
		return (result.ec == std::errc());
#else
		try
		{
			val = std::stol(value_str);
			return true;
		}
		catch (...)
		{
			return false;
		}
#endif
	}

	bool TryConvertStringToValue(std::string const& value_str, uint32_t& val)
	{
#ifdef KLAYGE_CXX17_LIBRARY_CHARCONV_SUPPORT
		char const* str = value_str.data();
		std::from_chars_result result = std::from_chars(str, str + value_str.size(), val);
		return (result.ec == std::errc());
#else
		try
		{
			val = std::stoul(value_str);
			return true;
		}
		catch (...)
		{
			return false;
		}
#endif
	}

	bool TryConvertStringToValue(std::string const& value_str, float& val)
	{
#ifdef KLAYGE_CXX17_LIBRARY_CHARCONV_SUPPORT
		char const* str = value_str.data();
		std::from_chars_result result = std::from_chars(str, str + value_str.size(), val);
		return (result.ec == std::errc());
#else
		try
		{
			val = std::stof(value_str);
			return true;
		}
		catch (...)
		{
			return false;
		}
#endif
	}

	bool TryConvertStringToValue(std::string const& value_str, bool& val)
	{
		std::string lower_value_str = value_str;
		StringUtil::ToLower(lower_value_str);
		if (lower_value_str == "true")
		{
			val = true;
			return true;
		}
		else if (lower_value_str == "false")
		{
			val = false;
			return true;
		}
		else
		{
			uint32_t uint_val;
			bool const ret = TryConvertStringToValue(value_str, uint_val);
			val = (uint_val != 0) ? true : false;
			return ret;
		}
	}
} // namespace

namespace KlayGE
{
	DomNode* DomDocument::RootNode() const
	{
		return root_.get();
	}

	void DomDocument::RootNode(std::unique_ptr<DomNode> new_node)
	{
		root_ = std::move(new_node);
	}

	std::unique_ptr<DomNode> DomDocument::CloneNode(DomNode const& node)
	{
		auto ret = this->AllocNode(node.Type(), node.Name());
		ret->Value(node.ValueString());

		for (auto child = node.FirstChildNode(); child; child = child->NextSibling())
		{
			ret->AppendNode(this->CloneNode(*child));
		}
		for (auto attr = node.FirstAttrib(); attr; attr = attr->NextAttrib())
		{
			ret->AppendAttrib(this->CloneAttrib(*attr));
		}

		return ret;
	}

	std::unique_ptr<DomAttrib> DomDocument::CloneAttrib(DomAttrib const& attrib)
	{
		return this->AllocAttribString(attrib.Name(), attrib.ValueString());
	}

	std::unique_ptr<DomNode> DomDocument::AllocNode(DomNodeType type, std::string_view name)
	{
		auto ret = MakeUniquePtr<DomNode>(type);
		ret->Name(name);
		return ret;
	}

	std::unique_ptr<DomAttrib> DomDocument::AllocAttrib(std::string_view name)
	{
		auto ret = MakeUniquePtr<DomAttrib>();
		ret->Name(name);
		return ret;
	}

	std::unique_ptr<DomAttrib> DomDocument::AllocAttribBool(std::string_view name, bool value)
	{
		return this->AllocAttribString(name, std::to_string(value ? 1U : 0U));
	}

	std::unique_ptr<DomAttrib> DomDocument::AllocAttribInt(std::string_view name, int32_t value)
	{
		return this->AllocAttribString(name, std::to_string(value));
	}

	std::unique_ptr<DomAttrib> DomDocument::AllocAttribUInt(std::string_view name, uint32_t value)
	{
		return this->AllocAttribString(name, std::to_string(value));
	}

	std::unique_ptr<DomAttrib> DomDocument::AllocAttribFloat(std::string_view name, float value)
	{
		return this->AllocAttribString(name, std::to_string(value));
	}

	std::unique_ptr<DomAttrib> DomDocument::AllocAttribString(std::string_view name, std::string_view value)
	{
		auto ret = this->AllocAttrib(name);
		ret->Value(value);
		return ret;
	}


	DomNode::DomNode(DomNodeType type) : type_(type)
	{
	}

	std::string_view DomNode::Name() const
	{
		return name_;
	}

	void DomNode::Name(std::string_view name)
	{
		name_ = name;
	}

	DomNodeType DomNode::Type() const
	{
		return type_;
	}

	DomNode* DomNode::Parent() const
	{
		return parent_;
	}

	void DomNode::Parent(DomNode* parent)
	{
		parent_ = parent;
	}

	DomAttrib* DomNode::FirstAttrib(std::string_view name) const
	{
		for (auto const& attr : attrs_)
		{
			if (attr->Name() == name)
			{
				return attr.get();
			}
		}

		return nullptr;
	}

	DomAttrib* DomNode::NextAttrib(DomAttrib const& attrib, std::string_view name) const
	{
		for (auto iter = attrs_.begin(); iter != attrs_.end(); ++iter)
		{
			if (iter->get() == &attrib)
			{
				++iter;
				for (; iter != attrs_.end(); ++iter)
				{
					if ((*iter)->Name() == name)
					{
						return iter->get();
					}
				}
				break;
			}
		}

		return nullptr;
	}

	DomAttrib* DomNode::LastAttrib(std::string_view name) const
	{
		for (auto iter = attrs_.rbegin(); iter != attrs_.rend(); ++iter)
		{
			if ((*iter)->Name() == name)
			{
				return iter->get();
			}
		}

		return nullptr;
	}

	DomAttrib* DomNode::FirstAttrib() const
	{
		if (attrs_.empty())
		{
			return nullptr;
		}
		else
		{
			return attrs_.front().get();
		}
	}

	DomAttrib* DomNode::NextAttrib(DomAttrib const& attrib) const
	{
		for (auto iter = attrs_.begin(); iter != attrs_.end(); ++iter)
		{
			if (iter->get() == &attrib)
			{
				++iter;
				if (iter != attrs_.end())
				{
					return iter->get();
				}
				break;
			}
		}

		return nullptr;
	}

	DomAttrib* DomNode::LastAttrib() const
	{
		if (attrs_.empty())
		{
			return nullptr;
		}
		else
		{
			return attrs_.back().get();
		}
	}

	DomAttrib* DomNode::Attrib(std::string_view name) const
	{
		return this->FirstAttrib(name);
	}

	bool DomNode::TryConvertAttrib(std::string_view name, int32_t& val, int32_t default_val) const
	{
		val = default_val;

		auto attr = this->Attrib(name);
		return attr ? attr->TryConvertValue(val) : true;
	}

	bool DomNode::TryConvertAttrib(std::string_view name, uint32_t& val, uint32_t default_val) const
	{
		val = default_val;

		auto attr = this->Attrib(name);
		return attr ? attr->TryConvertValue(val) : true;
	}

	bool DomNode::TryConvertAttrib(std::string_view name, float& val, float default_val) const
	{
		val = default_val;

		auto attr = this->Attrib(name);
		return attr ? attr->TryConvertValue(val) : true;
	}

	int32_t DomNode::AttribInt(std::string_view name, int32_t default_val) const
	{
		auto attr = this->Attrib(name);
		return attr ? attr->ValueInt() : default_val;
	}

	uint32_t DomNode::AttribUInt(std::string_view name, uint32_t default_val) const
	{
		auto attr = this->Attrib(name);
		return attr ? attr->ValueUInt() : default_val;
	}

	float DomNode::AttribFloat(std::string_view name, float default_val) const
	{
		auto attr = this->Attrib(name);
		return attr ? attr->ValueFloat() : default_val;
	}

	std::string_view DomNode::AttribString(std::string_view name, std::string_view default_val) const
	{
		auto attr = this->Attrib(name);
		return attr ? attr->ValueString() : default_val;
	}

	DomNode* DomNode::FirstChildNode(std::string_view name) const
	{
		for (auto const& node : children_)
		{
			if (node->Name() == name)
			{
				return node.get();
			}
		}

		return nullptr;
	}

	DomNode* DomNode::LastChildNode(std::string_view name) const
	{
		for (auto iter = children_.rbegin(); iter != children_.rend(); ++iter)
		{
			if ((*iter)->Name() == name)
			{
				return iter->get();
			}
		}

		return nullptr;
	}

	DomNode* DomNode::FirstChildNode() const
	{
		if (children_.empty())
		{
			return nullptr;
		}
		else
		{
			return children_.front().get();
		}
	}

	DomNode* DomNode::LastChildNode() const
	{
		if (children_.empty())
		{
			return nullptr;
		}
		else
		{
			return children_.back().get();
		}
	}

	DomNode* DomNode::PrevSibling(std::string_view name) const
	{
		DomNode* ret = nullptr;
		for (auto iter = parent_->children_.begin(); iter != parent_->children_.end(); ++iter)
		{
			if ((*iter)->Name() == name)
			{
				ret = iter->get();
			}
			if (iter->get() == this)
			{
				return ret;
			}
		}

		return nullptr;
	}

	DomNode* DomNode::NextSibling(std::string_view name) const
	{
		for (auto iter = parent_->children_.begin(); iter != parent_->children_.end(); ++iter)
		{
			if (iter->get() == this)
			{
				++iter;
				for (; iter != parent_->children_.end(); ++iter)
				{
					if ((*iter)->Name() == name)
					{
						return iter->get();
					}
				}
				break;
			}
		}

		return nullptr;
	}

	DomNode* DomNode::PrevSibling() const
	{
		for (auto iter = parent_->children_.begin(); iter != parent_->children_.end(); ++iter)
		{
			if (iter->get() == this)
			{
				if (iter != parent_->children_.begin())
				{
					--iter;
					return iter->get();
				}
				break;
			}
		}

		return nullptr;
	}

	DomNode* DomNode::NextSibling() const
	{
		for (auto iter = parent_->children_.begin(); iter != parent_->children_.end(); ++iter)
		{
			if (iter->get() == this)
			{
				++iter;
				if (iter != parent_->children_.end())
				{
					return iter->get();
				}
				break;
			}
		}

		return nullptr;
	}

	void DomNode::InsertAfterNode(DomNode const& location, std::unique_ptr<DomNode> new_node)
	{
		for (auto iter = children_.begin(); iter != children_.end(); ++iter)
		{
			if (iter->get() == &location)
			{
				new_node->Parent(this);
				children_.insert(iter, std::move(new_node));
				break;
			}
		}
	}

	void DomNode::InsertAfterAttrib(DomAttrib const& location, std::unique_ptr<DomAttrib> new_attr)
	{
		for (auto iter = attrs_.begin(); iter != attrs_.end(); ++iter)
		{
			if (iter->get() == &location)
			{
				new_attr->Parent(this);
				attrs_.insert(iter, std::move(new_attr));
				break;
			}
		}
	}

	void DomNode::AppendNode(std::unique_ptr<DomNode> new_node)
	{
		new_node->Parent(this);
		children_.emplace_back(std::move(new_node));
	}

	void DomNode::AppendAttrib(std::unique_ptr<DomAttrib> new_attr)
	{
		new_attr->Parent(this);
		attrs_.emplace_back(std::move(new_attr));
	}

	void DomNode::RemoveNode(DomNode const& node)
	{
		for (auto iter = children_.begin(); iter != children_.end(); ++iter)
		{
			if (iter->get() == &node)
			{
				(*iter)->parent_ = nullptr;
				children_.erase(iter);
				break;
			}
		}
	}

	void DomNode::RemoveAttrib(DomAttrib const& attr)
	{
		for (auto iter = attrs_.begin(); iter != attrs_.end(); ++iter)
		{
			if (iter->get() == &attr)
			{
				(*iter)->Parent(nullptr);
				attrs_.erase(iter);
				break;
			}
		}
	}

	void DomNode::ClearChildren()
	{
		for (auto iter = children_.begin(); iter != children_.end(); ++iter)
		{
			(*iter)->Parent(nullptr);
		}
		children_.clear();
	}

	void DomNode::ClearAttribs()
	{
		for (auto iter = attrs_.begin(); iter != attrs_.end(); ++iter)
		{
			(*iter)->Parent(nullptr);
		}
		attrs_.clear();
	}

	bool DomNode::TryConvertValue(bool& val) const
	{
		return TryConvertStringToValue(value_, val);
	}

	bool DomNode::TryConvertValue(int32_t& val) const
	{
		return TryConvertStringToValue(value_, val);
	}

	bool DomNode::TryConvertValue(uint32_t& val) const
	{
		return TryConvertStringToValue(value_, val);
	}

	bool DomNode::TryConvertValue(float& val) const
	{
		return TryConvertStringToValue(value_, val);
	}

	bool DomNode::ValueBool() const
	{
		bool val = false;
		this->TryConvertValue(val);
		return val;
	}

	int32_t DomNode::ValueInt() const
	{
		int32_t val = 0;
		this->TryConvertValue(val);
		return val;
	}

	uint32_t DomNode::ValueUInt() const
	{
		uint32_t val = 0;
		this->TryConvertValue(val);
		return val;
	}

	float DomNode::ValueFloat() const
	{
		float val = 0;
		this->TryConvertValue(val);
		return val;
	}

	std::string_view DomNode::ValueString() const
	{
		return value_;
	}

	void DomNode::Value(bool value)
	{
		value_ = std::to_string(value ? 1U : 0U);
	}

	void DomNode::Value(int32_t value)
	{
		value_ = std::to_string(value);
	}

	void DomNode::Value(uint32_t value)
	{
		value_ = std::to_string(value);
	}

	void DomNode::Value(float value)
	{
		value_ = std::to_string(value);
	}

	void DomNode::Value(std::string_view value)
	{
		value_ = value;
	}


	std::string_view DomAttrib::Name() const
	{
		return name_;
	}

	void DomAttrib::Name(std::string_view name)
	{
		name_ = name;
	}

	DomNode* DomAttrib::Parent() const
	{
		return parent_;
	}

	void DomAttrib::Parent(DomNode* parent)
	{
		parent_ = parent;
	}

	DomAttrib* DomAttrib::NextAttrib(std::string_view name) const
	{
		return parent_->NextAttrib(*this, name);
	}

	DomAttrib* DomAttrib::NextAttrib() const
	{
		return parent_->NextAttrib(*this);
	}

	bool DomAttrib::TryConvertValue(bool& val) const
	{
		return TryConvertStringToValue(value_, val);
	}

	bool DomAttrib::TryConvertValue(int32_t& val) const
	{
		return TryConvertStringToValue(value_, val);
	}

	bool DomAttrib::TryConvertValue(uint32_t& val) const
	{
		return TryConvertStringToValue(value_, val);
	}

	bool DomAttrib::TryConvertValue(float& val) const
	{
		return TryConvertStringToValue(value_, val);
	}

	bool DomAttrib::ValueBool() const
	{
		uint32_t val = 0;
		this->TryConvertValue(val);
		return val ? true : false;
	}

	int32_t DomAttrib::ValueInt() const
	{
		int32_t val = 0;
		this->TryConvertValue(val);
		return val;
	}

	uint32_t DomAttrib::ValueUInt() const
	{
		uint32_t val = 0;
		this->TryConvertValue(val);
		return val;
	}

	float DomAttrib::ValueFloat() const
	{
		float val = 0;
		this->TryConvertValue(val);
		return val;
	}

	std::string_view DomAttrib::ValueString() const
	{
		return value_;
	}

	void DomAttrib::Value(bool value)
	{
		value_ = std::to_string(value ? 1U : 0U);
	}

	void DomAttrib::Value(int32_t value)
	{
		value_ = std::to_string(value);
	}

	void DomAttrib::Value(uint32_t value)
	{
		value_ = std::to_string(value);
	}

	void DomAttrib::Value(float value)
	{
		value_ = std::to_string(value);
	}

	void DomAttrib::Value(std::string_view value)
	{
		value_ = value;
	}
} // namespace KlayGE
