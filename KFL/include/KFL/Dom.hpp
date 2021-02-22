/**
 * @file Dom.hpp
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

#ifndef KFL_DOM_HPP
#define KFL_DOM_HPP

#pragma once

#include <iosfwd>
#include <vector>

#include <boost/noncopyable.hpp>

namespace KlayGE
{
	enum class DomNodeType
	{
		Document,
		Element,
		Data,
		CData,
		Comment,
		Declaration,
		Doctype,
		PI
	};

	class DomDocument : boost::noncopyable
	{
	public:
		DomNode* RootNode() const;
		void RootNode(std::unique_ptr<DomNode> new_node);

		std::unique_ptr<DomNode> CloneNode(DomNode const& node);
		std::unique_ptr<DomAttrib> CloneAttrib(DomAttrib const& attrib);

		std::unique_ptr<DomNode> AllocNode(DomNodeType type, std::string_view name);
		std::unique_ptr<DomAttrib> AllocAttrib(std::string_view name);
		std::unique_ptr<DomAttrib> AllocAttribBool(std::string_view name, bool value);
		std::unique_ptr<DomAttrib> AllocAttribInt(std::string_view name, int32_t value);
		std::unique_ptr<DomAttrib> AllocAttribUInt(std::string_view name, uint32_t value);
		std::unique_ptr<DomAttrib> AllocAttribFloat(std::string_view name, float value);
		std::unique_ptr<DomAttrib> AllocAttribString(std::string_view name, std::string_view value);

	private:
		std::unique_ptr<DomNode> root_;
	};

	class DomNode final : boost::noncopyable
	{
	public:
		explicit DomNode(DomNodeType type);

		std::string_view Name() const;
		void Name(std::string_view name);

		DomNodeType Type() const;

		DomNode* Parent() const;
		void Parent(DomNode* parent);

		DomAttrib* FirstAttrib(std::string_view name) const;
		DomAttrib* NextAttrib(DomAttrib const& attrib, std::string_view name) const;
		DomAttrib* LastAttrib(std::string_view name) const;
		DomAttrib* FirstAttrib() const;
		DomAttrib* NextAttrib(DomAttrib const& attrib) const;
		DomAttrib* LastAttrib() const;

		DomAttrib* Attrib(std::string_view name) const;

		bool TryConvertAttrib(std::string_view name, bool& val, bool default_val) const;
		bool TryConvertAttrib(std::string_view name, int32_t& val, int32_t default_val) const;
		bool TryConvertAttrib(std::string_view name, uint32_t& val, uint32_t default_val) const;
		bool TryConvertAttrib(std::string_view name, float& val, float default_val) const;

		bool AttribBool(std::string_view name, bool default_val) const;
		int32_t AttribInt(std::string_view name, int32_t default_val) const;
		uint32_t AttribUInt(std::string_view name, uint32_t default_val) const;
		float AttribFloat(std::string_view name, float default_val) const;
		std::string_view AttribString(std::string_view name, std::string_view default_val) const;

		DomNode* FirstChildNode(std::string_view name) const;
		DomNode* LastChildNode(std::string_view name) const;
		DomNode* FirstChildNode() const;
		DomNode* LastChildNode() const;

		DomNode* PrevSibling(std::string_view name) const;
		DomNode* NextSibling(std::string_view name) const;
		DomNode* PrevSibling() const;
		DomNode* NextSibling() const;

		void InsertAfterNode(DomNode const& location, std::unique_ptr<DomNode> new_node);
		void InsertAfterAttrib(DomAttrib const& location, std::unique_ptr<DomAttrib> new_attr);
		void AppendNode(std::unique_ptr<DomNode> new_node);
		void AppendAttrib(std::unique_ptr<DomAttrib> new_attr);

		void RemoveNode(DomNode const& node);
		void RemoveAttrib(DomAttrib const& attr);

		void ClearChildren();
		void ClearAttribs();

		bool TryConvertValue(bool& val) const;
		bool TryConvertValue(int32_t& val) const;
		bool TryConvertValue(uint32_t& val) const;
		bool TryConvertValue(float& val) const;

		bool ValueBool() const;
		int32_t ValueInt() const;
		uint32_t ValueUInt() const;
		float ValueFloat() const;
		std::string_view ValueString() const;

		void Value(bool value);
		void Value(int32_t value);
		void Value(uint32_t value);
		void Value(float value);
		void Value(std::string_view value);

	private:
		DomNode* parent_{};

		DomNodeType type_;
		std::string name_;
		std::string value_;

		std::vector<std::unique_ptr<DomNode>> children_;
		std::vector<std::unique_ptr<DomAttrib>> attrs_;
	};

	class DomAttrib final : boost::noncopyable
	{
	public:
		std::string_view Name() const;
		void Name(std::string_view name);

		DomNode* Parent() const;
		void Parent(DomNode* parent);

		DomAttrib* NextAttrib(std::string_view name) const;
		DomAttrib* NextAttrib() const;

		bool TryConvertValue(bool& val) const;
		bool TryConvertValue(int32_t& val) const;
		bool TryConvertValue(uint32_t& val) const;
		bool TryConvertValue(float& val) const;

		bool ValueBool() const;
		int32_t ValueInt() const;
		uint32_t ValueUInt() const;
		float ValueFloat() const;
		std::string_view ValueString() const;

		void Value(bool value);
		void Value(int32_t value);
		void Value(uint32_t value);
		void Value(float value);
		void Value(std::string_view value);

	private:
		DomNode* parent_{};

		std::string name_;
		std::string value_;
	};

	std::unique_ptr<DomDocument> ParseXmlDocument(ResIdentifier& source);
	void PrintXmlDocument(DomDocument const& dom, std::ostream& os);

	std::unique_ptr<DomDocument> ParseJsonDocument(ResIdentifier& source);
	void PrintJsonDocument(DomDocument const& dom, std::ostream& os);
} // namespace KlayGE

#endif // KFL_DOM_HPP
