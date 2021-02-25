// RenderEffect.hpp
// KlayGE ��ȾЧ���ű��� ͷ�ļ�
// Ver 3.11.0
// ��Ȩ����(C) ������, 2003-2010
// Homepage: http://www.klayge.org
//
// 3.11.0
// Add RenderTechnique::Transparent() (2010.9.12)
//
// 3.9.0
// ֱ�Ӵ�fxml�ļ���ȡ��Ч�ű� (2009.4.21)
//
// 3.8.0
// ֧��CBuffer (2008.10.6)
//
// 3.6.0
// ������Clone (2007.6.11)
//
// 3.5.0
// ���û���xml����Ч��ʽ (2006.10.21)
//
// 3.4.0
// ��д��parameter�Ĵ洢�ṹ (2006.9.15)
//
// 3.2.0
// ֧����bool���� (2006.3.8)
//
// 3.0.0
// ������RenderTechnique��RenderPass (2005.9.4)
//
// 2.8.0
// ������Do*������ʹ��ģ�巽��ģʽ (2005.7.24)
// ʹ���µ��Զ����²����ķ��� (2005.7.25)
//
// 2.5.0
// ȥ����Clone (2005.4.16)
// SetTechnique�ķ������͸�Ϊbool (2005.4.25)
//
// 2.1.2
// ������Parameter (2004.5.26)
//
// 2.0.3
// �޸���SetTexture�Ĳ��� (2004.3.6)
// ������SetMatrixArray/GetMatrixArray (2004.3.11)
//
// 2.0.0
// ���ν��� (2003.8.15)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _RENDEREFFECT_HPP
#define _RENDEREFFECT_HPP

#pragma once

#include <KlayGE/PreDeclare.hpp>
#include <KFL/CXX20/span.hpp>
#include <vector>
#include <string>
#include <algorithm>

#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/Texture.hpp>
#include <KlayGE/ShaderObject.hpp>
#include <KFL/Math.hpp>

namespace KlayGE
{
	enum RenderEffectDataType
	{
		REDT_bool = 0,
		REDT_string,
		REDT_texture1D,
		REDT_texture2D,
		REDT_texture2DMS,
		REDT_texture3D,
		REDT_textureCUBE,
		REDT_texture1DArray,
		REDT_texture2DArray,
		REDT_texture2DMSArray,
		REDT_texture3DArray,
		REDT_textureCUBEArray,
		REDT_sampler,
		REDT_shader,
		REDT_uint,
		REDT_uint2,
		REDT_uint3,
		REDT_uint4,
		REDT_int,
		REDT_int2,
		REDT_int3,
		REDT_int4,
		REDT_float,
		REDT_float2,
		REDT_float2x2,
		REDT_float2x3,
		REDT_float2x4,
		REDT_float3,
		REDT_float3x2,
		REDT_float3x3,
		REDT_float3x4,
		REDT_float4,
		REDT_float4x2,
		REDT_float4x3,
		REDT_float4x4,
		REDT_buffer,
		REDT_structured_buffer,
		REDT_byte_address_buffer,
		REDT_rw_buffer,
		REDT_rw_structured_buffer,
		REDT_rw_texture1D,
		REDT_rw_texture2D,
		REDT_rw_texture3D,
		REDT_rw_texture1DArray,
		REDT_rw_texture2DArray,
		REDT_rw_byte_address_buffer,
		REDT_append_structured_buffer,
		REDT_consume_structured_buffer,
		REDT_rasterizer_ordered_buffer,
		REDT_rasterizer_ordered_byte_address_buffer,
		REDT_rasterizer_ordered_structured_buffer,
		REDT_rasterizer_ordered_texture1D,
		REDT_rasterizer_ordered_texture1DArray,
		REDT_rasterizer_ordered_texture2D,
		REDT_rasterizer_ordered_texture2DArray,
		REDT_rasterizer_ordered_texture3D,
		REDT_struct,

		REDT_count
	};
	static_assert(REDT_count < 256);

	class KLAYGE_CORE_API RenderVariable : boost::noncopyable
	{
	public:
		RenderVariable();
		virtual ~RenderVariable() = 0;

		virtual RenderEffectStructType* StructType() const
		{
			return nullptr;
		}

		virtual std::unique_ptr<RenderVariable> Clone() = 0;

		virtual RenderVariable& operator=(bool const & value);
		virtual RenderVariable& operator=(uint32_t const & value);
		virtual RenderVariable& operator=(int32_t const & value);
		virtual RenderVariable& operator=(float const & value);
		virtual RenderVariable& operator=(uint2 const & value);
		virtual RenderVariable& operator=(uint3 const & value);
		virtual RenderVariable& operator=(uint4 const & value);
		virtual RenderVariable& operator=(int2 const & value);
		virtual RenderVariable& operator=(int3 const & value);
		virtual RenderVariable& operator=(int4 const & value);
		virtual RenderVariable& operator=(float2 const & value);
		virtual RenderVariable& operator=(float3 const & value);
		virtual RenderVariable& operator=(float4 const & value);
		virtual RenderVariable& operator=(float4x4 const & value);
		virtual RenderVariable& operator=(TexturePtr const & value);
		virtual RenderVariable& operator=(ShaderResourceViewPtr const & value);
		virtual RenderVariable& operator=(UnorderedAccessViewPtr const & value);
		virtual RenderVariable& operator=(SamplerStateObjectPtr const & value);
		virtual RenderVariable& operator=(std::string const & value);
		virtual RenderVariable& operator=(ShaderDesc const & value);
		virtual RenderVariable& operator=(std::vector<bool> const & value);
		virtual RenderVariable& operator=(std::vector<uint32_t> const & value);
		virtual RenderVariable& operator=(std::vector<int32_t> const & value);
		virtual RenderVariable& operator=(std::vector<float> const & value);
		virtual RenderVariable& operator=(std::vector<uint2> const & value);
		virtual RenderVariable& operator=(std::vector<uint3> const & value);
		virtual RenderVariable& operator=(std::vector<uint4> const & value);
		virtual RenderVariable& operator=(std::vector<int2> const & value);
		virtual RenderVariable& operator=(std::vector<int3> const & value);
		virtual RenderVariable& operator=(std::vector<int4> const & value);
		virtual RenderVariable& operator=(std::vector<float2> const & value);
		virtual RenderVariable& operator=(std::vector<float3> const & value);
		virtual RenderVariable& operator=(std::vector<float4> const & value);
		virtual RenderVariable& operator=(std::vector<float4x4> const & value);
		virtual RenderVariable& operator=(std::span<bool const> value);
		virtual RenderVariable& operator=(std::span<uint32_t const> value);
		virtual RenderVariable& operator=(std::span<int32_t const> value);
		virtual RenderVariable& operator=(std::span<float const> value);
		virtual RenderVariable& operator=(std::span<uint2 const> value);
		virtual RenderVariable& operator=(std::span<uint3 const> value);
		virtual RenderVariable& operator=(std::span<uint4 const> value);
		virtual RenderVariable& operator=(std::span<int2 const> value);
		virtual RenderVariable& operator=(std::span<int3 const> value);
		virtual RenderVariable& operator=(std::span<int4 const> value);
		virtual RenderVariable& operator=(std::span<float2 const> value);
		virtual RenderVariable& operator=(std::span<float3 const> value);
		virtual RenderVariable& operator=(std::span<float4 const> value);
		virtual RenderVariable& operator=(std::span<float4x4 const> value);

		// For struct
		virtual RenderVariable& operator=(std::vector<uint8_t> const& value);
		virtual RenderVariable& operator=(std::span<uint8_t const> value);

		virtual void Value(bool& val) const;
		virtual void Value(uint32_t& val) const;
		virtual void Value(int32_t& val) const;
		virtual void Value(float& val) const;
		virtual void Value(uint2& val) const;
		virtual void Value(uint3& val) const;
		virtual void Value(uint4& val) const;
		virtual void Value(int2& val) const;
		virtual void Value(int3& val) const;
		virtual void Value(int4& val) const;
		virtual void Value(float2& val) const;
		virtual void Value(float3& val) const;
		virtual void Value(float4& val) const;
		virtual void Value(float4x4& val) const;
		virtual void Value(TexturePtr& val) const;
		virtual void Value(ShaderResourceViewPtr& val) const;
		virtual void Value(UnorderedAccessViewPtr& val) const;
		virtual void Value(SamplerStateObjectPtr& val) const;
		virtual void Value(std::string& val) const;
		virtual void Value(ShaderDesc& val) const;
		virtual void Value(std::vector<bool>& val) const;
		virtual void Value(std::vector<uint32_t>& val) const;
		virtual void Value(std::vector<int32_t>& val) const;
		virtual void Value(std::vector<float>& val) const;
		virtual void Value(std::vector<uint2>& val) const;
		virtual void Value(std::vector<uint3>& val) const;
		virtual void Value(std::vector<uint4>& val) const;
		virtual void Value(std::vector<int2>& val) const;
		virtual void Value(std::vector<int3>& val) const;
		virtual void Value(std::vector<int4>& val) const;
		virtual void Value(std::vector<float2>& val) const;
		virtual void Value(std::vector<float3>& val) const;
		virtual void Value(std::vector<float4>& val) const;
		virtual void Value(std::vector<float4x4>& val) const;

		// For struct
		virtual void Value(std::vector<uint8_t>& val) const;

		virtual void BindToCBuffer(RenderEffect const& effect, uint32_t cbuff_index, uint32_t offset, uint32_t stride);
		virtual void RebindToCBuffer(RenderEffect const& effect, uint32_t cbuff_index);
		virtual bool InCBuffer() const
		{
			return false;
		}
		virtual RenderEffectConstantBuffer* CBuffer() const
		{
			return nullptr;
		}
		virtual uint32_t CBufferIndex() const
		{
			return 0;
		}
		virtual uint32_t CBufferOffset() const
		{
			return 0;
		}
		virtual uint32_t Stride() const
		{
			return 0;
		}

	protected:
		struct CBufferDesc
		{
			RenderEffect const* effect;
			uint32_t cbuff_index;
			uint32_t offset;
			uint32_t stride;
		};
	};

	class KLAYGE_CORE_API RenderEffectAnnotation final : boost::noncopyable
	{
	public:
#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, DomNode const& node);
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res);
#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const;
#endif

		RenderEffectDataType Type() const
		{
			return type_;
		}
		std::string const & Name() const
		{
			return name_;
		}

		template <typename T>
		void Value(T& val) const
		{
			var_->Value(val);
		}

	private:
		RenderEffectDataType type_;
		std::string name_;

		std::unique_ptr<RenderVariable> var_;
	};

	class KLAYGE_CORE_API RenderShaderFragment final
	{
	public:
#if KLAYGE_IS_DEV_PLATFORM
		void Load(DomNode const& node);
#endif

		void StreamIn(ResIdentifier& res);
#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const;
#endif

		ShaderStage Stage() const
		{
			return stage_;
		}

		ShaderModel Version() const
		{
			return ver_;
		}

		std::string const & str() const
		{
			return str_;
		}

	private:
		ShaderStage stage_;
		ShaderModel ver_;
		std::string str_;
	};

	class KLAYGE_CORE_API RenderShaderGraphNode final
	{
	public:
#if KLAYGE_IS_DEV_PLATFORM
		void Load(DomNode const& node);
#endif

		void StreamIn(ResIdentifier& res);
#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const;
#endif

		std::string const & Name() const
		{
			return name_;
		}
		size_t NameHash() const
		{
			return name_hash_;
		}

		std::string const & ReturnType() const
		{
			return return_type_;
		}

		uint32_t NumParameters() const
		{
			return static_cast<uint32_t>(params_.size());
		}
		std::pair<std::string, std::string> const & Parameter(uint32_t n) const
		{
			BOOST_ASSERT(n < this->NumParameters());
			return params_[n];
		}

		std::string const & ImplName() const
		{
			return impl_;
		}

		void OverrideImpl(std::string_view impl)
		{
			impl_ = std::string(impl);
		}

#if KLAYGE_IS_DEV_PLATFORM
		std::string GenDeclarationCode() const;
		std::string GenDefinitionCode() const;
#endif

	private:
		std::string name_;
		size_t name_hash_;

		std::string return_type_;
		std::vector<std::pair<std::string, std::string>> params_;
		std::string impl_;
	};

	class KLAYGE_CORE_API RenderEffectStructType final : boost::noncopyable
	{
	public:
#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, DomNode const& node);
#endif

		void StreamIn(ResIdentifier& res);
#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const;
#endif

		std::string const& Name() const
		{
			return name_;
		}
		size_t NameHash() const
		{
			return name_hash_;
		}

		uint32_t NumMembers() const;
		RenderEffectDataType MemberType(uint32_t index) const;
		std::string const& MemberTypeName(uint32_t index) const;
		std::string const& MemberName(uint32_t index) const;
		std::shared_ptr<std::string> const& MemberArraySize(uint32_t index) const;

	private:
		std::string name_;
		size_t name_hash_;

		std::vector<std::tuple<RenderEffectDataType, std::string, std::string, std::shared_ptr<std::string>>> members_;
	};

	// ��ȾЧ��
	//////////////////////////////////////////////////////////////////////////////////
	class KLAYGE_CORE_API RenderEffect final : boost::noncopyable
	{
		friend class RenderEffectTemplate;

	public:
		void Load(std::span<std::string const> names);
#if KLAYGE_IS_DEV_PLATFORM
		void CompileShaders();
#endif
		void CreateHwShaders();

		RenderEffectPtr Clone();
		void CloneInPlace(RenderEffect& dst_effect);
		void Reclone(RenderEffect& dst_effect);

		bool HWResourceReady() const;

		std::string const & ResName() const;
		size_t ResNameHash() const;

		uint32_t NumParameters() const
		{
			return static_cast<uint32_t>(params_.size());
		}
		RenderEffectParameter* ParameterBySemantic(std::string_view semantic) const;
		RenderEffectParameter* ParameterByName(std::string_view name) const;
		RenderEffectParameter* ParameterByIndex(uint32_t n) const
		{
			BOOST_ASSERT(n < this->NumParameters());
			return params_[n].get();
		}

		uint32_t NumCBuffers() const
		{
			return static_cast<uint32_t>(cbuffers_.size());
		}
		RenderEffectConstantBuffer* CBufferByName(std::string_view name) const;
		uint32_t FindCBuffer(std::string_view name) const;
		RenderEffectConstantBuffer* CBufferByIndex(uint32_t index) const
		{
			BOOST_ASSERT(index < this->NumCBuffers());
			return cbuffers_[index].get();
		}
		void BindCBufferByName(std::string_view name, RenderEffectConstantBufferPtr const& cbuff);
		void BindCBufferByIndex(uint32_t index, RenderEffectConstantBufferPtr const& cbuff)
		{
			BOOST_ASSERT(index < this->NumCBuffers());
			cbuffers_[index] = cbuff;
		}

		uint32_t NumStructTypes() const;
		RenderEffectStructType* StructTypeByName(std::string_view name) const;
		RenderEffectStructType* StructTypeByIndex(uint32_t index) const;

		uint32_t NumTechniques() const;
		RenderTechnique* TechniqueByName(std::string_view name) const;
		RenderTechnique* TechniqueByIndex(uint32_t n) const;

		uint32_t NumShaderFragments() const;
		RenderShaderFragment const & ShaderFragmentByIndex(uint32_t n) const;

		uint32_t AddShaderDesc(ShaderDesc const & sd);
		ShaderDesc& GetShaderDesc(uint32_t id);
		ShaderDesc const & GetShaderDesc(uint32_t id) const;

		uint32_t NumMacros() const;
		std::pair<std::string, std::string> const & MacroByIndex(uint32_t n) const;

		uint32_t AddShaderObject();
		ShaderObjectPtr const & ShaderObjectByIndex(uint32_t n) const
		{
			BOOST_ASSERT(n < shader_objs_.size());
			return shader_objs_[n];
		}

#if KLAYGE_IS_DEV_PLATFORM
		void GenHLSLShaderText();
		std::string const & HLSLShaderText() const;
#endif
		
	private:
		RenderEffectTemplatePtr effect_template_;

		std::vector<std::unique_ptr<RenderEffectParameter>> params_;
		std::vector<RenderEffectConstantBufferPtr> cbuffers_;
		std::vector<ShaderObjectPtr> shader_objs_;

		mutable bool hw_res_ready_ = false;
	};

	class KLAYGE_CORE_API RenderEffectTemplate final : boost::noncopyable
	{
	public:
		void Load(std::span<std::string const> names, RenderEffect& effect);
#if KLAYGE_IS_DEV_PLATFORM
		void CompileShaders(RenderEffect& effect);
#endif
		void CreateHwShaders(RenderEffect& effect);

		bool StreamIn(ResIdentifier& source, RenderEffect& effect);
#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os, RenderEffect const & effect) const;
#endif

		std::string const & ResName() const
		{
			return res_name_;
		}
		size_t ResNameHash() const
		{
			return res_name_hash_;
		}

		uint32_t NumStructTypes() const
		{
			return static_cast<uint32_t>(struct_types_.size());
		}
		RenderEffectStructType* StructTypeByName(std::string_view name) const;
		RenderEffectStructType* StructTypeByIndex(uint32_t index) const
		{
			BOOST_ASSERT(index < this->NumStructTypes());
			return struct_types_[index].get();
		}

		uint32_t NumTechniques() const
		{
			return static_cast<uint32_t>(techniques_.size());
		}
		RenderTechnique* TechniqueByName(std::string_view name) const;
		RenderTechnique* TechniqueByIndex(uint32_t n) const
		{
			BOOST_ASSERT(n < this->NumTechniques());
			return techniques_[n].get();
		}

		uint32_t NumShaderFragments() const
		{
			return static_cast<uint32_t>(shader_frags_.size());
		}
		RenderShaderFragment const & ShaderFragmentByIndex(uint32_t n) const
		{
			BOOST_ASSERT(n < this->NumShaderFragments());
			return shader_frags_[n];
		}

		uint32_t AddShaderDesc(ShaderDesc const & sd);
		ShaderDesc& GetShaderDesc(uint32_t id);
		ShaderDesc const & GetShaderDesc(uint32_t id) const;

		uint32_t NumMacros() const
		{
			return static_cast<uint32_t>(macros_.size());
		}
		std::pair<std::string, std::string> const & MacroByIndex(uint32_t n) const
		{
			BOOST_ASSERT(n < this->NumMacros());
			return macros_[n].first;
		}

		uint32_t NumShaderGraphNodes() const
		{
			return static_cast<uint32_t>(shader_graph_nodes_.size());
		}
		RenderShaderGraphNode const & ShaderGraphNodesByIndex(uint32_t n) const
		{
			BOOST_ASSERT(n < this->NumShaderGraphNodes());
			return shader_graph_nodes_[n];
		}

#if KLAYGE_IS_DEV_PLATFORM
		void GenHLSLShaderText(RenderEffect const & effect);
		std::string const & HLSLShaderText() const
		{
			return hlsl_shader_;
		}
#endif

	private:
#if KLAYGE_IS_DEV_PLATFORM
		void PreprocessIncludes(DomDocument& doc, DomNode& root, std::vector<std::unique_ptr<DomDocument>>& include_docs);
		void RecursiveIncludeNode(DomNode const & root, std::vector<std::string>& include_names) const;
		void InsertIncludeNodes(
			DomDocument& target_doc, DomNode& target_root, DomNode const& target_place, DomNode const& include_root) const;

		std::unique_ptr<DomNode> ResolveInheritTechNode(DomDocument& doc, DomNode& root, DomNode const* tech_node);
		void ResolveOverrideTechs(DomDocument& doc, DomNode& root);

		void Load(DomNode const & root, RenderEffect& effect);
#endif

	private:
		std::string res_name_;
		size_t res_name_hash_;
#if KLAYGE_IS_DEV_PLATFORM
		uint64_t timestamp_;

		std::string kfx_name_;
		bool need_compile_;
#endif

		std::vector<std::unique_ptr<RenderEffectStructType>> struct_types_;

		std::vector<std::unique_ptr<RenderTechnique>> techniques_;

		std::vector<std::pair<std::pair<std::string, std::string>, bool>> macros_;
		std::vector<RenderShaderFragment> shader_frags_;
#if KLAYGE_IS_DEV_PLATFORM
		std::string hlsl_shader_;
#endif

		std::vector<ShaderDesc> shader_descs_;

		std::vector<RenderShaderGraphNode> shader_graph_nodes_;
	};

	class KLAYGE_CORE_API RenderTechnique final : boost::noncopyable
	{
	public:
#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect& effect, DomNode const& node, uint32_t tech_index);
		void CompileShaders(RenderEffect& effect, uint32_t tech_index);
#endif
		void CreateHwShaders(RenderEffect& effect, uint32_t tech_index);

		bool StreamIn(RenderEffect& effect, ResIdentifier& res, uint32_t tech_index);
#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(RenderEffect const & effect, std::ostream& os, uint32_t tech_index) const;
#endif

		std::string const & Name() const
		{
			return name_;
		}
		size_t NameHash() const
		{
			return name_hash_;
		}

		uint32_t NumAnnotations() const
		{
			return annotations_ ? static_cast<uint32_t>(annotations_->size()) : 0;
		}
		RenderEffectAnnotation const & Annotation(uint32_t n) const
		{
			BOOST_ASSERT(n < this->NumAnnotations());
			return *(*annotations_)[n];
		}

		uint32_t NumMacros() const
		{
			return macros_ ? static_cast<uint32_t>(macros_->size()) : 0;
		}
		std::pair<std::string, std::string> const & MacroByIndex(uint32_t n) const
		{
			BOOST_ASSERT(n < this->NumMacros());
			return (*macros_)[n];
		}

		uint32_t NumPasses() const
		{
			return static_cast<uint32_t>(passes_.size());
		}
		RenderPass const & Pass(uint32_t n) const
		{
			BOOST_ASSERT(n < this->NumPasses());
			return *passes_[n];
		}

		bool Validate() const
		{
			return is_validate_;
		}

		bool HWResourceReady(RenderEffect const& effect) const;

		float Weight() const
		{
			return weight_;
		}

		bool Transparent() const
		{
			return transparent_;
		}

		bool HasDiscard() const
		{
			return has_discard_;
		}
		bool HasTessellation() const
		{
			return has_tessellation_;
		}

	private:
		std::string name_;
		size_t name_hash_;

		std::vector<RenderPassPtr> passes_;
		std::shared_ptr<std::vector<RenderEffectAnnotationPtr>> annotations_;
		std::shared_ptr<std::vector<std::pair<std::string, std::string>>> macros_;

		float weight_;
		bool transparent_;

		bool is_validate_;
		bool has_discard_;
		bool has_tessellation_;
	};

	class KLAYGE_CORE_API RenderPass final : boost::noncopyable
	{
	public:
#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect& effect, DomNode const& node, uint32_t tech_index, uint32_t pass_index, RenderPass const* inherit_pass);
		void Load(RenderEffect& effect, uint32_t tech_index, uint32_t pass_index, RenderPass const* inherit_pass);
		void CompileShaders(RenderEffect& effect, uint32_t tech_index, uint32_t pass_index);
#endif
		void CreateHwShaders(RenderEffect& effect, uint32_t tech_index, uint32_t pass_index);

		bool StreamIn(RenderEffect& effect, ResIdentifier& res, uint32_t tech_index, uint32_t pass_index);
#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(RenderEffect const & effect, std::ostream& os, uint32_t tech_index, uint32_t pass_index) const;
#endif

		std::string const & Name() const
		{
			return name_;
		}
		size_t NameHash() const
		{
			return name_hash_;
		}

		void Bind(RenderEffect const & effect) const;
		void Unbind(RenderEffect const & effect) const;

		bool Validate() const
		{
			return is_validate_;
		}

		RenderStateObjectPtr const & GetRenderStateObject() const
		{
			return render_state_obj_;
		}
		ShaderObjectPtr const & GetShaderObject(RenderEffect const & effect) const
		{
			return effect.ShaderObjectByIndex(shader_obj_index_);
		}

		uint32_t NumAnnotations() const
		{
			return annotations_ ? static_cast<uint32_t>(annotations_->size()) : 0;
		}
		RenderEffectAnnotation const & Annotation(uint32_t n) const
		{
			BOOST_ASSERT(n < this->NumAnnotations());
			return *(*annotations_)[n];
		}

		uint32_t NumMacros() const
		{
			return macros_ ? static_cast<uint32_t>(macros_->size()) : 0;
		}
		std::pair<std::string, std::string> const & MacroByIndex(uint32_t n) const
		{
			BOOST_ASSERT(n < this->NumMacros());
			return (*macros_)[n];
		}

	private:
		std::string name_;
		size_t name_hash_;
		std::shared_ptr<std::vector<RenderEffectAnnotationPtr>> annotations_;
		std::shared_ptr<std::vector<std::pair<std::string, std::string>>> macros_;
		std::array<uint32_t, NumShaderStages> shader_desc_ids_;

		RenderStateObjectPtr render_state_obj_;
		uint32_t shader_obj_index_;

		bool is_validate_;
	};

	class KLAYGE_CORE_API RenderEffectConstantBuffer final : boost::noncopyable
	{
	public:
		explicit RenderEffectConstantBuffer(RenderEffect const& effect) : effect_(&effect), dirty_(true)
		{
		}

#if KLAYGE_IS_DEV_PLATFORM
		void Load(std::string const & name);
#endif

		void StreamIn(ResIdentifier& res);
#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const;
#endif

		RenderEffectConstantBufferPtr Clone(RenderEffect const& dst_effect);
		void Reclone(RenderEffectConstantBuffer& dst_cbuffer, RenderEffect const& dst_effect);
		RenderEffect const& OwnerEffect() const
		{
			return *effect_;
		}

		std::string const & Name() const
		{
			return name_->first;
		}
		size_t NameHash() const
		{
			return name_->second;
		}

		void AddParameter(uint32_t index);

		uint32_t NumParameters() const
		{
			return param_indices_ ? static_cast<uint32_t>(param_indices_->size()) : 0;
		}
		uint32_t ParameterIndex(uint32_t index) const
		{
			return (*param_indices_)[index];
		}

		void Resize(uint32_t size);
		uint32_t Size() const
		{
			return static_cast<uint32_t>(buff_.size());
		}

		template <typename T>
		T const * VariableInBuff(uint32_t offset) const
		{
			union Raw2T
			{
				uint8_t const * raw;
				T const * t;
			} r2t;
			r2t.raw = &buff_[offset];
			return r2t.t;
		}
		template <typename T>
		T* VariableInBuff(uint32_t offset)
		{
			union Raw2T
			{
				uint8_t* raw;
				T* t;
			} r2t;
			r2t.raw = &buff_[offset];
			return r2t.t;
		}

		void Dirty(bool dirty)
		{
			dirty_ = dirty;
		}
		bool Dirty() const
		{
			return dirty_;
		}

		void Update();
		GraphicsBufferPtr const & HWBuff() const
		{
			return hw_buff_;
		}
		void BindHWBuff(GraphicsBufferPtr const & buff);

	private:
		void RebindParameters(RenderEffectConstantBuffer& dst_cbuffer, RenderEffect const& dst_effect);

	private:
		RenderEffect const* effect_;

		std::shared_ptr<std::pair<std::string, size_t>> name_;
		std::shared_ptr<std::vector<uint32_t>> param_indices_;

		GraphicsBufferPtr hw_buff_;
		std::vector<uint8_t> buff_;
		bool dirty_;
	};

	class KLAYGE_CORE_API RenderEffectParameter final : boost::noncopyable
	{
	public:
#if KLAYGE_IS_DEV_PLATFORM
		void Load(RenderEffect const& effect, DomNode const& node);
#endif

		void StreamIn(RenderEffect const& effect, ResIdentifier& res);
#if KLAYGE_IS_DEV_PLATFORM
		void StreamOut(std::ostream& os) const;
#endif

		std::unique_ptr<RenderEffectParameter> Clone();

		RenderEffectDataType Type() const
		{
			return type_;
		}

		RenderEffectStructType* StructType() const
		{
			return var_->StructType();
		}

		RenderVariable const & Var() const
		{
			BOOST_ASSERT(var_);
			return *var_;
		}

		std::shared_ptr<std::string> const & ArraySize() const
		{
			return array_size_;
		}

		std::string const & Name() const
		{
			return name_->first;
		}
		size_t NameHash() const
		{
			return name_->second;
		}
		bool HasSemantic() const
		{
			return !!semantic_;
		}
		std::string const & Semantic() const;
		size_t SemanticHash() const;

		uint32_t NumAnnotations() const
		{
			return annotations_ ? static_cast<uint32_t>(annotations_->size()) : 0;
		}
		RenderEffectAnnotation const & Annotation(uint32_t n) const
		{
			BOOST_ASSERT(n < this->NumAnnotations());
			return *(*annotations_)[n];
		}

		template <typename T>
		RenderEffectParameter& operator=(T const & value)
		{
			*var_ = value;
			return *this;
		}

		template <typename T>
		void Value(T& val) const
		{
			var_->Value(val);
		}

		void BindToCBuffer(RenderEffect const& effect, uint32_t cbuff_index, uint32_t offset, uint32_t stride);
		void RebindToCBuffer(RenderEffect const& effect, uint32_t cbuff_index);
		RenderEffectConstantBuffer& CBuffer() const
		{
			BOOST_ASSERT(this->InCBuffer());
			return *var_->CBuffer();
		}
		bool InCBuffer() const
		{
			return var_->InCBuffer();
		}
		uint32_t CBufferIndex() const
		{
			return var_->CBufferIndex();
		}
		uint32_t CBufferOffset() const
		{
			return var_->CBufferOffset();
		}
		uint32_t Stride() const
		{
			return var_->Stride();
		}
		template <typename T>
		T const * MemoryInCBuff() const
		{
			return this->CBuffer().template VariableInBuff<T>(var_->CBufferOffset());
		}
		template <typename T>
		T* MemoryInCBuff()
		{
			return this->CBuffer().template VariableInBuff<T>(var_->CBufferOffset());
		}

	private:
		std::shared_ptr<std::pair<std::string, size_t>> name_;
		std::shared_ptr<std::pair<std::string, size_t>> semantic_;

		RenderEffectDataType type_;
		std::unique_ptr<RenderVariable> var_;
		std::shared_ptr<std::string> array_size_;

		std::shared_ptr<std::vector<std::unique_ptr<RenderEffectAnnotation>>> annotations_;
	};

	KLAYGE_CORE_API RenderEffectPtr SyncLoadRenderEffect(std::string_view effect_names);
	KLAYGE_CORE_API RenderEffectPtr SyncLoadRenderEffects(std::span<std::string const> effect_names);
	KLAYGE_CORE_API RenderEffectPtr ASyncLoadRenderEffect(std::string_view effect_name);
	KLAYGE_CORE_API RenderEffectPtr ASyncLoadRenderEffects(std::span<std::string const> effect_names);
}

#endif		// _RENDEREFFECT_HPP
