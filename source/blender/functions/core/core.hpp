#pragma once

#include <string>
#include <iostream>

#include "BLI_composition.hpp"
#include "BLI_small_vector.hpp"
#include "BLI_small_map.hpp"
#include "BLI_shared.hpp"

namespace FN {

	using namespace BLI;

	class Type;
	class Signature;
	class Function;

	using SharedType = Shared<Type>;
	using SharedFunction = Shared<Function>;
	using SmallTypeVector = SmallVector<SharedType>;

	class Type final {
	public:
		Type() = delete;
		Type(const std::string &name)
			: m_name(name) {}

		const std::string &name() const
		{
			return this->m_name;
		}

		template<typename T>
		inline const T *extension() const
		{
			return this->m_extensions.get<T>();
		}

		template<typename T>
		void extend(const T *extension)
		{
			BLI_assert(this->m_extensions.get<T>() == nullptr);
			this->m_extensions.add(extension);
		}

	protected:
		std::string m_name;

	private:
		Composition m_extensions;
	};

	class Parameter {
	public:
		Parameter(const std::string &name, const SharedType &type)
			: m_type(type), m_name(name) {}

		const SharedType &type() const
		{
			return this->m_type;
		}

		const std::string &name() const
		{
			return this->m_name;
		}

	private:
		const SharedType m_type;
		const std::string m_name;
	};

	class InputParameter : public Parameter {
	public:
		InputParameter(const std::string &name, const SharedType &type)
			: Parameter(name, type) {}
	};

	class OutputParameter : public Parameter {
	public:
		OutputParameter(const std::string &name, const SharedType &type)
			: Parameter(name, type) {}
	};

	using InputParameters = SmallVector<InputParameter>;
	using OutputParameters = SmallVector<OutputParameter>;

	class Signature {
	public:
		Signature() = default;
		~Signature() = default;

		Signature(const InputParameters &inputs, const OutputParameters &outputs)
			: m_inputs(inputs), m_outputs(outputs) {}

		inline const InputParameters &inputs() const
		{
			return this->m_inputs;
		}

		inline const OutputParameters &outputs() const
		{
			return this->m_outputs;
		}

		SmallTypeVector input_types() const
		{
			SmallTypeVector types;
			for (const InputParameter &param : this->inputs()) {
				types.append(param.type());
			}
			return types;
		}

		SmallTypeVector output_types() const
		{
			SmallTypeVector types;
			for (const OutputParameter &param : this->outputs()) {
				types.append(param.type());
			}
			return types;
		}

	private:
		const InputParameters m_inputs;
		const OutputParameters m_outputs;
	};

	class Function final {
	public:
		Function(const Signature &signature, const std::string &name = "Function")
			: m_signature(signature), m_name(name) {}

		~Function() = default;

		inline const Signature &signature() const
		{
			return this->m_signature;
		}

		template<typename T>
		inline const T *body() const
		{
			return this->m_bodies.get<T>();
		}

		template<typename T>
		void add_body(const T *body)
		{
			BLI_assert(this->m_bodies.get<T>() == nullptr);
			this->m_bodies.add(body);
		}

		const std::string &name() const
		{
			return this->m_name;
		}

	private:
		const Signature m_signature;
		Composition m_bodies;
		const std::string m_name;
	};

} /* namespace FN */