/**
 * @file MeshMetadata.cpp
 * @author Minmin Gong
 *
 * @section DESCRIPTION
 *
 * This source file is part of KlayGE
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

#include <KlayGE/KlayGE.hpp>

#include <KFL/ErrorHandling.hpp>
#include <KFL/Hash.hpp>
#include <KFL/JsonDom.hpp>
#include <KlayGE/ResLoader.hpp>

#include <algorithm>
#include <iterator>
#include <fstream>

#include "MetadataUtil.hpp"

#include <KlayGE/DevHelper/MeshMetadata.hpp>

namespace KlayGE
{
	MeshMetadata::MeshMetadata() = default;

	MeshMetadata::MeshMetadata(std::string_view name)
	{
		this->Load(name);
	}

	void MeshMetadata::Load(std::string_view name)
	{
		MeshMetadata new_metadata;

		ResIdentifierPtr metadata_file = ResLoader::Instance().Open(name);
		if (metadata_file)
		{
			auto document = LoadJson(*metadata_file);
			auto* root_value = document->RootValue();

			uint32_t const version = root_value->Member("version")->ValueInt();
			Verify(version == 1);

			if (auto const* auto_center_val = root_value->Member("auto_center"))
			{
				BOOST_ASSERT(auto_center_val->Type() == JsonValueType::Bool);
				new_metadata.auto_center_ = auto_center_val->ValueBool();
			}

			if (auto const* pivot_val = root_value->Member("pivot"))
			{
				BOOST_ASSERT(pivot_val->Type() == JsonValueType::Array);
				auto const& values = pivot_val->ValueArray();
				BOOST_ASSERT(values.size() >= new_metadata.pivot_.size());
				uint32_t index = 0;
				for (auto iter = values.begin(); (iter != values.end()) && (index < new_metadata.pivot_.size()); ++iter, ++index)
				{
					new_metadata.pivot_[index] = GetFloat(*(*iter));
				}
			}

			if (auto const* translation_val = root_value->Member("translation"))
			{
				BOOST_ASSERT(translation_val->Type() == JsonValueType::Array);
				auto const& values = translation_val->ValueArray();
				BOOST_ASSERT(values.size() >= new_metadata.translation_.size());
				uint32_t index = 0;
				for (auto iter = values.begin(); (iter != values.end()) && (index < new_metadata.translation_.size()); ++iter, ++index)
				{
					new_metadata.translation_[index] = GetFloat(*(*iter));
				}
			}

			if (auto const* rotation_val = root_value->Member("rotation"))
			{
				BOOST_ASSERT(rotation_val->Type() == JsonValueType::Array);
				auto const& values = rotation_val->ValueArray();
				BOOST_ASSERT(values.size() >= new_metadata.rotation_.size());
				uint32_t index = 0;
				for (auto iter = values.begin(); (iter != values.end()) && (index < new_metadata.rotation_.size()); ++iter, ++index)
				{
					new_metadata.rotation_[index] = GetFloat(*(*iter));
				}
			}

			if (auto const* scale_val = root_value->Member("scale"))
			{
				BOOST_ASSERT(scale_val->Type() == JsonValueType::Array);
				auto const& values = scale_val->ValueArray();
				BOOST_ASSERT(values.size() >= new_metadata.scale_.size());
				uint32_t index = 0;
				for (auto iter = values.begin(); (iter != values.end()) && (index < new_metadata.scale_.size()); ++iter, ++index)
				{
					new_metadata.scale_[index] = GetFloat(*(*iter));
				}
			}

			if (auto const* axis_mapping_val = root_value->Member("axis_mapping"))
			{
				BOOST_ASSERT(axis_mapping_val->Type() == JsonValueType::Array);
				auto const& values = axis_mapping_val->ValueArray();
				BOOST_ASSERT(values.size() == 3);
				uint32_t index = 0;
				for (auto iter = values.begin(); (iter != values.end()) && (index < 3); ++iter, ++index)
				{
					new_metadata.axis_mapping_[index] = static_cast<uint8_t>(GetInt(**iter));
				}
			}

			if (auto const* flip_winding_order_val = root_value->Member("flip_winding_order"))
			{
				BOOST_ASSERT(flip_winding_order_val->Type() == JsonValueType::Bool);
				new_metadata.flip_winding_order_ = flip_winding_order_val->ValueBool();
			}

			if (auto const* materials_val = root_value->Member("materials"))
			{
				BOOST_ASSERT(materials_val->Type() == JsonValueType::Array);
				auto const& values = materials_val->ValueArray();
				new_metadata.material_file_names_.reserve(values.size());
				for (auto const& value : values)
				{
					BOOST_ASSERT(value->Type() == JsonValueType::String);
					new_metadata.material_file_names_.emplace_back(value->ValueString());
				}
			}

			if (auto const* source_val = root_value->Member("source"))
			{
				if (source_val->Type() == JsonValueType::String)
				{
					new_metadata.lod_file_names_.assign(1, std::string(source_val->ValueString()));
				}
				else
				{
					if (auto const* lod_val = source_val->Member("lod"))
					{
						BOOST_ASSERT(lod_val->Type() == JsonValueType::Array);
						auto const& values = lod_val->ValueArray();
						new_metadata.lod_file_names_.reserve(values.size());
						for (auto const& value : values)
						{
							BOOST_ASSERT(value->Type() == JsonValueType::String);
							new_metadata.lod_file_names_.emplace_back(value->ValueString());
						}
					}
				}
			}

			new_metadata.UpdateTransforms();
		}
		else if(!name.empty())
		{
			LogInfo() << "Could NOT find " << name << ". Fallback to default metadata." << std::endl;
		}		

		if (new_metadata.lod_file_names_.empty())
		{
			std::string_view const implicit_source_name = name.substr(0, name.rfind(".kmeta"));
			new_metadata.lod_file_names_.assign(1, std::string(implicit_source_name));
		}

		*this = std::move(new_metadata);
	}

	void MeshMetadata::Save(std::string const & name) const
	{
		JsonDocument doc;
		auto root_value = doc.AllocValue(JsonValueType::Object);

		root_value->AppendValue("version", doc.AllocValueUInt(1U));

		if (auto_center_)
		{
			root_value->AppendValue("auto_center", doc.AllocValueBool(auto_center_));
		}

		if (MathLib::length_sq(pivot_) > 1e-6f)
		{
			std::vector<std::unique_ptr<JsonValue>> values;
			values.reserve(pivot_.size());
			for (size_t i = 0; i < pivot_.size(); ++ i)
			{
				values.emplace_back(doc.AllocValueFloat(pivot_[i]));
			}
			root_value->AppendValue("pivot", doc.AllocValueArray(std::move(values)));
		}

		if (MathLib::length_sq(translation_) > 1e-6f)
		{
			std::vector<std::unique_ptr<JsonValue>> values;
			values.reserve(translation_.size());
			for (size_t i = 0; i < translation_.size(); ++ i)
			{
				values.emplace_back(doc.AllocValueFloat(translation_[i]));
			}
			root_value->AppendValue("translation", doc.AllocValueArray(std::move(values)));
		}

		if (!MathLib::equal(rotation_.x(), 0.0f) || !MathLib::equal(rotation_.y(), 0.0f) || !MathLib::equal(rotation_.z(), 0.0f)
			|| !MathLib::equal(rotation_.w(), 1.0f))
		{
			std::vector<std::unique_ptr<JsonValue>> values;
			values.reserve(rotation_.size());
			for (size_t i = 0; i < rotation_.size(); ++ i)
			{
				values.emplace_back(doc.AllocValueFloat(rotation_[i]));
			}
			root_value->AppendValue("rotation", doc.AllocValueArray(std::move(values)));
		}

		if (!MathLib::equal(scale_.x(), 1.0f) || !MathLib::equal(scale_.y(), 1.0f) || !MathLib::equal(scale_.z(), 1.0f))
		{
			std::vector<std::unique_ptr<JsonValue>> values;
			values.reserve(scale_.size());
			for (size_t i = 0; i < scale_.size(); ++ i)
			{
				values.emplace_back(doc.AllocValueFloat(scale_[i]));
			}
			root_value->AppendValue("scale", doc.AllocValueArray(std::move(values)));
		}

		bool need_swizzle = false;
		for (size_t i = 0; i < std::size(axis_mapping_); ++ i)
		{
			if (axis_mapping_[i] != i)
			{
				need_swizzle = true;
				break;
			}
		}
		if (need_swizzle)
		{
			std::vector<std::unique_ptr<JsonValue>> values;
			values.reserve(std::size(axis_mapping_));
			for (size_t i = 0; i < std::size(axis_mapping_); ++ i)
			{
				values.emplace_back(doc.AllocValueInt(static_cast<int>(axis_mapping_[i])));
			}
			root_value->AppendValue("axis_mapping", doc.AllocValueArray(std::move(values)));
		}

		if (flip_winding_order_)
		{
			root_value->AppendValue("flip_winding_order", doc.AllocValueBool(flip_winding_order_));
		}

		if (!material_file_names_.empty())
		{
			std::vector<std::unique_ptr<JsonValue>> values;
			values.reserve(material_file_names_.size());
			for (size_t i = 0; i < material_file_names_.size(); ++i)
			{
				values.emplace_back(doc.AllocValueString(material_file_names_[i]));
			}
			root_value->AppendValue("materials", doc.AllocValueArray(std::move(values)));
		}

		if (lod_file_names_.size() == 1)
		{
			root_value->AppendValue("source", doc.AllocValueString(lod_file_names_[0]));
		}
		else
		{
			std::vector<std::unique_ptr<JsonValue>> lod_values;
			lod_values.reserve(lod_file_names_.size());
			for (size_t i = 0; i < lod_file_names_.size(); ++i)
			{
				lod_values.emplace_back(doc.AllocValueString(lod_file_names_[i]));
			}

			std::vector<std::pair<std::string, std::unique_ptr<JsonValue>>> lod_val(1);
			lod_val[0] = {"lod", doc.AllocValueArray(std::move(lod_values))};
			root_value->AppendValue("source", doc.AllocValueObject(std::move(lod_val)));
		}

		doc.RootValue(std::move(root_value));

		std::ofstream ofs(name);
		SaveJson(doc, ofs);
	}

	uint32_t MeshMetadata::NumLods() const
	{
		return lod_file_names_.empty() ? 1 : static_cast<uint32_t>(lod_file_names_.size());
	}

	void MeshMetadata::NumLods(uint32_t lods)
	{
		lod_file_names_.resize(lods);
	}

	std::string_view MeshMetadata::LodFileName(uint32_t lod) const
	{
		return lod_file_names_[lod];
	}

	void MeshMetadata::LodFileName(uint32_t lod, std::string_view lod_name)
	{
		lod_file_names_[lod] = std::string(lod_name);
	}

	uint32_t MeshMetadata::NumMaterials() const
	{
		return static_cast<uint32_t>(material_file_names_.size());
	}

	void MeshMetadata::NumMaterials(uint32_t materials)
	{
		material_file_names_.resize(materials);
	}

	std::string_view MeshMetadata::MaterialFileName(uint32_t mtl_index) const
	{
		return material_file_names_[mtl_index];
	}

	void MeshMetadata::MaterialFileName(uint32_t mtl_index, std::string_view mtlml_name)
	{
		material_file_names_[mtl_index] = std::string(mtlml_name);
	}

	void MeshMetadata::UpdateTransforms()
	{
		transform_ = MathLib::transformation(&pivot_, static_cast<Quaternion*>(nullptr),
			&scale_,
			&pivot_, &rotation_, &translation_);

		float4 const axis[3] = { transform_.Col(0), transform_.Col(1), transform_.Col(2) };
		transform_.Col(0, axis[axis_mapping_[0]]);
		transform_.Col(1, axis[axis_mapping_[1]]);
		transform_.Col(2, axis[axis_mapping_[2]]);

		transform_it_ = MathLib::transpose(MathLib::inverse(transform_));
	}
}
